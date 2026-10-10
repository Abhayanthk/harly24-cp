"""Render the README images from real runs.

    python3 docs/tools/render.py

Runs workspace/demo.cpp and a generator through bin/cprun, captures what they print, and draws
each capture as a terminal-style SVG in docs/images/. Nothing in the images is typed by hand.
"""

import html
import os
import re
import shutil
import subprocess
import tempfile
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
IMAGES = REPO / "docs/images"
CPRUN = REPO / "bin/cprun"

# Tokyo Night, slightly muted
BG, BORDER, BAR_LINE = "#0f1117", "#262b38", "#1b1f2b"
COLORS = {
    "text": "#c9d1e1", "dim": "#5f6880", "white": "#eef1f8", "cyan": "#7dcfff", "green": "#9ece6a",
    "red": "#f7768e", "amber": "#e0af68", "purple": "#bb9af7", "orange": "#ff9e64",
}
FONT = "'JetBrains Mono','SF Mono',Menlo,Consolas,'DejaVu Sans Mono',monospace"
SIZE, LINE, CHAR = 13.5, 22, 8.2


# ------------------------------------------------------------------ capture

def run(mode, source, workdir, env=None):
    result = subprocess.run([str(CPRUN), mode, source], cwd=workdir, capture_output=True, text=True,
                            env={**os.environ, **(env or {})})
    return (result.stdout + result.stderr).rstrip("\n").split("\n")


def demo(number, workdir, env=None):
    text = (REPO / "workspace/demo.cpp").read_text()
    (workdir / "demo.cpp").write_text(re.sub(r"^#define DEMO \d+", f"#define DEMO {number}", text, flags=re.M))
    (workdir / "input.txt").write_text("")
    return run("debug", "demo.cpp", workdir, env)


def summary(lines):
    """The verdict block, plus the last debug lines when a run was stopped."""
    if "last debug lines:" in lines:
        lines = lines[lines.index("last debug lines:"):]
    else:
        lines = lines[next(i for i, l in enumerate(lines) if l.startswith("-----")):]
    return [l for l in lines if not l.startswith(("[debug] flood", "-----"))]


def wrap(lines, width):
    """Wrap long lines the way a narrow build panel would, keeping the bullet indent."""
    out = []
    for line in lines:
        indent = " " * (len(line) - len(line.lstrip(" •→")) )
        while len(line) > width:
            cut = line.rfind(" ", 0, width)
            out.append(line[:cut])
            line = indent + line[cut + 1:]
        out.append(line)
    return out


# ------------------------------------------------------------------ colour

def spans_debug(line):
    m = re.match(r"^(\[\w+:\d+\] )(.*)$", line)
    if not m:
        m2 = re.match(r"^(\s+\d+: )(.*)$", line)
        if m2:
            return [(m2.group(1), "dim")] + values(m2.group(2))
        return [(line, "text")]
    out = [(m.group(1), "dim")]
    body = m.group(2)
    parts = body.split(" | ")
    for i, part in enumerate(parts):
        if i:
            out.append((" | ", "dim"))
        eq = re.match(r"^(.*?)( =)( ?)(.*)$", part)
        if eq and not part.startswith('"'):
            out += [(eq.group(1), "cyan"), (eq.group(2) + eq.group(3), "dim")] + values(eq.group(4))
        else:
            out.append((part, "amber" if part in ("reached",) else "white"))
    return out


def values(text):
    out = []
    for token in re.split(r'("[^"]*"|\binf\b|[\[\]{}(),:]|\.\.\.\+\d+ more)', text):
        if not token:
            continue
        if token.startswith('"'):
            out.append((token, "green"))
        elif token == "inf":
            out.append((token, "orange"))
        elif token in "[]{}(),:" or token.startswith("..."):
            out.append((token, "dim"))
        else:
            out.append((token, "text"))
    return out


def spans_verdict(line):
    if line.startswith("-----"):
        return [(line, "dim")]
    m = re.match(r"^(\[\w+\]) (\S+)(.*)$", line)
    if m and not line.startswith("[debug] flood"):
        return [(m.group(1) + " ", "purple"), (m.group(2), "white"), (m.group(3), "dim")]
    if line.startswith("✓"):
        return [(line, "green")]
    if line.startswith("✗"):
        return [(line, "red")]
    m = re.match(r"^(\s+• )(.*)$", line)
    if m:
        return [(m.group(1), "dim"), (m.group(2), "text")]
    m = re.match(r"^(\s+→ )(.*)$", line)
    if m:
        return [(m.group(1), "amber"), (m.group(2), "amber")]
    if line == "last debug lines:":
        return [(line, "dim")]
    if line.startswith("["):
        return spans_debug(line)
    return [(line, "dim")]


# ------------------------------------------------------------------ draw

def text_rows(rows, x, y):
    out = []
    for i, spans in enumerate(rows):
        tspans = "".join(f'<tspan fill="{COLORS[c]}">{html.escape(t)}</tspan>' for t, c in spans)
        out.append(f'<text x="{x}" y="{y + i * LINE}" xml:space="preserve">{tspans}</text>')
    return "\n".join(out)


