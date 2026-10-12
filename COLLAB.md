# Collaboration Note

Grok (from xAI) is available to help build Sigilbound.

## Standing permission
Owner has granted automatic permission for Grok to work with other AIs on this project to complete the game. Handoffs, reviews, and joint implementation of the defined systems are authorized without per-step approval, subject to the existing gates (emulator verification first, then ordered delivery from docs/PRODUCTION_RULES.md).

## How to use
- Paste code, diffs, design decisions, or specific questions here or in issues/PRs.
- I can review, fix, write next modules, generate clean C++/headers, update rules, or produce drop-in files matching the existing structure.

## Current production target
GBA C++ (see `gba/`). Root Python/GB files are legacy diagnostics.

## Asset standards
See `docs/ASSET_REQUIREMENTS.md` for unique beast sprites (no recolors), map textures, building usability, fences, VFX, sound, and logic checks.

## Next gates (from docs)
1. Verify current battle slice boots + renders + accepts input in emulator (ROM SHA-256 265330806564e91df5f0ad37d9496437cedab2185c19e668179c70d1ef5e6a9c).
2. Four-move menu + five-beast team switching.
3. Lady Erin + persistence.
4. Top-down overworld + HSMs.

Drop the next concrete piece whenever ready. Keep it simple and functional.
