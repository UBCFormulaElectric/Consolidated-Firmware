# Consolidated-Firmware

A repository for all software and firmware from UBC Formula Electric.

## Table of Contents

- [Environment Setup](#environment-setup)
  - [Install Dependencies](#install-dependencies)
  - [WSL Setup (Windows Only)](#wsl-setup-windows-only)
    - [WSL USB Setup](#wsl-usb-setup)
  - [Clone Repo](#clone-repo)
- [Building](#building)
  - [Connect to Container](#connect-to-container)
  - [Configure CMake](#configure-cmake)
  - [Build Embedded Binaries](#build-embedded-binaries)
  - [Build and Run Tests](#build-and-run-tests)
  - [VS Code Integration](#vs-code-integration)
- [Debugging](#debugging)
  - [Embedded](#embedded)
  - [Tests](#tests)
- [CAN Bus](#can-bus)
  - [Windows](#windows)
  - [Linux](#linux)

## Environment Setup

See our guides on Confluence on how to setup the build system.

For more information, and to see how to update the Docker container, see our [Docker README](./environment/README.md).

### Install Dependencies

1. Docker Desktop: Required for running Docker. Available on [Windows](https://docs.docker.com/desktop/install/windows-install/), 
[Linux](https://docs.docker.com/desktop/install/linux-install/), and
[Mac](https://docs.docker.com/desktop/install/mac-install/).
2. [Visual Studio Code](https://code.visualstudio.com/Download): Our IDE of choice. Also install the Remote Development VS Code extension pack (`ms-vscode-remote.vscode-remote-extensionpack`), which is required for connecting to Docker containers.

### WSL Setup (Windows only)

Linux Docker containers run extremely slowly on native Windows, but we can fix this problem by using Windows Subsystem for Linux (WSL). 
With WSL, we can run a Linux distribution on top of Windows, with excellent performance. 

To install WSL, start Windows PowerShell as an administrator (Start > Search for PowerShell > Run as Administrator), and run:

```sh
wsl --install
```

This installs Ubuntu by default, and requires a restart. 
Start WSL after restarting, and you will be prompted up to set up your username and password. After doing so you will get access to a Linux shell.

#### WSL USB Setup

WSL can't access USB devices out-of-the-box.
However, we can use [usbipd](https://github.com/dorssel/usbipd-win) to attach specific USB devices to WSL, which is required for connecting to debuggers.

1. Download and install the latest usbipd [release](https://github.com/dorssel/usbipd-win/releases).
2. From within WSL, run: 
```sh
sudo apt install linux-tools-5.4.0-77-generic hwdata
sudo update-alternatives --install /usr/local/bin/usbip usbip /usr/lib/linux-tools/5.4.0-77-generic/usbip 20
```
3. [wsl-usb-gui](https://gitlab.com/alelec/wsl-usb-gui) is a simple GUI program for attaching USB devices to WSL via usbipd. Install the latest wsl-usb-gui [release](https://gitlab.com/alelec/wsl-usb-gui/-/releases). 

**Note: All following steps must be completed from within WSL!**

### Clone Repo

Navigate to where you want to clone the repo and run:

```sh
# Install Git LFS.
apt update && apt install git-lfs
git lfs install

# Clone repo and update submodules.
git clone <repo link>
cd Consolidated-Firmware
git submodule update --init --recursive
git lfs pull
```

## Building

### Connect to Container

CMake is our build system of choice. It generates Makefiles according to the `CMakeLists.txt` files, which can be used
with `make` to build our binaries.
Makefiles are an extremely thin wrapper around the command line, and so are very annoying to work with, whereas
editing `CMakeLists.txt` files is more user-friendly.

We use 2 CMake profiles, one for embedded binaries and another for unit tests.
This is necessary because a specific compiler (`arm-none-eabi-gcc` from
the [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm)) is required for building binaries for
the ARM Cortex-M microcontrollers that we use.

Press `Ctrl+Shift+B` and run "Load CMake: Embedded" and "Load CMake: Tests". This should create a `build_arm` folder for
the embedded CMake output, and `build_x86` for the unit test CMake output.

Install the following VS Code extensions into the container (should only be required once):
- C/C++ Extension Pack (`ms-vscode.cpptools-extension-pack`)
- Python (`ms-python.python`)
- Cortex-Debug (`marus25.cortex-debug`) 

To build binaries for flashing onto boards, press `Ctrl+Shift+B`.

The options ending in `.elf` are the embedded ARM binaries for each board. Run "Build: All Embedded Binaries" to build
all boards.

We use 2 CMake profiles, one for embedded binaries and another for unit tests. 
This is necessary because a specific compiler (`arm-none-eabi-gcc` from the [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm)) is required for building binaries for the ARM Cortex-M microcontrollers that we use.

Load the embedded and test CMake profiles by running:

We use [GoogleTest](https://github.com/google/googletest) as our testing framework.
To build unit tests, press `Ctrl+Shift+B`.

# Create profile for unit tests.
mkdir build_x86
cd build_x86
cmake .. -DPLATFORM=x86 -DNO_VENV=ON # TODO: Deprecate NO_VENV option
```

### Build Embedded Binaries

To build binaries for flashing onto boards, use Make.

Connect a debugger to your laptop and the microcontroller's SWD port.
For our SEGGER JLink EDU Minis, The correct cable orientation is:

<img src="./images/jlink_connector.png" width="50%" height="50%"/>

If you're developing on Windows, make sure the debugger is attached to WSL.
Open `wsl-usb-gui` (Start > Search "WSL USB") and create a "Device" auto-attach profile for the debugger. You should
only have to do this once: If you unplug/replug a debugger it should auto-attach, as long as `wsl-usb-gui` is
open (`wsl-usb-gui` must be open!).

Integration with VS Code's step-through debuggers should work out-of-the-box after installing the Cortex-Debug
extension. Navigate to the "Run and Debug" menu to start a debugging session.

### Tests

Running and step-through-debugging tests are also available through the "Run and Debug" menu.

We use a script called [fakegen](./scripts/code_generation/fakegen/README.md) to generate fake versions of IO-level code
for tests.
Skimming the README is recommended if you're going to be working with unit tests.

## CAN Bus

Our microcontrollers use CAN bus to pass messages between each other. It is useful to set up your host machine to view
the messages on CAN bus.

### Windows

Use PCAN Explorer. Install on native Windows, since PCAN Explorer isn't available on Linux. See
our [PCAN Explorer Setup Guide](./tools/pcan_explorer/pcan_explorer_6_setup.md).

### Linux

Tested on Ubuntu 18.04.

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
candump can0 -c -t d | cantools decode --single-line can_bus/dbcs/CanMsgs.dbc
```

- Monitor CAN traffic in a text-based user interface:

```
cantools monitor scripts/codegen/CAN/App_CanMsgs.dbc -b socketcan -c can0 -B 500000
```

## Continuous Integration (CI)

We run (and require) continuous integration on every pull request before it is merged. This automatically makes sure the
code builds, and checks for formatting errors.
The CI jobs are defined in [github/workflows/ci.yml](./.github/workflows/ci.yml).

1. Build Check: If the code compiles in CLion, it should also compile in CI.
2. Formatting Check: Run the following commands (from repo root) to fix formatting (CI runs this and then checks if any
   code was modified):

```sh
python3 scripts/clang_format/fix_formatting.py
```
