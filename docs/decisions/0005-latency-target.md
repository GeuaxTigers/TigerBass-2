# 0005: Latency target

| | |
| --- | --- |
| Status | Open |
| Owner | `TBD(team)` |
| Needed by | Before the ADC and microcontroller are chosen |
| Requirements | M2, TB2-01 |
| Analysis | Planned: `docs/analysis/latency-budget.md` (Prompt 6) |

## The question

What latency are we designing to, measured between which two points, and how will we measure it?

## Why it matters

"Lower latency than Tiger Bass 1" is a project goal, and it cannot be designed for or tested until it is a number with two defined endpoints. The number sets how fast the ADC must be, how the scan is organised, and whether the firmware can afford to wait for a pluck's peak before sending the note.

## What we know

- Last year's target was 8 ms from note detected to MIDI packet sent.
- Last year's analysis predicted a worst case of 4.84 ms.
- That analysis assumed high-speed USB. The board uses full-speed USB, where transfers are scheduled in 1 ms frames, so its USB figure is too optimistic.
- A recorded waveform showed the pickup signal reaching its first peak about 2.5 ms after the pluck on the lowest note. That part is physics: the firmware cannot measure a peak before it happens.
- We have no measured end-to-end latency for the finished Tiger Bass 1 in our notes.

Sources are in [`../legacy/tigerbass1-notes.md`](../legacy/tigerbass1-notes.md).

## Decide the endpoints first

| Start | End | What it includes |
| --- | --- | --- |
| The pick or finger releases the string | The MIDI message leaves the instrument | Everything we control, plus the pickup rise time |
| The pickup signal crosses the threshold | The MIDI message leaves the instrument | Only electronics and firmware. Closest to last year's definition |
| The pick or finger releases the string | Sound comes out of the computer | What the player feels. Includes the computer's audio buffering, which we do not control |

A target is only useful with the row it refers to.

## Options for the number

### A. Keep 8 ms

Known to be achievable on paper. Does not by itself show an improvement.

### B. Beat a measured Tiger Bass 1 figure by a set margin

Measure last year's instrument, then commit to a percentage or a number of milliseconds below it. The most direct proof that the goal was met. Needs last year's instrument to still work.

### C. Set it from a budget

Add up the stages we cannot shorten and the best we can do on the rest, and commit to that total plus a margin. The most defensible engineering answer. The result is whatever it is, and may or may not sound impressive.

### D. Set it from what players notice

Find published figures for the delay at which musicians start to notice latency, with a citable source, and aim below it. Ties the number to the user. Depends on finding a source the judges will accept.

## Also decide: send at the threshold, or at the peak?

Sending Note On when the signal first crosses the threshold is fastest and has to guess the velocity. Waiting for the first peak gives a real velocity and adds up to the rise time to every note. A middle path is to wait a fixed short window. This is a firmware choice, but it changes the achievable number by milliseconds, so it belongs with the target.

## How to choose

1. The target can be measured with equipment we have.
2. It shows a real improvement over Tiger Bass 1.
3. It is achievable with margin, according to the budget.
4. The sponsor agrees it is the right thing to measure.

## What we need to find out first

1. **Measure Tiger Bass 1**, if it still works: scope on the pickup channel and on a signal that marks the MIDI packet leaving, 20 or more plucks per string, record the worst case as well as the average.
2. **Build the latency budget** (Prompt 6) for last year's design and for this year's with unknowns left as ranges.
3. **Ask the sponsor** which interval matters most.
4. **Decide how "MIDI sent" is observed.** A USB packet is hard to see on a scope. Options: toggle a spare GPIO when the packet is queued, watch the DIN MIDI line, or timestamp at the host.

## Decision

_Not decided._

## Consequences

_Fill in when decided._
