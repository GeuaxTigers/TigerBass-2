# docs/test

How we show the instrument meets its requirements: test plans, step-by-step procedures, and the recorded results of each run.

**Status: not started.** Writing the test plan and the bring-up checklist is Prompt 9 in [`docs/copilot/kickoff-prompt.md`](../copilot/kickoff-prompt.md).

## Planned contents

| File | Contents |
| --- | --- |
| `test-plan.md` | One test per requirement in [`docs/requirements.md`](../requirements.md) that is verified by test or demonstration |
| `bring-up-checklist.md` | What to check, in order, the first time a new board is powered |
| `results/` | One file per test run |

## Recording a result

Every run records the same things, so two runs can be compared months apart:

- Date and who ran it.
- Board revision and firmware commit (`git rev-parse --short HEAD`).
- Equipment, with model numbers. For an oscilloscope: probe type, bandwidth setting and timebase.
- Where the raw data is: scope captures, logs, spreadsheets.
- The result against the pass criterion, as a number.
- Anything unexpected, even if the test passed.

A result of "it worked" cannot be used as evidence. A result of "4.1 ms pluck to first USB packet, 20 plucks, worst case 4.6 ms, revA board, commit `3f2a91c`" can.
