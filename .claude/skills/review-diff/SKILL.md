---
name: review-diff
description: Review this project's working tree and staged or unstaged changes for scope, generated files, and risk.
---

Verify git rev-parse --show-toplevel resolves to this project, then inspect git status --short, git diff --stat, git diff, and git diff --cached. Inspect untracked setup/source files separately because git diff omits them.
Identify unrelated modifications, generated Unreal/IDE output, missing LFS attributes, secrets, and unexpected plugin changes. Summarize concrete risks and validation gaps. Preserve all user changes; never stage, discard, commit, or push automatically.
