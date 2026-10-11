# Sigilbound build status

This repository baseline is a standalone fixed-damage battle diagnostic, not the complete adventure.
A attacks; B braces; release buttons between turns. A/B after result resets the entire test party.
Nugwolf begins at 24 HP; enemy at 18 HP. Victory assigns XP 20 only to this freshly initialized fixture.
No exploration, party switching, capture, campaign rewards, audio or saves are integrated.

The repository builder uses Pillow's bundled font and revised title; it is not byte-identical to the earlier chat ROM. Earlier limited-interpreter results do not automatically verify this revision.
Structural unit tests are included. No complete emulator or Delta test is configured. A green Actions run means the included checks passed, not that the game works in Delta.

Step 1 initial planning established; Steps 2-7 remain partially implemented/unaccepted; Steps 8-20 remain ahead. No completed-game percentage is claimed.

Build locally: python -m pip install -r requirements.txt; python -m unittest discover -s tests -v; python build_rom.py.
Phone workflow: repository Actions > Build Sigilbound battle baseline > Run workflow. After successful completion download the Sigilbound-battle-baseline artifact; extract its .gb for Delta. GitHub authorization and Actions availability/policy must permit execution. No run has been observed yet.

Memory: C000 input latch; C001 result; C002 guard; C100-C28F party buffer; C110-C14F starter record; C300 enemy HP. HP/XP manipulation is low-byte diagnostic logic only; replace with full-width routines before campaign integration.

Next acceptance target: full-emulator execution of this revision, then merge one battle into exploration without resetting party.
