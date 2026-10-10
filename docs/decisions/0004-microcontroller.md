# 0004: Microcontroller

| | |
| --- | --- |
| Status | Open |
| Owner | `TBD(team)` |
| Needed by | Before the controller schematic starts |
| Requirements | F1, F4, M3, TB2-01, TB2-11 |
| Analysis | Planned: `docs/analysis/mcu-pin-budget.md` (Prompt 7) |

## The question

Do we keep last year's STM32F439ZIT6, or move to a different microcontroller?

## Why it matters

Everything on the controller board connects to this part, and all of `firmware/bsp` is written for it. It is the hardest part to change after layout.

Last year's design needed 80 fret inputs plus everything else, and its 144-pin package provided them. The team also weighed a microcontroller against an FPGA at the concept stage. With 20 shared fret lines, the need for that many pins is gone.

## Options

### A. Keep the STM32F439ZIT6

The schematic around it is known to work: decoupling, crystal, boot switch, debug header, USB. Last year's firmware runs on it. Last year's team prototyped on a NUCLEO-F439ZI development board, which may still be available. Costs: a large package to route and solder, more board area and supply current than we may need.

### B. A smaller package in the same STM32F4 family

Keeps the firmware, the tools and most of the support circuitry. Fewer pins to route. Needs a check that a smaller package still brings out every peripheral we need on pins that do not clash.

### C. A different STM32 family

A newer family may offer USB that needs no external crystal, lower power, or a faster ADC interface. Costs: the CubeMX configuration and the BSP start again, and nothing from last year's schematic can be assumed.

### D. A microcontroller plus programmable logic

A small FPGA or CPLD doing the scan and ADC readout in parallel, with a microcontroller for USB and MIDI. Could cut scan time. Adds a second toolchain, a second device to program and a larger design. Only worth considering if the latency budget in [0005](0005-latency-target.md) shows the scan is the bottleneck.

## How to choose

1. **It meets the pin and peripheral budget** with spares: fret inputs, string outputs, the ADC bus, expression inputs, USB, a UART if DIN MIDI stays, debug, crystal, LEDs, battery monitoring.
2. **USB device support** for full-speed without an external PHY (TB2-11).
3. **Fast enough for the scan and ADC readout** inside the latency budget (TB2-01).
4. **A package the team can solder and inspect.** Leaded packages can be hand-soldered and checked by eye. Leadless ones need hot air or reflow and are harder to rework.
5. **A development board exists**, so firmware can start before the PCB arrives.
6. **Supported by STM32CubeIDE**, or whatever toolchain the team picks in [0011](0011-tool-versions.md).
7. **How much of last year's firmware and schematic carries over.**
8. **Can be bought now**, in small quantity.
9. Cost.

## What we need to find out first

1. **The pin and peripheral count** for this year's design. It depends on [0001](0001-shared-fret-ghosting.md), [0002](0002-external-adc.md), [0007](0007-din-midi-out.md) and [0008](0008-expression-sensing.md), but a count with ranges is enough to rule options in or out.
2. **How much of last year's firmware is worth keeping.** If the answer is "the note table and not much else", reuse stops being an argument for option A. Prompt 2 answers this once the firmware is in `legacy/`.
3. **Whether the scan or the ADC dominates latency**, from the budget in [0005](0005-latency-target.md).
4. **Who on the team has used which toolchain.**

## Decision

_Not decided._

## Consequences

_Fill in when decided._
