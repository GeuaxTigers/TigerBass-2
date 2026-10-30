# tools

Scripts that support the design: calculations, generators and repository checks. They are Python using only the standard library, so they run anywhere without installing anything. The one exception is the shell script CI uses to drive KiCad.

## Present

| Script | What it does |
| --- | --- |
| [`check_links.py`](check_links.py) | Checks that every relative link in the repository's Markdown files points at a file that exists. CI runs it |
| [`check_editor_leftovers.py`](check_editor_leftovers.py) | Fails if a KiCad backup, autosave or lock file has been committed. CI runs it. With `--fix`, removes them from Git instead and keeps them on disk; the cleanup workflow runs it that way on every branch push |
| [`ci_kicad_checks.sh`](ci_kicad_checks.sh) | Runs KiCad's ERC and DRC on every project under `hardware/` and exports a schematic PDF and BOM. CI runs it inside the KiCad container. Needs `kicad-cli` |
| [`tests/`](tests/) | Tests for the scripts above |

Run them from the repository root:

```bash
python3 tools/check_links.py
python3 tools/check_editor_leftovers.py
python3 -m unittest discover -s tools/tests -v
npx markdownlint-cli2          # Markdown lint, needs Node.js
```

## Planned

| Script | What it will do | Prompt |
| --- | --- | --- |
| `fret_matrix_sim.py` | Model the string and fret switch matrix and report which fingerings a shared-fret scan reads wrongly | 3 |
| `adc_budget.py` | From channel count, sample rate, resolution and bus type: bus clock needed and time to read all channels | 5 |
| `latency_budget.py` | Add up the delay of each stage between a pluck and the MIDI message, best and worst case | 6 |
| `pin_budget.py` | Total the microcontroller pins the design needs, by type | 7 |
| `gen_note_map.py` | Print the string and fret to MIDI note table for a tuning, to check the firmware against | 4 |
| `bom_summary.py` | Summarise a KiCad BOM export: line count, parts missing a part number | none yet |

"Prompt" is the task number in [`docs/copilot/kickoff-prompt.md`](../docs/copilot/kickoff-prompt.md).

## Rules

- Standard library only, unless a `requirements.txt` in this folder says otherwise.
- Every script prints its assumptions along with its answer. A number with no stated inputs gets copied into a report and cannot be checked later.
- Scripts that compute something have tests in `tools/tests/`.
