# Parallel milestone: C++ five-beast team foundation

Status: standalone model, not integrated into the ROM. No progress percentage is claimed. Existing candidate ROM, rendering and combat source remain unchanged.

Team holds up to five unique beast IDs; each record has four equipped move slots. Records retain HP and uses across switches. Invalid, duplicate or over-capacity additions are rejected without changing the team. Initial member must be conscious. Voluntary switches are classified as consuming a turn; replacing an incapacitated active beast is classified as a forced replacement. The model does not itself schedule retaliation or resolve battle turns. Incapacitated destinations, same-slot selections and invalid indices are rejected. HP clamps at zero, uses never underflow, and unavailable moves fail.

Standalone tests cover empty-team safety, five-member capacity, duplicate identity, record validity, switching classifications, HP/use preservation, exhaustion and defeat. Host tests do not establish GBA integration or emulator acceptance.

Work split: Perplexity prepares this isolated C++ model and tests. Grok is asked by the owner to retrieve the exact-candidate mGBA workflow results and screenshots; no direct invocation is implied. Both inspect evidence before changing the runtime candidate. After the current emulator/visual gate passes, integrate four-move selection and five-member switching into a new playable candidate, then test that exact new ROM. Lady Erin, storage/save, learned-move history, LSM/HSM compatibility and traversal remain later tasks.
