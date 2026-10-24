# Dimos

This is the software which runs on the dashboard: a Flutter app that decodes the car's CAN bus with the
Rust jsoncan generated code.

```
 car CAN bus ──► SocketCAN (can0 / vcan0) ──┐
                                             ├─► libdimos_jsoncan (generated decoder, C++) ──► Flutter UI
 tool/can_sim ──► UDP 127.0.0.1:5005 ────────┘
```

- **On Linux** the app reads classic and CAN FD frames from a SocketCAN interface (`can0` by default).
- **On macOS** (and on Linux, if you ask for it) the app listens on UDP for frames from `tool/can_sim`.
- **`tool/can_sim`** generates dummy traffic: every message on the dashboard's bus, packed by the Rust
  jsoncan from the car's CAN JSON, with each signal sweeping through its valid range.

Either way, frames go through the same generated jsoncan decoder as on the car.

## Requirements

|              | macOS                          | Linux                                                                                   |
| ------------ | ------------------------------ | --------------------------------------------------------------------------------------- |
| Flutter      | `brew install --cask flutter`  | [flutter.dev install guide](https://docs.flutter.dev/get-started/install/linux/desktop) |
| Build tools  | Xcode, `brew install cmake`    | `sudo apt install clang cmake ninja-build pkg-config libgtk-3-dev`                      |
| Rust (cargo) | [rustup.rs](https://rustup.rs) | [rustup.rs](https://rustup.rs)                                                          |

Rust is needed both for the simulator and to build `cppcodegen`, the jsoncan generator. Building the app
generates the decoder automatically.

## Running on macOS (simulated CAN)

Run the simulator in one terminal and the app in another, in either order:

```bash
cd software/dimos/tool/can_sim && cargo run --release
```

```bash
cd software/dimos && flutter run -d macos
```

The simulator prints how many messages it's sending and its frame rate. The Xcode build of the app
builds `libdimos_jsoncan.dylib` and bundles it into the app (`tool/build_native_macos.sh`).

## Running on a Linux dev machine (no car)

**Option A: virtual CAN (`vcan0`).** This exercises the real SocketCAN + CAN FD reader:

```bash
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set vcan0 mtu 72
sudo ip link set up vcan0
```

`mtu 72` makes `vcan0` CAN FD-capable. Then:

```bash
cd software/dimos/tool/can_sim && cargo run --release -- --socketcan vcan0
```

```bash
cd software/dimos && flutter run -d linux --dart-define=DIMOS_CAN_INTERFACE=vcan0
```

You can watch the traffic with `candump vcan0` from `can-utils`.

**Option B: UDP**, like on macOS:

```bash
cd software/dimos/tool/can_sim && cargo run --release
```

```bash
cd software/dimos && flutter run -d linux --dart-define=DIMOS_CAN_SOURCE=udp
```

## Running on the car

Hexray's FDCAN bus is CAN FD **without** bit-rate switching, at 1 Mbit/s nominal (see the FDCAN config in
`firmware/hexray/VC/src/cubemx`). Bring the interface up, then build and run the release bundle:

```bash
sudo ip link set can0 type can bitrate 1000000 dbitrate 1000000 fd on
sudo ip link set can0 up
```

```bash
cd software/dimos && flutter build linux --release
```

```bash
software/dimos/build/linux/<arch>/release/bundle/dimos
```

`<arch>` is `arm64` or `x64`. The bundle is self-contained: the decoder library is in `bundle/lib/`.
Reading the bus doesn't need root; bringing the interface up does.

## Windows

The jsoncan decoder isn't built for Windows, so the app defaults to `DIMOS_CAN_SOURCE=sim`: no CAN, an
in-app fake car (`lib/data/services/vehicle_sim.dart`). Use the debug keys below to drive it. `sim` also
works on macOS/Linux for UI work without `tool/can_sim`.

## Inputs

**Steering wheel.** PLACEHOLDER until the wheel's device tree exists: the plan is `gpio-keys` /
`rotary-encoder` key events, so a keyboard stands in (`lib/data/input/input_service.dart`).

| Keys             | Input                                       |
| ---------------- | ------------------------------------------- |
| A / D            | LEFT / RIGHT: page between DRIVE and FAULTS |
| Return           | OK: confirm drive mode, dismiss popup       |
| T or Esc         | TELEM                                       |
| 1 / 2            | Torque vectoring rotary − / +               |
| 3 / 4            | Regen strength rotary − / +                 |
| ← / →            | Drive mode rotary: open / move selector     |
| Space            | Drive mode rotary push: confirm             |
| Q / E (hold 1 s) | TV / regen rotary push: toggle on / off     |

LEFT + RIGHT + OK together starts Snake (only in INIT).

**Debug keys** (development only: debug builds, or `--dart-define=DIMOS_DEV_KEYS=true`;
`lib/data/input/debug_keys.dart`). <code>`</code> shows a panel with a button for each wheel input and debug action.

| Key            | Action                                                  |
| -------------- | ------------------------------------------------------- |
| S              | System state step: INIT → READY TO DRIVE → DRIVE → INIT |
| F              | Open a random shutdown node or fault a random board     |
| X              | Press a random e-stop                                   |
| I / B / M      | Open the IMD / BSPD / BMS OK node                       |
| N              | Random info message                                     |
| C              | Clear debug faults and the forced system state          |
| K / U / L      | Snake / fun effects on-off / day-night theme            |
| <code>`</code> | Debug panel                                             |

Debug faults and the forced system state are layered over whatever CAN says, and stay until **C**.

**Routing.** Wheel inputs pass down a middleware chain (`lib/routing/input_router.dart`), most specific
first: Snake chord → Snake (while playing) → `DriverControls` (TV/regen/mode, popups) → `RouterProvider`
(pages). `DashboardController` (`lib/routing/dashboard_controller.dart`) builds the chain, runs debug
actions, and jumps to the FAULTS page when a new fault appears.

**Not on CAN yet.** DIMOS doesn't transmit, so TV level, regen strength and drive mode only live on the
dashboard (`DriverControls`) until `DIMOS_tx.json` gets messages for them.

## App options (`--dart-define`)

| Define                | Default                                                | Meaning                                          |
| --------------------- | ------------------------------------------------------ | ------------------------------------------------ |
| `DIMOS_CAN_SOURCE`    | `socketcan` on Linux, `udp` on macOS, `sim` on Windows | Where frames come from (`sim`: no CAN, fake car) |
| `DIMOS_CAN_INTERFACE` | `can0`                                                 | SocketCAN interface (`socketcan` source)         |
| `DIMOS_CAN_UDP_PORT`  | `5005`                                                 | UDP port to listen on (`udp` source)             |
| `DIMOS_DEV_KEYS`      | `false` (always on in debug builds)                    | Debug keys and panel in release builds           |

Pass these to `flutter run` or `flutter build`, e.g. `--dart-define=DIMOS_CAN_INTERFACE=vcan0`.

## Simulator options (`tool/can_sim`)

Run `cargo run --release -- --help` for the full list.

| Option            | Default          | Meaning                                                           |
| ----------------- | ---------------- | ----------------------------------------------------------------- |
| `--car`           | `hexray`         | Which `can_bus/<car>` JSON to simulate                            |
| `--node`          | `DIMOS`          | Simulate everything the other nodes on this node's buses transmit |
| `--udp`           | `127.0.0.1:5005` | Where to send UDP frame batches                                   |
| `--socketcan`     | (off)            | Send to this SocketCAN interface instead of UDP (Linux)           |
| `--min-period-ms` | `50`             | Fastest any message is sent; `1` uses the real cycle times        |

## Troubleshooting

Errors show up in the app's warning bar, prefixed `CAN:`, and in the console.

| Message                                 | Fix                                                                                                 |
| --------------------------------------- | --------------------------------------------------------------------------------------------------- |
| `can0: interface not found`             | Wrong interface name, or the CAN device/driver isn't present (`ip link`)                            |
| `can0: interface is down`               | `sudo ip link set can0 up` (after setting bitrates, see above)                                      |
| `can0: kernel has no SocketCAN support` | `sudo modprobe can_raw`; the kernel/image needs `CONFIG_CAN_RAW` (Docker Desktop's doesn't have it) |
| `jsoncan library not loaded`            | The native build didn't run or failed; check the `flutter build` output                             |
| `UDP port 5005: ...`                    | Another app instance is using the port; close it or use `DIMOS_CAN_UDP_PORT`                        |
| Nothing on screen, no errors            | Is the simulator running and pointed at the same port/interface?                                    |
| Xcode: `cmake: command not found`       | `brew install cmake`                                                                                |

## CAN (jsoncan)

**The `DIMOS` node.** The dashboard is the `DIMOS` node in the car's CAN JSON (`can_bus/hexray/DIMOS`). It is
listed in `bus.json` `loggers`, so jsoncan generates real `needsLog` / `needsTelem` data capture for it.

**Adding messages.** Add the messages the dashboard needs to `DIMOS_rx.json`, listed explicitly:
`"messages": "all"` doesn't compile on hexray, because several Boot messages share signal names.

**Native library.** `native/CMakeLists.txt` builds `libdimos_jsoncan` at build time:

1. It runs the Rust jsoncan (`scripts/code_generation/jsoncan-rust`, `cppcodegen`) on `can_bus/<DIMOS_CAR>` for
   the `DIMOS` node.
2. It compiles that with `native/jsoncan_bridge` (a C API over the generated C++) and `native/can_socket` (the
   SocketCAN reader).
3. Dart loads it via FFI (`lib/data/services/jsoncan_ffi.dart`, `can_api.dart`). Every refresh, one
   `dimos_jsoncan_read_signals()` call copies the values the UI uses into a `DimosSignals` struct, plus how
   long ago each board's `<BOARD>_Vitals` arrived (for CAN TIMEOUT). The struct is mirrored in Dart, which
   checks its size at load; change `jsoncan_bridge.h` and `jsoncan_ffi.dart` together.

**Build options:**

- **Without cargo:** pass `-DJSONCAN_BINARY_GENERATE=OFF` and put `cppcodegen` on `PATH` (e.g. in a Yocto build).
- **Another car:** give it a `DIMOS` node and configure with `-DDIMOS_CAR=<car>`.

**Building the library on its own** (e.g. for testing):

```bash
cmake -S native -B build/native && cmake --build build/native
```
