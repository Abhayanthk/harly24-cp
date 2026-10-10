#!/usr/bin/env bash
# install.sh: wire harly24-cp into Sublime Text, your shell and a workspace folder. Safe to re-run.
#
#   ./install.sh                    workspace ~/Documents/CPP on macOS, ~/cp on Linux
#   ./install.sh --workspace DIR    solve somewhere else
#   ./install.sh --with-prefs       also link my editor preferences, keymap and mousemap
#   ./install.sh --uninstall        remove every link this script made (your files stay)
#
# Everything is a symlink back into this repo, so `git pull` updates the whole setup.
# Anything a link would replace is moved to ~/.harly24-cp-backup/<time>/ first.

set -eu

repo=$(cd "$(dirname "$0")" && pwd -P)
os=$(uname -s)
workspace=''
with_prefs=0
uninstall=0
while [ $# -gt 0 ]; do
  case $1 in
    --workspace) workspace=$2; shift ;;
    --with-prefs) with_prefs=1 ;;
    --uninstall) uninstall=1 ;;
    -h|--help) sed -n '2,10p' "$0" | sed 's/^# \{0,1\}//'; exit 0 ;;
    *) echo "unknown option: $1" >&2; exit 2 ;;
  esac
  shift
done

if [ "$os" = Darwin ]; then
  : "${workspace:=$HOME/Documents/CPP}"
  sublime="$HOME/Library/Application Support/Sublime Text/Packages"
  keymap_name="Default (OSX)"
else
  : "${workspace:=$HOME/cp}"
  sublime="${XDG_CONFIG_HOME:-$HOME/.config}/sublime-text/Packages"
  keymap_name="Default (Linux)"
fi
backup="$HOME/.harly24-cp-backup/$(date +%Y%m%d-%H%M%S)"

bold=$'\033[1m'; dim=$'\033[2m'; green=$'\033[32m'; yellow=$'\033[33m'; reset=$'\033[0m'
say()  { printf '%s\n' "$*"; }
step() { printf '\n%s%s%s\n' "$bold" "$*" "$reset"; }
ok()   { printf '  %sok%s       %s\n' "$green" "$reset" "$*"; }
warn() { printf '  %smissing%s  %s\n' "$yellow" "$reset" "$*"; }

link() {   # link TARGET PATH: back up whatever stands at PATH, then point it at TARGET
  local target=$1 path=$2
  if [ -L "$path" ] && [ "$(readlink "$path")" = "$target" ]; then ok "$path"; return; fi
  if [ -e "$path" ] || [ -L "$path" ]; then
    mkdir -p "$backup$(dirname "$path")"
    mv "$path" "$backup$path"
    say "  ${dim}backup${reset}   $path"
  fi
  mkdir -p "$(dirname "$path")"
  ln -s "$target" "$path"
  say "  ${green}link${reset}     $path ${dim}→ ${target#$repo/}${reset}"
}

unlink_ours() {   # remove PATH only if it is a link into this repo
  local path=$1
  if [ -L "$path" ] && [[ $(readlink "$path") == "$repo"* ]]; then rm "$path"; say "  removed  $path"; fi
}

prefs=("Preferences.sublime-settings:Preferences.sublime-settings"
       "Default.sublime-keymap:$keymap_name.sublime-keymap"
       "Default.sublime-mousemap:$keymap_name.sublime-mousemap")
headers=(debug.h gen.h AllHeaders.h)

if [ "$uninstall" = 1 ]; then
  step "Removing harly24-cp links"
  unlink_ours "$sublime/harly24-cp"
  for p in "${prefs[@]}"; do unlink_ours "$sublime/User/${p#*:}"; done
  unlink_ours "$HOME/.local/bin/cprun"
  for h in "${headers[@]}"; do unlink_ours "$workspace/$h"; done
  say ""; say "Done. Workspace files and backups in ~/.harly24-cp-backup are untouched."
  exit 0
fi

step "Toolchain"
gcc=''
for c in /opt/homebrew/bin/g++-{16,15,14,13} /usr/local/bin/g++-{16,15,14,13} g++-{16,15,14,13} g++; do
  p=$(command -v "$c" 2>/dev/null) || continue
  if "$p" --version 2>/dev/null | grep -q 'Free Software Foundation'; then gcc=$p; break; fi
done
if [ -n "$gcc" ]; then ok "GCC $("$gcc" -dumpversion)  ${dim}$gcc${reset}"
elif [ "$os" = Darwin ]; then warn "GCC: brew install gcc   (plain g++ on macOS is Apple clang)"
else warn "GCC: sudo apt install g++"; fi
if command -v perl >/dev/null && perl -MTime::HiRes -e1 2>/dev/null; then ok "perl  ${dim}runs the time and memory guards${reset}"
else warn "perl with Time::HiRes: sudo apt install perl"; fi
if [ "$os" = Darwin ]; then
  if [ -x /usr/bin/lldb ]; then ok "lldb  ${dim}names the crash line${reset}"; else warn "lldb: xcode-select --install"; fi
  if command -v clang++ >/dev/null; then ok "clang++  ${dim}Sanitize variant${reset}"; else warn "clang++: xcode-select --install"; fi
else
  if command -v gdb >/dev/null; then ok "gdb  ${dim}names the crash line${reset}"; else warn "gdb: sudo apt install gdb   (optional, for the crash line)"; fi
fi

step "Command line"
link "$repo/bin/cprun" "$HOME/.local/bin/cprun"
case ":$PATH:" in *":$HOME/.local/bin:"*) ;; *) say "  ${yellow}note${reset}     add ~/.local/bin to PATH to type cprun anywhere";; esac

step "Sublime Text"
if [ -d "$sublime" ]; then
  link "$repo/sublime" "$sublime/harly24-cp"
  if [ "$with_prefs" = 1 ]; then
    for p in "${prefs[@]}"; do link "$repo/preferences/${p%%:*}" "$sublime/User/${p#*:}"; done
  fi
else
  say "  ${dim}skipped: no Sublime Text 4 found at $sublime${reset}"
fi

step "Workspace  $workspace"
mkdir -p "$workspace"
for h in "${headers[@]}"; do link "$repo/include/$h" "$workspace/$h"; done
for f in demo.cpp gen.cpp checklist.cpp; do
  if [ -e "$workspace/$f" ]; then ok "$workspace/$f ${dim}(kept yours)${reset}"
  else cp "$repo/workspace/$f" "$workspace/$f"; say "  ${green}copy${reset}     $workspace/$f"; fi
done
[ -f "$workspace/input.txt" ] || : > "$workspace/input.txt"

step "Next"
if [ "$os" = Darwin ]; then key=Cmd; else key=Ctrl; fi
say "  1. Restart Sublime Text, then Tools > Build System > CP"
say "  2. Open $workspace/demo.cpp and press $key+B"
say "  3. New file: type CPP and press Tab for the contest template"
