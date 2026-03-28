# Dimos App Status and Completion Overview

Date: March 3, 2026  
Repo baseline: `/Users/petergu/Documents/School/FElec/Consolidated-Firmware/software/dimos`

## 1) Current Status Snapshot

### What is implemented
- Flutter desktop app scaffold is functional with Linux, macOS, and Windows targets present.
- Core UI pages exist and are routable:
  - LV
  - Racing
  - Skidpad
  - Braking
  - Debug
- State management is set up using `provider` and centralized variable classes in `lib/data/services/can_variables.dart`.
- Development data simulation path exists (`DevApiWorker`) for macOS/Windows and periodically updates UI state.
- Linux CAN receive loop exists (`CanApiWorker`) and listens on `can0` using `linux_can`.

### What is currently blocking production readiness
- CAN data path is not decoding real frames yet; Linux worker emits `"TRIGGERED"` only.
- CAN-driven variable update methods do not currently call `notifyListeners()`, so UI does not react to Linux CAN updates.
- Warning bar is wired, but warning semantics are placeholder text and not mapped to real CAN warning signals.
- JSON CAN autogen RX/TX integration is not implemented.
- Automated testing is broken:
  - `flutter test` fails because `test/widget_test.dart` still references `MyApp` (no longer exists).
- Static analysis has one hard error and multiple warnings (`flutter analyze`).

## 2) Verified Baseline (from this repo state)

### Runtime architecture
- In `lib/main.dart`:
  - macOS/Windows -> `DevApiWorker` -> calls `update*Dev(...)` methods.
  - Linux -> `CanApiWorker` -> calls `update*Can()` methods.

### Linux CAN behavior today
- `lib/data/services/can_api.dart`:
  - Selects interface by exact name `can0`.
  - Throws if interface is down.
  - Reads frames from socket stream.
  - Sends only `"TRIGGERED"` back to app isolate (no frame payload decode).

### Plugin/integration note
- `linux_can` is declared in `pubspec.yaml`.
- Generated Linux plugin file (`linux/flutter/generated_plugins.cmake`) currently lists `screen_retriever_linux` and `window_manager`; no explicit `linux_can` entry appears (possible package design detail, but should be validated during integration testing).

### Tooling status
- `flutter analyze`: fails with one error (`MyApp` type issue in test) plus warnings/info.
- `flutter test`: fails due to `MyApp` constructor reference in `test/widget_test.dart`.

## 3) High-Priority Work (Linux + Flutter)

## Priority 1: Prove Linux CAN library reads real frames
Goal: Confirm that real CAN frames are read on target Linux and shown using the same dummy-style visual feedback currently used for dev CAN.

Work items:
- Add frame-level logging/telemetry in Linux CAN worker (ID, DLC, timestamp, data bytes).
- Replace `"TRIGGERED"` passthrough with structured frame payload handoff to app isolate.
- Add robust interface discovery and error handling:
  - device missing
  - device down
  - permission denied
  - wrong interface name
- Add a temporary on-screen “CAN Frame Monitor” debug panel showing incoming frame count + latest frame metadata.

Failure points to address (as requested):
- Library not being used properly.
- CAN device on board not being named/accessed properly.

Acceptance criteria:
- On target Linux board, app shows live frame counter incrementing with bus traffic.
- If interface name is wrong/down, UI shows clear error state (not silent failure).

## Priority 2: Integrate JSON CAN + autogen + RX/TX in Flutter
Goal: End-to-end decode path from raw CAN frame -> autogen decode -> app state update.

Work items:
- Define canonical JSON CAN source and versioning policy.
- Add autogen pipeline outputs for RX and TX into repo (or generated artifact location).
- Integrate generated decode/encode layer into Flutter runtime path.
- Validate whether integration is:
  - pure Dart decode, or
  - FFI bridge to native-generated code.
- Create smoke tests for representative message IDs and boundary values.

Acceptance criteria:
- App can decode at least a representative subset of real CAN messages from live or replayed traffic.
- TX path can construct at least one validated message from app side.
- Decode pipeline is reproducible from JSON CAN source with documented generation command.

## Priority 3: CAN warnings processing + warning bar behavior
Goal: Show that warnings are correctly processed and visualized, enabling broader signal processing rollout.

Work items:
- Define warning signal mapping table (`signal -> condition -> display token/priority`).
- Implement deduplication and clear/reset behavior for warnings.
- Ensure warning updates call `notifyListeners()` and refresh UI in real time.
- Add warning priority and maximum-display policy for compact dashboard layout.

Acceptance criteria:
- Warning bar reflects active warning set from CAN-driven conditions.
- Warnings appear/disappear correctly without duplicates.
- Behavior is validated with replayed test frames for warning on/off transitions.

## 4) Linux-Related Work (Jakob)

## Startup and interface naming
Goal: App boots immediately on board and CAN interface naming is deterministic.

Work items:
- Add `systemd` service to auto-start dashboard app at boot.
- Add persistent CAN naming policy:
  - systemd-networkd link rule and/or udev naming rule.
- Add startup dependency ordering so app waits for CAN device readiness.
- Add health checks + restart policy in service unit.

Acceptance criteria:
- After reboot, app launches automatically and connects without manual `ip link` intervention.
- CAN interface has stable expected name on every boot.

## 5) Lower-Priority Work

## Yocto build flow
- Create documented, minimal-change Yocto workflow.
- Keep recipe changes small and version-scoped.
- Add checklist for Yocto upgrades and compatibility validation.

## IO strategy discussion
- Current input assumptions (keyboard/mouse) are development-only.
- Define board IO plan:
  - physical buttons/rotary/other input devices
  - event routing into Flutter
  - failure and debounce behavior

## 6) Suggested Execution Order

1. Fix baseline quality gates (`flutter test` + `flutter analyze` error).  
2. Complete Linux CAN frame monitor and interface error handling.  
3. Integrate JSON CAN autogen RX decode path.  
4. Build warning pipeline and warning bar semantics.  
5. Add `systemd` boot + deterministic CAN naming.  
6. Finalize Yocto/IO processes.

## 7) Immediate Technical Debt to Clean Up

- Update/remove stale default widget test (`MyApp` reference).
- Add `notifyListeners()` to CAN update paths where UI should react.
- Remove or track unused imports/variables found by analyzer.
- Normalize naming style (`Battery.dart`, `Speedometer.dart`) per lint rules.

## 8) Definition of “Done” for this phase

- Linux target receives and displays real CAN-driven values.
- JSON CAN autogen path is documented and repeatable.
- Warning bar behavior is correct and validated against message transitions.
- Board boots directly into app with stable CAN device naming.
- CI/local checks: analyzer and tests pass on current branch.
