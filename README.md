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

We write, build, and debug code from inside of an Ubuntu 22.04 Docker container, which is effectively a lightweight virtual machine running on top of your native OS.
This significantly simplifies environment setup, since the container bundles all the dependencies required for development, and is isolated from any software a member might already have on their computer.
The container uses [Docker Volumes](https://docs.docker.com/storage/volumes/) such  any changes made to the repo from inside the container will be mirrored to your native filesystem, and vice-versa.

For more information, and to see how to update the Docker container, see our [Docker README](./environment/README.md).

### Install Dependencies

1. Docker Desktop: Required for running Docker. Available on [Windows](https://docs.docker.com/desktop/install/windows-install/), 
[Linux](https://docs.docker.com/desktop/install/linux-install/), and
[Mac](https://docs.docker.com/desktop/install/mac-install/). Some people have had issues with this on Ubuntu, so please follow the instructions carefully!
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
sudo apt install linux-tools-generic hwdata
sudo update-alternatives --install /usr/local/bin/usbip usbip /usr/lib/linux-tools/*-generic/usbip 20
```
These commands are sourced from [here](https://learn.microsoft.com/en-us/windows/wsl/connect-usb).
3. [wsl-usb-gui](https://gitlab.com/alelec/wsl-usb-gui) is a simple GUI program for attaching USB devices to WSL via usbipd. Install the latest wsl-usb-gui [release](https://gitlab.com/alelec/wsl-usb-gui/-/releases) (the `.msi`). 

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

To start the development Docker container, navigate to the repo root and run: 

```sh
docker compose up --detach
```

If you get an error about failing to mount `/tmp`, you may need to add it as a mountable directory in Docker Desktop.
Open Docker Desktop > Settings > Resources > File Sharing > Add `/tmp`.

From VS Code, click the arrows in the bottom left and select "Attach to Running Container". 
Select the `consolidated-firmware-develop` option and VS Code should connect you to the container. From here, you can open the repo and build code from VS Code's integrated terminal.

Install the following VS Code extensions into the container (should only be required once):
- C/C++ Extension Pack (`ms-vscode.cpptools-extension-pack`)
- Python (`ms-python.python`)
- Cortex-Debug (`marus25.cortex-debug`) 

When you're finished developing and want to stop the container, run this from the repo root:
```sh
docker compose down
```

### Load CMake

We use 2 CMake profiles, one for embedded binaries and another for unit tests. 
This is necessary because a specific compiler (`arm-none-eabi-gcc` from the [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/gnu-rm)) is required for building binaries for the ARM Cortex-M microcontrollers that we use.

Load the embedded and test CMake profiles by running:

```sh
# Create profile for embedded.
mkdir build_arm
cd build_arm
cmake .. -DPLATFORM=arm -DNO_VENV=ON # TODO: Deprecate NO_VENV option
cd ..

# Create profile for unit tests.
mkdir build_x86
cd build_x86
cmake .. -DPLATFORM=x86 -DNO_VENV=ON # TODO: Deprecate NO_VENV option
```

### Build Embedded Binaries

To build binaries for flashing onto boards, use Make.

```sh
cd build_arm

# Build binary for single board:
make <board>.elf # Example: make BMS.elf
 
# Build all ARM binaries:
make

# Note: Pass -j10 to `make` to use multiple threads to speed up builds (glitchy when building multiple targets).
```
### Build and Run Tests

To build unit test binaries, use Make.

```sh
cd build_x86

# Build tests for single board:
make <board>_test # Example: make BMS_test

# Run tests for single board:
./firmware/thruna/<board>/<board>_test # Example: ./firmware/thruna/BMS/BMS_test

# Build all tests:
make

# Run all tests:
ctest

# Note: Pass -j10 to `make` to use multiple threads to speed up builds (glitchy when building multiple targets).
```

### VS Code Integration

All of the above are available as VS Code "Build Tasks." 
Press `Ctrl-Shift-B` to bring up the "Build Tasks" menu to load CMake and build.

## Debugging

### Embedded

Connect a debugger to your laptop and the microcontroller's SWD port. The correct cable orientation is: 

<img src="./images/jlink_connector.png" width="50%" height="50%"/>

If you're developing on Windows, make sure the debugger is attached to WSL. 
Open `wsl-usb-gui` (Start > Search "WSL USB") and create a "Device" auto-attach profile for the debugger. You should only have to do this once: If you unplug/replug a debugger it should auto-attach, as long as `wsl-usb-gui` is open.

Integration with VS Code's step-through debuggers should work out-of-the-box after installing the Cortex-Debug extension. Navigate to the "Run and Debug" menu to start a debugging session.

### Tests

Running and step-through-debugging tests are also available through the "Run and Debug" menu.

## CAN Bus

Our microcontrollers use CAN bus to pass messages between each other. It is useful to set up your host machine to view the messages on CAN bus.

### Windows

Use PCAN Explorer. Install on native Windows, since PCAN Explorer isn't available on Linux. See our [PCAN Explorer Setup Guide](./tools/pcan_explorer/pcan_explorer_6_setup.md).

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

We run (and require) continuous integration on every pull request before it is merged. This automatically makes sure the code builds, and checks for formatting errors.
The CI jobs are defined in [github/workflows/ci.yml](./.github/workflows/ci.yml).

1. Build Check: If the code compiles in CLion, it should also compile in CI.
2. Formatting Check: Run the following commands (from repo root) to fix formatting (CI runs this and then checks if any code was modified):
```sh
python3 scripts/clang_format/fix_formatting.py
```
