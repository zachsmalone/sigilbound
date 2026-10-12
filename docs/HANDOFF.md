# Handoff for other AI

## Current gate (do not skip)
Emulator-verify this exact ROM before any rendering or gameplay merge:
- SHA-256: `265330806564e91df5f0ad37d9496437cedab2185c19e668179c70d1ef5e6a9c`
- Commit: `5bc81343fe761cd7621e7a6960a9c913e8bfaea9`
- Required: startup / victory / defeat screenshots, emulator version, confirmation of both sprites, readable UI, single-press A/B, one-time XP, Start reset.

## Prepared on this branch
- `gba/include/battle.hpp` — four-move and five-beast team structures. Existing diagnostic `act()` unchanged.
- `gba/tests/team_move_test.cpp` — host tests for team count, move slots, and invalid slot rejection.
- `act_move()` is a stub that falls back to the diagnostic path.

## Next implementation once gate passes
1. Wire a four-move menu in the GBA ROM using the structures above.
2. Add voluntary team switch (1–5 active beasts). Forced switch on faint. Block switches to zero-HP beasts.
3. Keep host tests green. Do not change the diagnostic ROM behavior until the menu path is verified in emulator.
4. Then Lady Erin storage (atomic exchange, identity preservation) and basic save format.

## Constraints
- Unique beast sprites only (no recolors). See `docs/ASSET_REQUIREMENTS.md`.
- Preserve production rules: 1–5 team, 4 equipped moves, Lady Erin, HSM field moves, no permanent death.
- Do not extend the legacy `.gb` path as production.
