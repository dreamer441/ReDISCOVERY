---
name: verify-task
description: Verify a requested Unreal task before completion using relevant builds, tests, logs, and a scope review.
---

Derive verification from the requested outcome. Build the actual editor target after C++ or native plugin changes; run relevant existing tests when available. Inspect relevant Unreal logs for errors. For MCP setup, verify the connection and read-only current-map/actor query rather than assuming configuration proves connectivity. Blender setup requires a read-only scene inspection.
Review Git status, diff, staged changes, and untracked files for unintended scope. Report passed checks, failed/unrun checks, and required decisions concisely. Do not claim success on configuration alone or create gameplay/content merely to test environment setup.