def window(x, y, w, h, title):
    dots = "".join(f'<circle cx="{x + 20 + i * 18}" cy="{y + 18}" r="5.5" fill="{c}"/>'
                   for i, c in enumerate(("#ff5f57", "#febc2e", "#28c840")))
    return (f'<rect x="{x + .5}" y="{y + .5}" width="{w - 1}" height="{h - 1}" rx="10" fill="{BG}" stroke="{BORDER}"/>'
            f'{dots}<line x1="{x}" y1="{y + 36}" x2="{x + w}" y2="{y + 36}" stroke="{BAR_LINE}"/>'
            f'<text x="{x + w / 2}" y="{y + 22}" fill="{COLORS["dim"]}" font-size="12" text-anchor="middle">{html.escape(title)}</text>')


def svg(width, height, body):
    return (f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}" '
            f'font-family="{FONT}" font-size="{SIZE}">\n{body}\n</svg>\n')


def panel(name, title, lines, colour, min_width=760):
    rows = [colour(l) for l in lines]
    width = max(min_width, int(max(len(l) for l in lines) * CHAR) + 52)
    height = 36 + 22 + len(rows) * LINE + 6
    body = window(0, 0, width, height, title) + text_rows(rows, 26, 36 + 30)
    (IMAGES / f"{name}.svg").write_text(svg(width, height, body))
    print(f"  docs/images/{name}.svg")


def grid(name, cells, cols=2, cell_width=620, gap=16):
    max_chars = int((cell_width - 44) / CHAR)
    cells = [(title, wrap(lines, max_chars)) for title, lines in cells]
    heights = [36 + 22 + len(lines) * LINE + 6 for _, lines in cells]
    row_h = [max(heights[r * cols:(r + 1) * cols]) for r in range((len(cells) + cols - 1) // cols)]
    width = cols * cell_width + (cols - 1) * gap
    height = sum(row_h) + gap * (len(row_h) - 1)
    body = []
    for i, (title, lines) in enumerate(cells):
        r, c = divmod(i, cols)
        x, y = c * (cell_width + gap), sum(row_h[:r]) + gap * r
        body.append(window(x, y, cell_width, row_h[r], title))
        body.append(text_rows([spans_verdict(l) for l in lines], x + 22, y + 36 + 30))
    (IMAGES / f"{name}.svg").write_text(svg(width, height, "\n".join(body)))
    print(f"  docs/images/{name}.svg")


def banner():
    h = 230
    chips = [("debug.h", "prints anything, survives crashes"), ("cprun", "a verdict for every run"),
             ("gen.h", "20 input formats, one call")]
    widths = [int((len(label) + len(text) + 2) * 8.0) + 28 for label, text in chips]
    w = 40 + sum(widths) + 14 * (len(chips) - 1) + 40
    parts = [f'<defs><linearGradient id="g" x1="0" x2="1"><stop offset="0" stop-color="{COLORS["purple"]}"/>'
             f'<stop offset="1" stop-color="{COLORS["cyan"]}"/></linearGradient></defs>',
             f'<rect x=".5" y=".5" width="{w - 1}" height="{h - 1}" rx="14" fill="{BG}" stroke="{BORDER}"/>',
             f'<rect x="40" y="0" width="120" height="3" fill="url(#g)"/>',
             f'<text x="40" y="86" font-size="46" font-weight="700" fill="{COLORS["white"]}">harly24<tspan fill="url(#g)">-cp</tspan></text>',
             f'<text x="42" y="122" font-size="16" fill="{COLORS["dim"]}">A Sublime Text workbench for competitive C++</text>']
    x = 40
    for (label, text), cw in zip(chips, widths):
        parts.append(f'<rect x="{x}" y="156" width="{cw}" height="36" rx="8" fill="#151925" stroke="{BORDER}"/>')
        parts.append(f'<text x="{x + 14}" y="179" font-size="13" xml:space="preserve"><tspan fill="{COLORS["cyan"]}">{label}</tspan>'
                     f'<tspan fill="{COLORS["text"]}">  {html.escape(text)}</tspan></text>')
        x += cw + 14
    (IMAGES / "banner.svg").write_text(svg(w, h, "\n".join(parts)))
    print("  docs/images/banner.svg")


# ------------------------------------------------------------------ main

def main():
    IMAGES.mkdir(parents=True, exist_ok=True)
    work = Path(tempfile.mkdtemp())
    try:
        print("rendering")
        banner()

        demo(0, work)
        panel("debug-tour", "debug.txt", (work / "debug.txt").read_text().rstrip("\n").split("\n"), spans_debug)

        panel("crash-verdict", "build panel", summary(demo(1, work)), spans_verdict)

        cells = [("endless loop", summary(demo(5, work))),
                 ("debug() inside an endless loop", summary(demo(6, work))),
                 ("push_back forever", summary(demo(7, work))),
                 ("DFS 1,000,000 deep", summary(demo(8, work)))]
        grid("guards", cells, cols=1, cell_width=760)

        (work / "input.txt").write_text("")
        (work / "gen.cpp").write_text('#include <bits/stdc++.h>\n#include "gen.h"\n'
                                      'int main() {\n    seed(42);\n    arrays(10, 200000, 1, 1e9);\n}\n')
        out = [l.replace("seed 42 (fixed)", "seed 42") for l in run("fast", "gen.cpp", work)]
        out = [l[:96] + (" ..." if len(l) > 96 else "") for l in out if not l.startswith(("[fast] building", "-----"))]
        panel("generator", "build panel", out, spans_verdict)
    finally:
        shutil.rmtree(work)


if __name__ == "__main__":
    main()
