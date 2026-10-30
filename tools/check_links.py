#!/usr/bin/env python3
"""Check that relative links in the repository's Markdown files resolve.

For every Markdown file tracked by Git, find each link of the form
[text](target) and check that:

  * a relative target points at a file or folder that exists, and
  * a "#heading" fragment matches a heading in the Markdown file it
    points at.

Links to websites (http, https, mailto) are not checked: they need a
network, and a site being down should not fail a pull request.

Run from anywhere inside the repository:

    python3 tools/check_links.py

Exit status is 0 when every link resolves, 1 otherwise.
Standard library only.
"""

from __future__ import annotations

import re
import subprocess
import sys
import unicodedata
from pathlib import Path

# Folders whose Markdown we do not own and should not police.
SKIP_PREFIXES = ("legacy/tigerbass1/", "firmware/tests/vendor/")

EXTERNAL = re.compile(r"^[a-zA-Z][a-zA-Z0-9+.-]*:")
INLINE_LINK = re.compile(r"(?<!\!)\[(?:[^\]\\]|\\.)*\]\(\s*<?([^)\s>]+)>?(?:\s+\"[^\"]*\")?\s*\)")
FENCE = re.compile(r"^\s*(```|~~~)")
INLINE_CODE = re.compile(r"`[^`\n]*`")
HEADING = re.compile(r"^\s{0,3}(#{1,6})\s+(.*?)\s*#*\s*$")


def repo_root() -> Path:
    out = subprocess.run(
        ["git", "rev-parse", "--show-toplevel"],
        check=True, capture_output=True, text=True,
    )
    return Path(out.stdout.strip())


def tracked_markdown(root: Path) -> list[Path]:
    out = subprocess.run(
        ["git", "ls-files", "--cached", "--others", "--exclude-standard", "*.md"],
        check=True, capture_output=True, text=True, cwd=root,
    )
    files = []
    for line in out.stdout.splitlines():
        if line.startswith(SKIP_PREFIXES):
            continue
        path = root / line
        if path.is_file():
            files.append(path)
    return sorted(files)


def strip_code(text: str) -> list[tuple[int, str]]:
    """Return (line number, line) pairs with code blocks and code spans blanked."""
    lines = []
    in_fence = False
    for number, line in enumerate(text.splitlines(), start=1):
        if FENCE.match(line):
            in_fence = not in_fence
            lines.append((number, ""))
            continue
        if in_fence:
            lines.append((number, ""))
            continue
        lines.append((number, INLINE_CODE.sub("", line)))
    return lines


def slugify(heading: str) -> str:
    """Turn a heading into the anchor GitHub generates for it."""
    # Drop Markdown that does not appear in the rendered heading text.
    text = re.sub(r"`([^`]*)`", r"\1", heading)
    text = re.sub(r"!\[([^\]]*)\]\([^)]*\)", r"\1", text)
    text = re.sub(r"\[([^\]]*)\]\([^)]*\)", r"\1", text)
    text = re.sub(r"[*_]{1,3}([^*_]+)[*_]{1,3}", r"\1", text)
    text = text.strip().lower()
    out = []
    for char in text:
        category = unicodedata.category(char)
        if char in "-_":
            out.append(char)
        elif char == " ":
            out.append("-")
        elif category[0] in ("L", "N") or category == "Mn":
            out.append(char)
        # Everything else (punctuation, symbols) is dropped.
    return "".join(out)


def anchors_in(path: Path) -> set[str]:
    """Every heading anchor in a Markdown file, numbering duplicates as GitHub does."""
    seen: dict[str, int] = {}
    anchors = set()
    for _, line in strip_fences_only(path.read_text(encoding="utf-8")):
        match = HEADING.match(line)
        if not match:
            continue
        slug = slugify(match.group(2))
        count = seen.get(slug, 0)
        anchors.add(slug if count == 0 else f"{slug}-{count}")
        seen[slug] = count + 1
    return anchors


def strip_fences_only(text: str) -> list[tuple[int, str]]:
    """Like strip_code, but keeps code spans so headings with `code` slug correctly."""
    lines = []
    in_fence = False
    for number, line in enumerate(text.splitlines(), start=1):
        if FENCE.match(line):
            in_fence = not in_fence
            lines.append((number, ""))
            continue
        lines.append((number, "" if in_fence else line))
    return lines


def check_file(path: Path, root: Path, anchor_cache: dict[Path, set[str]]) -> list[str]:
    problems = []
    rel = path.relative_to(root)
    for number, line in strip_code(path.read_text(encoding="utf-8")):
        for match in INLINE_LINK.finditer(line):
            target = match.group(1)
            if EXTERNAL.match(target):
                continue
            file_part, _, fragment = target.partition("#")
            if file_part:
                if file_part.startswith("/"):
                    dest = root / file_part.lstrip("/")
                else:
                    dest = path.parent / file_part
                dest = dest.resolve()
                if not dest.exists():
                    problems.append(f"{rel}:{number}: '{target}' does not exist")
                    continue
            else:
                dest = path
            if not fragment:
                continue
            if dest.is_dir() or dest.suffix.lower() != ".md":
                continue
            if dest not in anchor_cache:
                anchor_cache[dest] = anchors_in(dest)
            if fragment.lower() not in anchor_cache[dest]:
                problems.append(
                    f"{rel}:{number}: '{target}' has no heading '#{fragment}' in "
                    f"{dest.relative_to(root)}"
                )
    return problems


def main() -> int:
    root = repo_root()
    files = tracked_markdown(root)
    cache: dict[Path, set[str]] = {}
    problems = []
    for path in files:
        problems.extend(check_file(path, root, cache))
    if problems:
        print(f"{len(problems)} broken link(s) in {len(files)} Markdown file(s):\n")
        for problem in problems:
            print("  " + problem)
        print("\nFix the link, or the file or heading it points at.")
        return 1
    print(f"All relative links resolve ({len(files)} Markdown files checked).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
