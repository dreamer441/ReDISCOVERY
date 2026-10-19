---
name: build-unreal
description: Build this project's detected Unreal editor target and diagnose compiler errors after C++ or plugin changes.
---

Inspect Git status. Read the .uproject EngineAssociation and locate the matching installation via registry or documented path; verify Engine/Build/Build.version and Build.bat exist. Currently UE 5.8.2 is at D:\UE_5.8.

Discover Source/*Editor.Target.cs and use its actual target. This project uses RediscoveryEditor (renamed from NEWMAGICwClaudeEditor on 2026-10-19).

Run Engine/Build/BatchFiles/Build.bat RediscoveryEditor Win64 Development with the absolute -Project="D:\REDISCOVERY-MAIN\Rediscovery\REDISCOVERY.uproject" argument (or simply run Tools\RD_Build.cmd), -WaitMutex and -NoHotReloadFromIDE. Capture output under ignored Saved/Logs. Report exit code and relevant compiler errors, diagnose safely, repair within requested scope, and rebuild. Do not dump giant logs or hide failure. Stop for a genuine architectural decision; never modify Engine source.
