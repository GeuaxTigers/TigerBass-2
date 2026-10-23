# docs/analysis

Trade studies and calculations: the work that sits between an open question and a decision.

**Status: started.** Written so far:

| Document | Feeds decision |
| --- | --- |
| [`controller-board-schematic-review.md`](controller-board-schematic-review.md) | [0002](../decisions/0002-external-adc.md), [0003](../decisions/0003-amplitude-front-end.md), [0009](../decisions/0009-battery-and-power-tree.md) |

Still planned, each with a prompt in [`docs/copilot/kickoff-prompt.md`](../copilot/kickoff-prompt.md):

| Document | Feeds decision |
| --- | --- |
| `shared-fret-ghosting.md` | [0001](../decisions/0001-shared-fret-ghosting.md) |
| `adc-trade-study.md` | [0002](../decisions/0002-external-adc.md), [0003](../decisions/0003-amplitude-front-end.md) |
| `latency-budget.md` | [0005](../decisions/0005-latency-target.md) |
| `mcu-pin-budget.md` | [0004](../decisions/0004-microcontroller.md) |
| `usb-midi.md` | [0006](../decisions/0006-usb.md) |

## What an analysis document contains

1. **The question**, and which decision it feeds.
2. **Assumptions**, each one stated so it can be checked.
3. **Method**: the calculation, simulation or script, with enough detail to repeat it.
4. **Results**, with units.
5. **What it does not tell us**, and which bench measurement would.

An analysis document compares options. It does not choose one. The choice is recorded in the decision file, by the team.
