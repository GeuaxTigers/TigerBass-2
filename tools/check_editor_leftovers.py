#!/usr/bin/env python3
"""Fail if editor leftovers or build output have been committed.

KiCad, STM32CubeIDE and LTspice write backups, autosaves, lock files and
build products next to the design files. They are not design data. In
Git they cause conflicts between teammates and bury the real change.

.gitignore stops most of them being added. This check catches the ones
that get through, for example when someone runs "git add -f" or copies a
whole project folder in from elsewhere.

Run from anywhere inside the repository:

    python3 tools/check_editor_leftovers.py          # report, exit 1 if found
    python3 tools/check_editor_leftovers.py --fix    # also remove them from Git

--fix runs "git rm --cached" on each leftover, so it is no longer tracked
but stays on disk. Review and commit the result. The cleanup workflow
(.github/workflows/clean-editor-leftovers.yml) runs it on every push to a
branch other than main.

Exit status is 0 when nothing is found or --fix removed everything,
1 otherwise.
Standard library only.
"""

from __future__ import annotations

import argparse
import fnmatch
import subprocess
import sys

# (pattern, what it is). A pattern containing "/" is matched against the
# whole path; one without is matched against the file name only.
# A trailing "/" means "any file inside a folder with this name".
LEFTOVERS = [
    ("*-backups/", "KiCad backup folder"),
    ("#auto_saved_files#", "KiCad autosave marker"),
    ("_autosave-*", "KiCad autosave file"),
    ("*.lck", "KiCad lock file: someone had the file open when they committed"),
    ("~*", "editor temporary or lock file"),
    ("*.kicad_prl", "KiCad per-user window and layer settings"),
    ("fp-info-cache", "KiCad footprint cache"),
    ("*-bak", "backup file"),
    ("*.bak", "backup file"),
    ("Debug/", "STM32CubeIDE build output"),
    ("Release/", "STM32CubeIDE build output"),
    (".settings/", "STM32CubeIDE per-user settings"),
    ("*.raw", "LTspice simulation output"),
    ("*.elf", "firmware build output"),
    ("*.o", "compiler object file"),
    (".DS_Store", "macOS folder metadata"),
    ("Thumbs.db", "Windows thumbnail cache"),
    ("__pycache__/", "Python cache"),
]


def classify(path: str) -> str | None:
    """Return a description if the path is a leftover, otherwise None."""
    parts = path.split("/")
    name = parts[-1]
    folders = parts[:-1]
    for pattern, description in LEFTOVERS:
        if pattern.endswith("/"):
            folder_pattern = pattern[:-1]
            if any(fnmatch.fnmatchcase(folder, folder_pattern) for folder in folders):
                return description
        elif fnmatch.fnmatchcase(name, pattern):
            return description
    return None


def tracked_files() -> list[str]:
    out = subprocess.run(
        ["git", "ls-files", "-z"],
        check=True, capture_output=True,
    )
    return [p for p in out.stdout.decode("utf-8", "surrogateescape").split("\0") if p]


def untrack(paths: list[str]) -> None:
    """Stop tracking the given paths in Git, leaving the files on disk."""
    for start in range(0, len(paths), 100):
        subprocess.run(
            ["git", "rm", "--cached", "--quiet", "--"] + paths[start:start + 100],
            check=True,
        )


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument(
        "--fix", action="store_true",
        help="remove each leftover from Git (git rm --cached), keeping it on disk",
    )
    args = parser.parse_args(argv)

    found = []
    files = tracked_files()
    for path in files:
        description = classify(path)
        if description:
            found.append((path, description))
    if found and args.fix:
        untrack([path for path, _ in found])
        print(f"Removed {len(found)} file(s) from Git (still on disk):\n")
        for path, description in found:
            print(f"  {path}\n      {description}")
        return 0
    if found:
        print(f"{len(found)} file(s) that should not be in Git:\n")
        for path, description in found:
            print(f"  {path}\n      {description}")
        print(
            "\nRemove each one from Git but keep it on disk with:\n"
            "    git rm --cached <path>\n"
            "then commit. If .gitignore did not stop it being added, fix .gitignore too."
        )
        return 1
    print(f"No editor leftovers or build output committed ({len(files)} files checked).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
