# 0009: Battery and power tree

| | |
| --- | --- |
| Status | Open |
| Owner | `TBD(team)`. Needs one electrical and one mechanical owner |
| Needed by | Before the controller schematic's power sheet, and before the battery mount is designed |
| Requirements | F5, Q3, M5, TB2-04, TB2-13 |
| Analysis | None planned yet |

## The question

What battery powers the instrument, how is it charged and protected, and how do we get from its voltage to the rails the electronics need?

## Why it matters

"Integrated battery" is a project goal. A battery built into a wooden instrument that someone holds against their body is also the part of the project with the most serious safety consequences if it is done badly. The choice sets the supply rails available to every other circuit, the run time, the weight, and a cavity in the body.

## What Tiger Bass 1 did

A 9 V rechargeable lithium-ion battery in the standard 9 V size, which charges through its own USB-C port, so the board had no charging circuit. An LT1763 linear regulator made 3.3 V for the microcontroller. The 9 V rail also went off-board on a header. Estimated run time was about 3 hours.

A linear regulator dropping 9 V to 3.3 V turns most of the battery's energy into heat: at the same current in and out, roughly 3.3 / 9 of the power reaches the load.

## First: what do the loads need?

List every load with its voltage range and current before picking a battery. In particular the pickups: if they have a preamp that needs 9 V, that fixes one rail. `TBD(team)`.

## Battery options

### A. Keep a 9 V rechargeable in the standard size

Charging is inside the battery, so we do not design it. Check the battery's datasheet for what protection it includes. Replaceable by the user. Low capacity for its size, and the 9 V to 3.3 V step wastes most of it unless a switching regulator is used.

### B. Single lithium-ion or lithium-polymer cell

About 3.7 V nominal. Much more energy for the volume. We then have to provide a charger IC, a protection circuit (or buy a cell with one built in), and a regulator that works across the cell's voltage range. Needs a boost converter for any rail above the cell voltage.

### C. Two or more lithium cells in series

Higher voltage without a boost converter. Needs cell balancing during charging, which adds complexity and risk. Hard to justify unless a load needs the voltage.

### D. NiMH cells

Safer chemistry and simple to charge slowly. Heavier and lower energy for the volume. Several cells are needed for a useful voltage.

### E. An off-the-shelf USB power bank module

Charging, protection and a regulated 5 V output in one bought part. Least design risk. Many shut off when the load current is small, which a low-power instrument may trigger.

## Regulation options

- **Linear regulator:** quiet and simple. Efficient only when input and output voltages are close.
- **Switching regulator:** efficient across a wide input range. It generates electrical noise at its switching frequency, right next to pickups and an ADC measuring small signals. Layout, filtering and the switching frequency all need care.
- **Switching regulator followed by a linear one** for the analog rail: efficiency from the first, quiet from the second.

## How to choose

1. **Safety (Q3, TB2-13):** protection against over-charge, over-discharge, short circuit and over-temperature, and a plan for what happens if the cell fails inside the body.
2. **Run time (TB2-04)** at the measured current draw.
3. **Rails every load needs**, with the fewest conversions.
4. **Noise** on the analog rail and in the pickups.
5. **How it is charged:** which connector, how long, whether the instrument can be played while charging, and whether that shares the USB port in [0006](0006-usb.md).
6. **Whether the user can replace it** without tools.
7. Size and weight in the body (TB2-05).
8. Any LSU or Capstone Design Program rules on lithium batteries in student projects.

## What we need to find out first

1. **A current budget:** every load, typical and worst case. Measure Tiger Bass 1's total draw as a starting point.
2. **What the chosen pickups need** for supply.
3. **The run time target**, from the sponsor.
4. **Program and university rules** on lithium cells, charging in the lab and storage. Ask the advisor.
5. **Whether a switching regulator's noise shows up** in the pickup channels: a bench test with an evaluation board near the pickup.
6. **How much body volume** the mechanical engineers can give a battery.

## Decision

_Not decided._

## Consequences

_Fill in when decided._
