#!/usr/bin/env bash
# One-line install for harly24-cp:
#
#   curl -fsSL https://raw.githubusercontent.com/Abhayanthk/harly24-cp/main/get.sh | bash
#   curl -fsSL https://raw.githubusercontent.com/Abhayanthk/harly24-cp/main/get.sh | bash -s -- --with-prefs
#
# Downloads the latest release into ~/.harly24-cp and runs its install.sh with your options.
# Run it again to update. To remove: ~/.harly24-cp/install.sh --uninstall && rm -rf ~/.harly24-cp

set -euo pipefail

main() {   # wrapped so nothing runs until the whole script has downloaded
  local dest=${HARLY24_CP_HOME:-$HOME/.harly24-cp}
  local url=${HARLY24_CP_URL:-https://github.com/Abhayanthk/harly24-cp/releases/latest/download/harly24-cp.tar.gz}

  for tool in curl tar; do
    command -v "$tool" >/dev/null || { echo "harly24-cp: needs $tool" >&2; exit 1; }
  done
  if [ -e "$dest" ] && [ ! -f "$dest/bin/cprun" ]; then
    echo "harly24-cp: $dest exists and isn't a harly24-cp install; set HARLY24_CP_HOME to another folder" >&2
    exit 1
  fi
  if [ -d "$dest/.git" ]; then
    echo "harly24-cp: $dest is a git clone; update it with git pull" >&2
    exit 1
  fi

  local tmp
  tmp=$(mktemp -d)
  trap 'rm -rf "$tmp"' EXIT
  echo "Downloading harly24-cp..."
  curl -fsSL "$url" -o "$tmp/harly24-cp.tar.gz"
  tar -xzf "$tmp/harly24-cp.tar.gz" -C "$tmp"

  # Swap the new version in at the same path, so every existing link keeps working.
  rm -rf "$dest.new"
  mv "$tmp/harly24-cp" "$dest.new"
  rm -rf "$dest"
  mv "$dest.new" "$dest"
  echo "Installed into $dest"

  "$dest/install.sh" ${@+"$@"}
}

main ${@+"$@"}
