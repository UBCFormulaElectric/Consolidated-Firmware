<img src="https://github.com/user-attachments/assets/64d7b3c9-c0af-4a89-bb48-cad103b336c5" alt="Formula E Logo" width="150"/>

# Consolidated-Firmware
A repository for all software and firmware from UBC Formula Electric. [![Binary and DBC Generation](https://github.com/UBCFormulaElectric/Consolidated-Firmware/actions/workflows/master_cd.yml/badge.svg)](https://github.com/UBCFormulaElectric/Consolidated-Firmware/actions/workflows/master_cd.yml)

Brought to you by the students and alumni of the software team at UBC Formula Electric.

<a href="https://github.com/UBCFormulaElectric/Consolidated-Firmware/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=UBCFormulaElectric/Consolidated-Firmware&max=40&columns=20"/>
</a>

## Table of Contents
- [Environment Setup](#environment-setup)
- [Building](#building)
	- [Load CMake](#load-cmake)
	- [Build Embedded Binaries](#build-embedded-binaries)
	- [Build Tests](#build-tests)
- [Debugging](#debugging)
	- [Embedded](#embedded)
	- [Tests](#tests)
- [CAN Bus](#can-bus)
	- [Windows](#windows)
	- [Linux](#linux)
- [Continuous Integration (CI)](#continuous-integration-ci)

## Environment Setup

See our guides on Confluence on how to setup the build system.
- [General Setup Guide](https://ubcformulaelectric.atlassian.net/wiki/x/VwJKCg)
- [Windows Setup Guide](https://ubcformulaelectric.atlassian.net/wiki/x/EwClE)
- [Linux Setup Guide](https://ubcformulaelectric.atlassian.net/wiki/spaces/UFE/pages/169771078/Software+-+Onboarding+-+Week+2+-+Environment+Setup+WSL+Linux)
- [Mac Setup Guide](https://ubcformulaelectric.atlassian.net/wiki/spaces/UFE/pages/168329217/Software+-+Onboarding+-+Week+2+-+Environment+Setup+Mac)

### For those without Confluence Access
- Install CMake.
- Install GNU Arm Embedded Compiler.
- Follow CMake instructions, install dependencies as it asks it from you.

## Building

### Load CMake

CMake is our build system of choice. Whereas editing `CMakeLists.txt` files is more user-friendly, editing the files of your generator of choice (Make, Ninja, MSVC, etc.) is annoying. CMake builds projects compatible with all these generators, and these generators can be used to build the project once the CMake configuration step is complete.

We use 2 CMake profiles, one for embedded binaries and another for unit tests. This is necessary because a specific compiler (`arm-none-eabi-gcc` from the [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm)) is required for building binaries for the ARM Cortex-M microcontrollers that we use. All of our profiles are defined in `CMakePresets.json`. You are welcome to extend these presets with your own custom `CMakeUserPresets.json`.

To load the CMake manually with the command line, run

```
cmake --preset "FW Dev"
```

or

```
cmake --preset "FW Test"
```

for tests.

If you are on VSCode, press `Ctrl+Shift+B` and run "Load CMake: Embedded" and "Load CMake: Tests". This should create a `build_fw_deploy` folder for the embedded CMake output, and `build_fw_test` for the unit test CMake output.

### Build Embedded Binaries

To build binaries for flashing onto boards from the command line,
```
cmake --build build_fw_dev --target <TARGET_NAME_HERE>
```
or alternatively
```
cd <BUILD DIRECTORY OF BINARY>
<GENERATOR_COMMAND>
```

If you are in VSCode, press `Ctrl+Shift+B`. The options ending in `.elf` are the embedded ARM binaries for each board. Run "Build: All Embedded Binaries" to build all boards.

### Build Tests

We use [GoogleTest](https://github.com/google/googletest) as our testing framework.

To build unit tests, similar to above run
```
cmake --build build_fw_test --target <TARGET_NAME_HERE>
```

If you are in VSCode, press `Ctrl+Shift+B`. The options ending in `_test` are the GoogleTest binaries for each board. Run "Build: All Tests" to build all tests.

## Debugging

### Embedded

Connect a debugger to your laptop and the microcontroller's SWD port. Ensure that [JLink Software](https://www.segger.com/downloads/jlink/) is install on your computer.

#### WSL Instructions
If you're developing on Windows, make sure the debugger is attached to WSL. Open `wsl-usb-gui` (Start > Search "WSL USB") and create a "Device" auto-attach profile for the debugger. You should
only have to do this once: If you unplug/replug a debugger it should auto-attach, as long as `wsl-usb-gui` is open (`wsl-usb-gui` must be open!).

Integration with VS Code's step-through debuggers should work out-of-the-box after installing the Cortex-Debug extension. Navigate to the "Run and Debug" menu to start a debugging session.

### Tests

Running and step-through-debugging tests are also available through the "Run and Debug" menu.

We use a script called [fakegen](./scripts/code_generation/fakegen/README.md) to generate fake versions of IO-level code for tests. **Skimming the README is recommended if you're going to be working with unit tests.**

## CAN Bus

Our microcontrollers use CAN bus to pass messages between each other. It is useful to set up your host machine to view
the messages on CAN bus.

### Linux

Tested on Ubuntu 18.04.

PCAN has drivers for Linux and Mac, however they do not provide a GUI to explore the data. Consider the following instructions to sniff the CAN bus.

1. Connect a PCAN dongle between your host machine and the microcontroller.
   The required `socketcan` driver comes with the Ubuntu 18.04 LTS distribution so there is no driver to install.
2. Bring up the CAN interface with the baudrate set to 500kHz.

```
sudo ip link set can0 type can bitrate 500000 && sudo ip link set up can0
```

3. Verify that `can0` shows up in `ifconfig`.
4. Use `candump` and `cantools` to sniff the packets with DBC decoding.

- Display CAN messages on `stdout` with DBC decoding and delta-time as timestamps:

```
candump can0 -c -t d | cantools decode --single-line can_bus/dbcs/quadruna.dbc
```

- Monitor CAN traffic in a text-based user interface:

```
cantools monitor can_bus/dbcs/quadruna.dbc -b socketcan -c can0 -B 500000
```

On Mac:

```
cantools monitor can_bus/dbcs/quadruna.dbc -b pcan -c PCAN_USBBUS1 -B 500000
```

## Continuous Integration (CI)

We run (and require) continuous integration on every pull request before it is merged. This automatically makes sure the
code builds, and checks for formatting errors.
The CI jobs are defined in [github/workflows/ci.yml](./.github/workflows/ci.yml).

1. Build Check: If the code compiles natively, it should also compile in CI.
2. Formatting Check: Run the following commands (from repo root) to fix formatting (CI runs this and then checks if any code was modified):
### Formatting

If you are failing CI because your formatting is wrong, you can use the following command

```sh
python3 scripts/clang_format/fix_formatting.py
```

or on Windows

```
./scripts/clang_format/fix_formatting.ps1
```
