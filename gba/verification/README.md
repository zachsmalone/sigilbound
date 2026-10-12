# Exact-candidate runtime verification

This harness does not change gameplay, ROM graphics, the GBA Makefile, or the existing build workflow. It downloads the previously identified artifact from run 38185099919 and refuses any ROM whose SHA-256 differs from 265330806564e91df5f0ad37d9496437cedab2185c19e668179c70d1ef5e6a9c. An expired artifact or missing access is an infrastructure failure, not a game failure. Do not silently substitute a rebuild.

CI installs Ubuntu's libmgba-dev and records its package version. It uses mGBA's built-in BIOS replacement, not a proprietary BIOS. The harness executes the ROM, drives A/B/Start, reads expected displayed counter glyphs from emulated framebuffer memory, and captures the emulator's rendered startup/victory/defeat PNGs. Held-A, reward-once, exhaustion, defeat and resets are tested. Checks are for this diagnostic's fixed layout and values, not future gameplay.

No local mGBA compilation or execution has been performed when this infrastructure is proposed. Header/library compatibility, screenshot support and runtime behavior must be established by the actual CI run. A passing host combat test is not a passing emulator test. Runtime errors produce logs where possible; evidence uploads run even on failure.

Automated framebuffer-memory checks do not establish full display readability or visible sprites. Review all three rendered screenshots for both Nugwolf characters, facing direction, clipping and labels; keep those visual acceptance items pending until reviewed. No Delta compatibility claim. Before interpreting failures, confirm the identity gate passed and distinguish installation/harness failures from ROM behavior.
