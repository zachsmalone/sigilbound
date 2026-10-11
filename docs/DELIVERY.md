# Phone-friendly build delivery

Successful main-branch push or manual builds publish a new diagnostic prerelease after the structural and PyBoy checks pass. Pull requests do not publish.
The build job keeps read-only repository permissions. A separate publish job has contents:write only to create a tag/prerelease and upload assets. Repository rules can still block it.
Publishing validates report success, matching commit, matching ROM SHA-256, and presence of three screenshots. It does not change the builder, gameplay, or existing releases.
Each prerelease includes the .gb, build validation, emulator report, and startup/victory/defeat screenshots. Release notes contain the exact report so a repository connector can read it without Actions-artifact access.
A new tag uses workflow run ID and attempt. These are test prereleases, not finished-game releases or confirmed Delta builds. Publication does not make a private repository public; downloads still follow repository access rules.
No API key or personal token is added. Publishing uses the workflow's GitHub token. After this change is committed, a new run must complete before a download link exists.
GitHub connector can list the prerelease and return its asset URLs. Opening/saving the ROM to your phone and testing in Delta remain device-side actions.
