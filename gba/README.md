# Sigilbound GBA migration: first C++ battle slice

GBA is now the production target; C++ is the gameplay language. Existing root Game Boy/Python files and build.yml are legacy diagnostics, preserved unchanged. The old Game Boy sprite proposal is superseded.

Build: with devkitARM installed, run `make -C gba`. Output: `gba/build/Sigilbound_GBA_Battle.gba`. Toolchain uses gba.specs startup/link configuration and gbafix header repair. CI uses devkitpro/devkitarm:latest; this is an initial automated build, NOT a pinned reproducible release. Pin a tested container digest after the first successful build.

Implemented source scope: two static 16x16 Nugwolf hardware OBJ sprites, mirrored to face each other; Mode 3 bitmap backdrop and text; HP bars and numbers; attack, Brace, move uses, XP, victory/defeat, restart. A attacks, B braces, Start restarts. Input is edge-triggered; simultaneous A/B prioritizes A; Start takes precedence. These are diagnostic controls, not final menu controls. No audio, animation, overworld, team switching, storage, bonding, saving or HSM traversal yet.

Shared C++ combat model is host-testable: initial HP 24/18, attack deals 3 with enemy retaliation of 2, Brace reduces retaliation to 1, 35 attack uses and 20 Brace uses, one-time 20 XP for victory. This preserves legacy diagnostic behavior; it is NOT the full simultaneous-choice/Speed ruleset in docs/PRODUCTION_RULES.md.

Verification: host compilation and combat tests can pass without a working GBA ROM. Cloud cross-compilation is a separate gate. The workflow labels artifacts as emulator-unverified and does NOT automatically publish them as verified releases. No claim of boot/rendering/Delta success until actual emulator execution and screenshot/input checks have been completed.

Next gates: compile; verify boot, both sprites, UI, held-button debounce and restart in a GBA emulator; inspect screenshots; pin toolchain; then add four-move menus and five-member team switching. Preserve the five-beast, Lady Erin, LSM/HSM and traversal design. Do not extend the old .gb build as the production game.
