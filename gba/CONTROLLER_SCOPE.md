# Battle-controller integration gate

Standalone C++ controller only. Not included by gba/source/main.cpp; no playable ROM or rendering changes. Local host tests must pass and the current candidate's emulator/visual gate must be resolved before ROM integration.

Implemented logic: Fight/Team menu states; four-slot move selection; five-slot team selection; up/down navigation; A confirm; B cancellation; forced replacement cannot be cancelled; edge-triggered held-button protection. Start resets the diagnostic fixture and does not repeat while held. Invalid moves/switches consume no uses, HP or enemy action. Voluntary switching receives an enemy hit; forced replacement does not. Defeated members cannot fight. Team-wide defeat ends battle. Victory rewards once.

Only move IDs 1 (Tackle) and 2 (Brace) are implemented. Other IDs are deliberately rejected, not silently treated as attacks. Four slots are supported; four distinct battle moves are not yet authored. Enemy always retaliates after a valid non-winning player action; full simultaneous-choice, priority, Speed, items, retreat and bonding are not implemented. XP is a battle reward counter, not persistent per-beast experience. Fixture restart is not sanctuary recovery. No storage, teaching or saving.

Next integration: draw menu options, names, move uses and five-member HP; wire controller into main.cpp on a reviewed branch; cross-compile and verify the new exact ROM including switching/replacement tests and screenshots. Coordinate with Grok through the owner; no automatic invocation or overlapping main.cpp edits.
