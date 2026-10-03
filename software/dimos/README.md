# Dimos

This is the software which runs on the dashboard.

## Running

- **macOS / Windows (dev):** `flutter run -d macos`. CAN data is simulated by `DevApiWorker`; no CAN hardware or native library is needed.
- **Linux (car):** `flutter run -d linux` / `flutter build linux`. Reads SocketCAN `can0` and decodes frames through `libdimos_jsoncan`.

## CAN (jsoncan)

The dashboard is the `DIMOS` node in the car's CAN JSON (`can_bus/hexray/DIMOS`). It is listed in
`bus.json` `loggers`, so jsoncan generates real `needsLog` / `needsTelem` data capture for it.
Add messages the dashboard needs to `DIMOS_rx.json`, and list them explicitly: `"messages": "all"`
doesn't compile on hexray, because several Boot messages share signal names.

`native/CMakeLists.txt` builds `libdimos_jsoncan` at build time:

1. It runs the Rust jsoncan (`scripts/code_generation/jsoncan-rust`, `cppcodegen`) on `can_bus/<DIMOS_CAR>` for the `DIMOS` node.
2. It compiles that with `native/jsoncan_bridge` (a C API over the generated C++).
3. Dart loads the result via FFI (`lib/data/services/jsoncan_ffi.dart`).

The Linux build includes it automatically. Requirements:

- CMake and a C++20 compiler.
- `cargo`, to build `cppcodegen`. Alternatively, pass `-DJSONCAN_BINARY_GENERATE=OFF` and put `cppcodegen` on `PATH`.

To target another car, give it a `DIMOS` node and configure with `-DDIMOS_CAR=<car>`.

To build the library on its own (e.g. on macOS for testing):

```bash
cmake -S native -B build/native && cmake --build build/native
```
