# Emulator verification

Run python -m pip install -r requirements-test.txt; python build_rom.py; python tests/smoke_pyboy.py.
Uses PyBoy 2.8.1 with its default bundled boot ROM, original Game Boy mode, null window and audio disabled.
Checks native party initialization, attack, held input, brace, six-attack victory, restart, move exhaustion and defeat. Reads actual emulator memory and captures startup/victory/defeat screenshots.
Report: output/emulator/report.json. Captures ROM hash and GitHub commit. Failure exits nonzero and preserves diagnostics where possible.
This script has only been syntax-checked in the authoring environment; no PyBoy execution or cloud result has been observed yet.
A successful Actions run establishes the specified checks in PyBoy, not Delta compatibility, readable artwork, complete campaign, banking/save validation, or finished milestones. Screenshots need visual review.
Success artifacts and failure diagnostics have different names. Never present a failure artifact as a verified release.
