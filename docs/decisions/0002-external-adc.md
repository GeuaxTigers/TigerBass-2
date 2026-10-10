# 0002: External ADC

| | |
| --- | --- |
| Status | Open |
| Owner | `TBD(team)` |
| Needed by | Before the controller schematic's analog sheet |
| Requirements | F2, F3, TB2-01, TB2-14, DC-02 |
| Analysis | Planned: `docs/analysis/adc-trade-study.md` (Prompt 5) |

## The question

Which ADC measures the four per-string pickup channels, and how does it connect to the microcontroller?

## Why it matters

The ADC sets how soon after a pluck the firmware can know the string was plucked, and how finely it can tell a soft pluck from a hard one. It is on the latency path for every note. It also sets what the analog front end has to deliver ([0003](0003-amplitude-front-end.md)) and what bus and timing the microcontroller has to provide ([0004](0004-microcontroller.md)).

Tiger Bass 1 used the microcontroller's internal 12-bit ADC, with four pickup channels and five control potentiometers sharing it.

**First, write down why we are moving the ADC off the microcontroller.** `TBD(team)`. The reason decides the criteria. If it is noise, the part needs a clean reference and layout attention. If it is speed or all four channels sampled at the same instant, it needs a particular architecture. If it is resolution, it needs more bits and a front end quiet enough to use them.

## Options

These are architectures, not part numbers. The trade study shortlists parts within whichever fits.

### A. Simultaneous-sampling multi-channel SAR

Four or more inputs captured at the same instant, then read out. Every string is measured at the same moment, so nothing is lost to channel-to-channel skew and a chord is seen as one event. Usually costs more and uses more pins than a multiplexed part.

### B. Single SAR converter with an input multiplexer

One converter stepping through the channels. Cheaper and common. The per-channel rate is the converter rate divided by the number of channels, less the settling time after each switch. The front end must be able to drive the sampling capacitor each time the multiplexer lands on it.

### C. Delta-sigma converter

High resolution and built-in filtering. The digital filter adds a fixed delay between the input changing and the output showing it. That delay is on the latency path, so it has to be checked against the budget before one of these is considered.

### D. Multi-channel audio codec

Converts continuously at audio rates over an audio serial interface. Good resolution and anti-aliasing built in. Like option C it has a filter delay, and it needs a microcontroller with the matching audio interface.

### E. Stay on the microcontroller's internal ADC

Listed so the comparison is honest. If the reason in "Why it matters" turns out to be something a better front end or better firmware would fix, the external part may not be needed.

## How to choose

1. **Time from pluck to a usable reading**, including any filter delay and the time to read the result over the bus (TB2-01).
2. **Per-channel sample rate**, enough to catch the first peak of a pluck. Last year's team recorded the first peak arriving about 2.5 ms after the pluck on the lowest note.
3. **Resolution that survives the noise**, enough for the velocity range we want (TB2-14).
4. **Input range and type** that suit the front end in [0003](0003-amplitude-front-end.md).
5. **Interface** the chosen microcontroller can run fast enough without blocking the scan.
6. **A package the team can solder by hand**, and a part that can be bought in small quantities.
7. **Supply and reference** that fit the power tree ([0009](0009-battery-and-power-tree.md)).
8. Cost, against a $1000 total budget.

Also decide whether the scroll wheel and whammy bar ([0008](0008-expression-sensing.md)) go through this ADC or the microcontroller's own.

## What we need to find out first

1. **What the pickup actually puts out.** Scope each string's channel on the real pickup: peak voltage for the softest and hardest pluck, time from pluck to first peak, and how long the signal lasts. Every criterion above depends on these numbers.
2. **How many velocity levels are worth having.** A synth takes 127. A player may be able to control far fewer. Ask the sponsor.
3. **The latency share available to the ADC**, from the budget in [0005](0005-latency-target.md).
4. **Which pickup we are using.** The report and the as-built board of Tiger Bass 1 disagree. See [`../legacy/tigerbass1-notes.md`](../legacy/tigerbass1-notes.md).

## Decision

_Not decided._

## Consequences

_Fill in when decided._
