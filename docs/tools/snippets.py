"""Write docs/snippets.md from the snippet files: python3 docs/tools/snippets.py"""

import html
import re
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
TITLES = {"templates": "Templates", "math": "Math and number theory", "graphs": "Graphs and trees",
          "data-structures": "Data structures", "arrays-strings": "Arrays and strings", "bits": "Bits",
          "utility": "Utility"}

out = ["# Snippets", "",
       "Type the trigger in a C++ file and press Tab. Every snippet lives in `sublime/snippets/<group>/`;",
       "drop a new `.sublime-snippet` file there and Sublime picks it up at once.", ""]
for group, title in TITLES.items():
    files = sorted((REPO / "sublime/snippets" / group).glob("*.sublime-snippet"))
    if not files:
        continue
    out += [f"## {title}", "", "| Trigger | What it gives you | File |", "| --- | --- | --- |"]
    for f in files:
        s = f.read_text()
        trigger = re.search(r"<tabTrigger>(.*?)</tabTrigger>", s).group(1)
        desc = html.unescape(re.search(r"<description>(.*?)</description>", s).group(1))
        out.append(f"| `{trigger}` | {desc} | [{f.stem}](../sublime/snippets/{group}/{f.name}) |")
    out.append("")
(REPO / "docs/snippets.md").write_text("\n".join(out))
print("docs/snippets.md")
