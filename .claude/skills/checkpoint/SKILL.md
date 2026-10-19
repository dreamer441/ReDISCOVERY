---
name: checkpoint
description: Inspect, commit and push a safe Git checkpoint of reviewed in-scope work to the canonical REDISCOVERY repository.
---

Verify the repository root and origin (https://github.com/dreamer441/ReDISCOVERY.git); inspect status, diffs and untracked files. Flag generated/unrelated files or secrets and exclude them. Commits and pushes of reviewed in-scope work are authorized by the owner (19 Oct 2026). Write the message to Saved\rd_commit_message.txt and run Tools\RD_GitSync.cmd (Windows Git with LFS), then read Saved\Logs\rd\git.log.
Never reset hard, clean destructively, force push, rewrite history, or discard user work. Before a usage limit is reached: update docs/PROJECT_STATE.md, checkpoint the latest working state, and output a handoff prompt.
