# Sigilbound build status

Standalone fixed-damage battle diagnostic, not the complete adventure. A attacks; B braces; release buttons between turns. A/B after result resets the test party. Nugwolf starts at 24 HP, enemy at 18 HP; victory assigns XP 20 to freshly initialized starter.
No exploration, party switching, bonding, campaign reward handling, audio or saves are integrated.
Repository reconstruction uses Pillow's bundled font and revised title; earlier chat-ROM interpreter results do not automatically verify this revision.
Six structural tests and a headless PyBoy verification script are configured. The PyBoy script has been syntax-checked only; execution and GitHub run status have not been observed. No Delta verification.
Step 1 planning established; Steps 2-7 partially implemented/unaccepted; Steps 8-20 remain ahead. No completion percentage claimed.
Build: pip install -r requirements-test.txt; python -m unittest discover -s tests -v; python build_rom.py; python tests/smoke_pyboy.py.
Cloud build uploads a successful artifact only after all checks pass; failures upload separately named diagnostics where possible. See docs/VERIFICATION.md.
WRAM: C000 input latch, C001 result, C002 guard, C100-C28F party buffer, C110-C14F starter, C300 enemy HP. HP/XP are low-byte fixture logic, not production full-width routines.
Next: observe PyBoy results, review screenshots, fix actual failures, then merge battle with exploration without resetting party.
