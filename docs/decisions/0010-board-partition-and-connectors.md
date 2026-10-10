# 0010: Board partition and connectors

| | |
| --- | --- |
| Status | Open |
| Owner | `TBD(team)`. Needs one electrical and one mechanical owner |
| Needed by | Before either board outline is drawn |
| Requirements | Q5, TB2-05 |
| Analysis | The interface table in [`../architecture.md`](../architecture.md#interfaces-that-cross-a-board-boundary) |

## The question

Which circuits go on which board, and what connectors and cables join the boards to each other and to the rest of the instrument?

## Why it matters

Every connection between boards is a connector to buy, a cable to route through the body, and a place for an intermittent fault on an instrument that gets shaken and carried. Last year's instrument had its boards and wiring in the body cavity, with two 40-way ribbon cables to the neck. "Sleeker" (TB2-05) largely means doing this part better.

This decision is where the electrical and mechanical designs meet. Neither side can finish an outline without it.

## Part 1: what goes where

### A. Two boards: a passive fret board and a controller board

As last year. The fret board is only contacts and traces. Everything active is on the controller board in the body. The neck stays simple and nothing in it can fail electrically except a trace or the cable.

### B. Two boards, with some electronics in the neck

For example multiplexers or shift registers on the fret board, so fewer wires leave the neck. Only worth it if [0001](0001-shared-fret-ghosting.md) lands on an option that needs more than 20 lines.

### C. Three or more boards

A controller board plus small boards where they are needed: one at the bridge for the pickup front end, close to the pickups, to keep small signals off long wires; one for the expression sensors; one for the battery and charging. Shorter analog wiring and more freedom in the body. More connectors and more boards to order.

### D. One controller board with everything except the fret contacts

All circuits on one board, with pickups and controls on flying leads. Fewest boards. Long leads for the smallest signals.

## Part 2: connectors and cables

For each interface in the architecture table, choose:

- **Cable type.** Ribbon with IDC connectors, as last year: cheap, easy to make, bulky. Flat flexible cable: thin and neat for the neck, fragile and needs matching connectors. Discrete wires in a crimped housing: flexible to route, each one made by hand. Shielded cable for the pickup signals.
- **Whether the connector locks.** Friction-fit headers work loose under vibration. Latching or locking connectors do not.
- **Polarisation.** A connector that can go in backwards will be put in backwards.
- **Where it sits on the board**, so the cable reaches without crossing the board or a noisy part.

## How to choose

1. Reliability on an instrument that is moved and played: locking, strain relief, as few connections as possible.
2. Signal quality for the pickup channels: short, shielded or kept away from the USB and any switching regulator.
3. Whether the instrument can be assembled and taken apart without soldering.
4. What fits the body and the neck joint.
5. What the team can crimp or assemble with the tools available.
6. Test access: every connector is also a place to probe during bring-up.

## What we need to find out first

1. **The outcome of [0001](0001-shared-fret-ghosting.md):** how many lines leave the neck, and whether the fret board is passive.
2. **How the neck joins the body**, and what cable can pass through that joint.
3. **How long each cable run is** in the body design, especially from the pickups.
4. **What went wrong with last year's wiring.** Look at the instrument: which connections were reworked, taped or glued.
5. **Where the string drive wires attach to the strings**, and whether that is at the bridge or the headstock.
6. **Which connectors must be reachable from outside:** USB, charging, MIDI out.

## Decision

_Not decided._

## Consequences

_Fill in when decided._
