# Sigilbound build status

## Current production candidate (GBA ROM)

- Commit: `5bc81343fe761cd7621e7a6960a9c913e8bfaea9`
- Actions run: `38185099919`
- Artifact: `Sigilbound-GBA-Cpp-Battle-Unverified-In-Emulator`
- ROM SHA-256: `265330806564e91df5f0ad37d9496437cedab2185c19e668179c70d1ef5e6a9c`
- C++ logic tests: passed
- GBA cross-compilation: succeeded
- Artifact upload: succeeded

## Emulator gate (open)

No emulator is available in the current review environment. The following acceptance checks remain unverified for this exact ROM:

- Boot reaches the battle screen
- Both Nugwolf sprites appear and face each other
- Labels, HP bars, and counters are readable
- A attacks once per press; holding A does not repeat turns
- B uses Brace correctly
- Victory and defeat display correctly
- Victory XP is awarded only once
- Start resets the battle

No screenshots exist. No Delta claim. Rendering and ROM gameplay changes remain paused until an emulator test of this checksum is recorded.

## Standalone battle controller (not yet in ROM)

- Commit: `0a0d353c43f924b9d80dca0e3ec8b9479ca6f3bf`
- Adds Fight/Team menu states, four-slot move selection, five-member team selection, edge-triggered input, switch retaliation rules, and forced replacement.
- Host tests included. Not connected to `gba/source/main.cpp`. Current ROM is unchanged.
- Only Tackle and Brace have implemented effects.

## Scope note

The GBA ROM slice is still a fixed-damage battle diagnostic. Lady Erin, overworld, HSMs, unique sprites, sound, and VFX are not implemented. Root Python/GB files are legacy diagnostics.

Next required evidence: emulator run of the candidate ROM above with checksum, emulator/version, and startup/victory/defeat screenshots. Then integrate the controller into a new ROM for switching and replacement tests.
