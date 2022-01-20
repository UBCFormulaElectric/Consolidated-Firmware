# Consolidated-Firmware
A consolidated repository for gathering all firmware under one roof.

## Table of Content
- [Project Setup](#project-setup)
  - [Environment Dependencies](#environment-dependencies)
  - [Python Package Dependencies](#python-package-dependencies)
  - [STM32CubeMX](#stm32cubemx)
  - [CLion](#clion)
- [Continuous Integration (CI)](#continuous-integration-ci)
    - [CommentPragmas](#commentpragmas)
- [Conventions](#conventions)
  - [Github Conventions](#github-conventions)
  - [Pull Requests Conventions](#pull-requests-conventions)
  - [Coding Conventions](#coding-conventions)
- [CAN Bus](#can-bus)
  - [Ubuntu 18.04](#ubuntu-1804)
- [Periodic Task Scheduling](#periodic-task-scheduling)
- [CMakeLists](#cmakelists)
- [FAQ](#faq)

## Project Setup
### Environment Dependencies
Follow these steps to compile and debug code in **CLion**:

(**Note**: Before you can build the project you should setup your own [fork](#conventions) of the repo)
#### 1. Install Dependencies:
##### Ubuntu 18.04
You need to install `git lfs` following the instructions here: https://git-lfs.github.com/. Don't forget to run `git lfs install` afterwards.

Then, `cd` to the root directory of the repo and run the following commands:
```
pipenv install
pipenv shell
git lfs pull
scripts/environment_setup/install_gcc_arm_none_eabi.sh /usr/local
python scripts/environment_setup/install_cube.py /usr/local/STM32CubeMX ./tools/en.STM32CubeMX_v5-3-0.zip
sudo apt-get install openocd
```
##### Windows
  * GNU Make: http://gnuwin32.sourceforge.net/packages/make.htm
  * CMake: https://cmake.org/install/
  * Python 3+ (*Python < 3 will NOT work*): https://www.python.org/downloads/
  * ARM GNU Embedded Toolchain: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads (Check `install_gcc_arm_none_eabi.sh` for which version to download)
  * STM32CubeMX: https://www.st.com/en/development-tools/stm32cubemx.html
  * MinGW: https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download (**select 32-bit verison/i686 architecture**)
  * Java: https://www.java.com/download/ie_manual.jsp

##### Mac OS
First, install Homebrew and CLion. Then install the following programs using Homebrew:
```
brew install git-lfs pipenv openocd gcc
brew install --cask clion
```
Verify the programs above were installed by running `<program name> --version`.
Now, we need to install the `arm-none-eabi` compiler toolchain for ARM processors. To avoid bugs associated with different versions, go to the [ARM website](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) and download the *9-2019-q4-major* version (link [here](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads#:~:text=gcc-arm-none-eabi-9-2019-q4-major-mac.tar.bz2)).

Then, set up the repo:
```
git lfs install
git clone <repo link>
cd Consolidated-Firmware
pipenv install
pipenv shell
git lfs pull
```
Install cube:

`python3 scripts/environment_setup/install_cube.py /usr/local/STM32CubeMX ./tools/en.STM32CubeMX_v5-3-0.zip`

##### All Operating Systems
  * J-Link Software and Documentation Pack: https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack
    ##### Submodules:
    In the root of the directory run the following command:
    ```
    git submodule update --init --recursive
    ```
    to fetch all of the submodules.

#### 2. Modify your `PATH` Environment Variable:
##### Ubuntu 18.04 and Mac OS
Modify the `PATH` variable by adding the following to your `.bashrc`:
```
export PATH="$PATH:/path/to/arm-none-eabi-gcc"
export PATH="$PATH:/path/to/STM32CubeMX"
```

Always invoke `clion` from the command line. That way `clion` will load `.bashrc` and read the modifications you made the the `PATH` variable.

##### Windows
Find `Environment Variables` in your start menu and then add the appropriate paths to `PATH`:
```
C:\Program Files (x86)\GNU Tools Arm Embedded\<VERSION>\bin
C:\Program Files (x86)\GnuWin32\bin
C:\Program FIles (x86)\...\path\to\STM32CubeMX
...
```

These paths should include wherever the `cmake`, `make`, `arm-none-eabi-gcc`, and STM32CubeMX binaries have been installed.

Create a new enviroment variable under your user variables named `PYTHONPATH`. Set the value of the variable to path for the root directory of your fork:
```
C:\path\to\Consolidated-Firmware-Username
```

### Python Package Dependencies
We use python both for CI (see below), and to generate C code from the `.dbc` defining messages passed over CAN. Python dependencies are managed via [pipenv](https://pipenv.readthedocs.io/en/latest/). To install all required dependencies in a [python virtual environment](https://realpython.com/python-virtual-environments-a-primer/), navigate to the root of this repository and run `pipenv install`.

### STM32CubeMX
**Note: Do not migrate project when opening .ioc file. Newer versions of FreeRTOS (>9) are incompatible with Segger SysView.**

### CLion
We edit, compile, and debug our code using [CLion](https://www.jetbrains.com/clion/). Students can obtain a CLion educational license [here](https://www.jetbrains.com/shop/eform/students). Open the `boards` directory in CLion. To compile this CLion project , you must **manually** add CMake profiles by going to: `Settings -> Build, Execution, Deployment -> CMake`. Click on the `+` to add two profiles with the following settings:

```
// First CMake profile:
Name: embedded
Build type: Debug
CMake options: -DPLATFORM=arm

// Second CMake profile:
Name: gtest
Build type: Debug
CMake options: -DPLATFORM=x86
```

In each project, there will be two configurations to use: `<board>_SeggerGDB.elf` and `OCD <board>`. Either one can be used for flashing and debugging, but the `<board>_SeggerGDB.elf` has unlimited flash breakpoints among some other extra functionalities. Use `<board>_SeggerGDB.elf` whenever possible.

##### Windows
Under **File->Settings->Build, Execution, Deployment...->Toolchains**:
Set the default toolchain to be MinGW and provide the file path to the 32-bit version of MinGW, ie:
```
C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32
```
Next, set the debugger to be ARM GDB, ie:
```
C:\Program Files (x86)\GNU Tools ARM Embedded\8 2019-q3-update\bin\arm-none-eabi-gdb.exe
```

For each project under **Run->Edit Configurations->Embedded GDB Server**, select `<board_name>_SeggerGDB`:
Then set the GDB Server to be JLinkGDBServer, ie:
```
C:\Program Files (x86)\SEGGER\JLink\JLinkGDBServer.exe
```

##### Mac OS
Under **File->Settings->Build, Execution, Deployment...->Toolchains**:
Set the default C compiler to be the GNU C and C++ compilers you installed earlier. If you installed `gcc` using `brew` your C/C++ compiler paths should be something similar to the following:
- C compiler: `/usr/local/Cellar/gcc/<some version number>/bin/gcc-11`
- C++ compiler: `/usr/local/Cellar/gcc/<some version number>/bin/g++-11`

Set the debugger to be `arm-none-eabi-gdb`, ie:

Debugger: `/path/to/arm-none-eabi-gcc/bin/arm-none-eabi-gdb`

Note: whenever you get an error saying *"`<program name>` cannot be opened because the developer cannot be verified"*, go to **System Preferences --> Security & Privacy --> General**, and click **Allow Anyway** next to the part saying *"`<program name>` was blocked from use because it is not from an identified developer"*.

## Continuous Integration (CI)
We run (and require) continuous integration on every pull request before it is merged. This automatically makes sure the code builds, and checks for formatting errors.

1. **Build Check**: If the code compiles in CLion, it should also compile in CI.
2. **Formatting Check**: Run the following commands (starting from the **root directory** of this project) to fix formatting (CI runs this and then checks if any code was modified):
  * *All Operating Systems:*
  ```
  python clang-format/fix_formatting.py
  ```

#### CommentPragmas
In `.clang-format`, the line `CommentPragmas: '\/\*(\*(?!\/_|[^*])*\*\/'` is ineffective because it's being preempted by `ReflowComments`. We are hoping that the next version of `clang-format` will resolve this.

## Conventions

### Github Conventions
- We follow the Forking Workflow:
    - [what it is](https://www.atlassian.com/git/tutorials/comparing-workflows#forking-workflow)
    - [how to use it](https://gist.github.com/Chaser324/ce0505fbed06b947d962)
- Only commit files that are essential for the system to run; do not put any photos or videos in here.
- Once your pull request has been reviewed and revised until it looks good from both your and the reviewers' sides, go ahead and Squash and Merge it, which will squash all the commits on your pull request into one and merge it to the target branch.

### Pull Requests Conventions
- Before opening a pull request, quickly run through the [pull request checklist](.github/pull_request_template.md) and make sure you've satisfied everything.
- Once your pull request has been approved, please proceed to merge and close the pull request yourself.
- When your pull requests receive comments, please reply to each comment individually.
- If there were any fixes that were specific to resources you found (eg. stackoverflow thread), please comment them into the PR for future reference.
- On a similar note, if you made design decisions, please document them in the comments of the PR. We often go back to close PRs to look at why things were done a certain way. It is very helpful for us to know how you came up with your solution when reading through the PR.

### Coding Conventions
- Source files should roughly look like
```
{includes}
{macros, structs, enums, global variables, etc}
{static function declarations}
{static function definitions}
{public function definitions}

```
- Headers should roughly look like
```
{javadoc}
{#pragma once}
{includes}
{macros, structs, enums, global variables, etc}
{public function declarations}
```

- Function comments (below) should be directly above every function in the **headers**:
```
/**
 * Description of the purpose of the function
 * @param param_one The first parameter of the function
 * @param param_two The second parameter of the function whose description goes
 *                  longer than one line
 * @return What the function returns (not applicable if it returns void)
 */
```

- Variables are **non_camel_case**
- Constants are **ALL_CAPS_WITH_UNDERSCORES**
- Structs, Enums, Unions are **PascalCase**
- Filenames are **PascalCase** and follow the format **{AbstractionPrefix}_{Name}**
  - **{AbstractionPrefix}** may be **App** for application code or **Io** for hardware-specific code
  - **{Name}** is a descriptive name
  - An example would be **Io_Can.h**, which describes CAN hardware drivers
- Functions are **PascalCase** and follow the the format **{HeaderName}_{FunctionName}**
  - An example would be **struct CanMaskFilterConfig Io_Can_GetCanMaskFilters(void)**
- Static Functions are **PascalCase** and don't require a **{HeaderName}** prefix

- Indentations are 4 spaces

- Do **not** typedef structs and enums unless you have a good reason to make an opaque type

## CAN Bus
Our microcontrollers use CAN bus to pass messages between each other. It is useful to set up your host machine to view the messages on CAN bus.
### Ubuntu 18.04
1. Connect a PCAN dongle between your host machine and the microcontroller. The required `socketcan` driver comes with the Ubuntu 18.04 LTS distribution so there is no driver to install.
2. Bring up the CAN interface with the baudrate set to 500kHz.
```
sudo ip link set can0 type can bitrate 500000 && sudo ip link set up can0
```
3. Verify that `can0` shows up in `ifconfig`.
4. Use `candump` and `cantools` to sniff the packets with DBC decoding.
- Display CAN messages on `stdout` with DBC decoding and delta-time as timestamps:
```
candump can0 -c -t d | cantools decode --single-line scripts/codegen/CAN/App_CanMsgs.dbc
```
- Monitor CAN traffic in a text-based user interface:
```
cantools monitor scripts/codegen/CAN/App_CanMsgs.dbc -b socketcan -c can0 -B 500000
```
## Periodic Task Scheduling
We follow rate-monotonic scheduling to assign priorities to periodic tasks. This priority-assignment algorithm rules that every periodic task must have higher priority than other periodic tasks with longer cycle duration than itself. For example, if we have a 1Hz task and a 10Hz task, the 10Hz task must have a higher priority than the 1Hz task. The exact priority value isn't important, as long as the relative priorities follow rate-monotonic scheduling.

## CMakeLists
- Each board project (e.g. PDM, FSM) shall have its `CMakeLists.txt` and linker directives generated by a **STM32CubeMX** project in **CLion**. Here is a guide: https://blog.jetbrains.com/clion/2019/02/clion-2019-1-eap-clion-for-embedded-development-part-iii/.

## FAQ
### What is the correct orientation for plugging in the J-Link mini connector?
The red marking on the ribbon cable indicates pin 1. Align the red marking with the pin 1 marker next to the J-Link mini connector.

<img src="https://github.com/UBCFormulaElectric/Consolidated-Firmware/blob/master/images/jlink_connector.png" width="50%" height="50%"/>
