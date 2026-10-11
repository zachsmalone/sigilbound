# Asset Requirements

## Beasts / Sprites
- Every named beast must have a unique sprite. No shared base sprites. No recolors or palette swaps as the only difference.
- Each must read as one of a kind in silhouette, shape, and detail.
- Nugwolf (001) is the current starter. Full roster is 102 beasts (001–100 ordinary, 101 Aurevion, 102 Noctyra).
- Battle and overworld sprites are both required. Current GBA code only has one static 16×16 Nugwolf used for both sides.

## Maps / Textures
- Tile-based top-down maps.
- Distinct terrain textures (grass, dirt, water, stone, paths, etc.).
- Clear collision boundaries. Running (hold B) must respect the same tiles as walking.

## Buildings
- Usable. Player must be able to enter, exit, and interact inside.
- Interiors require their own tilemaps and collision.
- Door and transition points must be defined and tested.

## Fences
- Key blocking element.
- Must stop movement cleanly on both walk and run speeds.
- No soft or partial collisions. Legal puzzle and path states must remain reachable.

## VFX
- Move effects, bonding indicators, damage feedback, and field HSM visuals (Water Sphere, Soar, Flex, Crush, Slice, Lumin).
- Must not obscure gameplay readability.

## Sound
- Battle hits, menu confirm/cancel, bonding success, field HSM activations, and ambient map loops.
- No audio exists in the current GBA slice.

## Logic Checks
- Unique beast identity must be preserved across team, storage (Lady Erin), and save/load.
- Sprite assignment must match beast ID, not just name or type.
- Collision, transitions, and HSM field use must be verified before any map is accepted.

## Current Reality
- Only one hardcoded Nugwolf sprite exists in `gba/source/main.cpp`.
- No map data, no building tiles, no fences, no VFX, no sound.
- Rules for team size, moves, Lady Erin, and HSMs exist in docs and Python but are not implemented in the GBA ROM beyond the basic battle diagnostic.

All new assets must ship with verification steps that can be checked in an emulator.
