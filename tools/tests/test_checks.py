"""Tests for the repository check scripts.

Run from the repository root:

    python3 -m unittest discover -s tools/tests -v
"""

import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

import check_editor_leftovers as leftovers  # noqa: E402
import check_links as links  # noqa: E402


class SlugifyTests(unittest.TestCase):
    def test_plain_heading(self):
        self.assertEqual(links.slugify("Getting set up"), "getting-set-up")

    def test_punctuation_is_dropped(self):
        self.assertEqual(links.slugify("What we are building?"), "what-we-are-building")
        self.assertEqual(links.slugify("Old board vs. new board"), "old-board-vs-new-board")

    def test_colon_and_slash(self):
        self.assertEqual(
            links.slugify("0001: Shared-fret ghosting"), "0001-shared-fret-ghosting"
        )
        self.assertEqual(links.slugify("hardware/sim"), "hardwaresim")

    def test_code_span_keeps_its_text(self):
        self.assertEqual(links.slugify("Rules for `main`"), "rules-for-main")

    def test_hyphens_and_underscores_survive(self):
        self.assertEqual(links.slugify("pluck_detect and note-map"), "pluck_detect-and-note-map")


class LinkCheckTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name).resolve()
        (self.root / "docs").mkdir()
        (self.root / "docs" / "target.md").write_text(
            "# Target\n\n## A section\n\n## A section\n", encoding="utf-8"
        )

    def tearDown(self):
        self.tmp.cleanup()

    def check(self, body: str) -> list[str]:
        page = self.root / "page.md"
        page.write_text(body, encoding="utf-8")
        return links.check_file(page, self.root, {})

    def test_good_links_pass(self):
        body = (
            "[file](docs/target.md) [folder](docs/) "
            "[anchor](docs/target.md#a-section) [second](docs/target.md#a-section-1) "
            "[web](https://example.com/nothing) [mail](mailto:a@example.com)\n"
        )
        self.assertEqual(self.check(body), [])

    def test_missing_file_is_reported(self):
        problems = self.check("[gone](docs/missing.md)\n")
        self.assertEqual(len(problems), 1)
        self.assertIn("does not exist", problems[0])

    def test_missing_anchor_is_reported(self):
        problems = self.check("[bad](docs/target.md#no-such-heading)\n")
        self.assertEqual(len(problems), 1)
        self.assertIn("no heading", problems[0])

    def test_same_file_anchor(self):
        self.assertEqual(self.check("# Top\n\n[up](#top)\n"), [])
        self.assertEqual(len(self.check("# Top\n\n[up](#bottom)\n")), 1)

    def test_links_inside_code_are_ignored(self):
        body = "```text\n[gone](docs/missing.md)\n```\n\nAnd `[gone](docs/missing.md)` inline.\n"
        self.assertEqual(self.check(body), [])


class LeftoverTests(unittest.TestCase):
    def test_design_files_are_allowed(self):
        for path in [
            "hardware/controller-board/controller-board.kicad_sch",
            "hardware/controller-board/controller-board.kicad_pcb",
            "hardware/controller-board/controller-board.kicad_pro",
            "firmware/bsp/.project",
            "firmware/bsp/.cproject",
            "firmware/bsp/tigerbass2.ioc",
            "hardware/sim/front-end/front-end.asc",
            "docs/legacy/exports/ControllerPCB.net",
            "README.md",
        ]:
            self.assertIsNone(leftovers.classify(path), path)

    def test_leftovers_are_caught(self):
        for path in [
            "hardware/controller-board/controller-board-backups/2026-10-10.zip",
            "hardware/controller-board/#auto_saved_files#",
            "hardware/controller-board/_autosave-controller-board.kicad_sch",
            "hardware/controller-board/~controller-board.kicad_sch.lck",
            "hardware/controller-board/controller-board.kicad_prl",
            "hardware/controller-board/fp-info-cache",
            "hardware/controller-board/controller-board.kicad_sch-bak",
            "firmware/bsp/Debug/tigerbass2.elf",
            "firmware/bsp/.settings/language.settings.xml",
            "hardware/sim/front-end/front-end.raw",
            "mechanical/.DS_Store",
            "tools/__pycache__/check_links.cpython-312.pyc",
            "legacy/tigerbass1/ControllerPCB/~ControllerPCB.kicad_sch.lck",
        ]:
            self.assertIsNotNone(leftovers.classify(path), path)


class FixModeTests(unittest.TestCase):
    """--fix untracks leftovers but leaves them on disk and leaves design files alone."""

    def setUp(self):
        import os
        import subprocess
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        self.cwd = os.getcwd()
        run = lambda *a: subprocess.run(a, cwd=self.root, check=True, capture_output=True)
        run("git", "init", "-q")
        run("git", "config", "user.email", "test@example.com")
        run("git", "config", "user.name", "test")
        board = self.root / "hardware" / "controller-board"
        (board / "controller-board-backups").mkdir(parents=True)
        (board / "controller-board.kicad_sch").write_text("(kicad_sch)")
        (board / "controller-board.kicad_prl").write_text("{}")
        (board / "controller-board-backups" / "b.zip").write_bytes(b"zip")
        run("git", "add", "-f", ".")
        run("git", "commit", "-q", "-m", "init")
        os.chdir(self.root)

    def tearDown(self):
        import os
        os.chdir(self.cwd)
        self.tmp.cleanup()

    def tracked(self):
        return set(leftovers.tracked_files())

    def test_report_mode_fails_and_changes_nothing(self):
        before = self.tracked()
        self.assertEqual(leftovers.main([]), 1)
        self.assertEqual(self.tracked(), before)

    def test_fix_untracks_leftovers_only(self):
        self.assertEqual(leftovers.main(["--fix"]), 0)
        self.assertEqual(self.tracked(), {"hardware/controller-board/controller-board.kicad_sch"})
        # Still on disk.
        self.assertTrue((self.root / "hardware/controller-board/controller-board.kicad_prl").exists())
        self.assertTrue((self.root / "hardware/controller-board/controller-board-backups/b.zip").exists())
        # A second run finds nothing.
        self.assertEqual(leftovers.main([]), 0)


if __name__ == "__main__":
    unittest.main()
