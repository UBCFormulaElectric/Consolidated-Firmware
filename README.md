# Consolidated-Firmware
A consolidated repository for gathering all firmware under one roof. 

## Project Setup

### Integrated Development Environment (IDE)
We edit, compile, and debug our code using [CLion](https://www.jetbrains.com/clion/).
Students can obtain a CLion educational license [here](https://www.jetbrains.com/shop/eform/students).
Open any one of the folders under `boards/` (e.g. `boards/DCM`) in **CLion**.

### Environment Dependencies
Follow these steps so you can compile the code in **CLion**:
1. **Install Dependencies**: There are several dependencies required in order to mimic what CI is doing. *(Note: All links below are for windows. All equivalent linux utilities may be easily installed via the package manager for your linux distribution (`apt-get` in Ubuntu, etc.) or using the scripts found under `scripts/environment_setup/`)*
  * GNU Make: http://gnuwin32.sourceforge.net/packages/make.htm
  * CMake: https://cmake.org/install/
  * Python 3+ (*Python < 3 will NOT work*): https://www.python.org/downloads/
  * ARM GNU Embedded Toolchain: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads (Check `install_gcc_arm_none_eabi.sh` for which version to download)
  *  J-Link Software and Documentation Pack: https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack
2. **Modify your `PATH` Environment Variable**: Make sure to add the binary executables to `PATH`. To add these, find `Environment Variables` in your start menu and then add the appropriate paths to `PATH`:
```
C:\Program Files (x86)\GNU Tools Arm Embedded\<VERSION>\bin
C:\Program Files (x86)\GnuWin32\bin
...
```
(*These paths should include wherever the `cmake`, `make`, and `GCC` binaries have been installed.*)

#### Python Package Dependencies
We use python both for CI (see below), and to generate C code from the `.dbc` defining messages passed over CAN. Python dependencies are managed via [pipenv](https://pipenv.readthedocs.io/en/latest/). To install all required dependencies in a [python virtual environment](https://realpython.com/python-virtual-environments-a-primer/), navigate to the root of this repository and run `pipenv install`.

## Continuous Integration (CI)
We run (and require) continuous integration on every pull request before it is merged. This automatically makes sure the code builds, and checks for formatting errors.

1. **Build Check**: If the code compiles in CLion, it should also compile in CI.
2. **Formatting Check**: Run the following commands (starting from the **root directory** of this project) to fix formatting (CI runs this and then checks if any code was modified):
  * *Windows and Linux:*
  ```
  python clang_format/fix_formatting.py
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
- Every and **.h** file should start with
```
/**
 * @brief A quick description of what this file does/is for
 */
```

- Functions comments (below) should be directly above every function in the **header** file:
```
/**
 * @brief A quick descrption
 * @param param_one The first parameter of the function
 * @param param_two The second parameter of the function whose
 *                  description goes longer than one line
 * @return what the function returns if it returns anything
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
### Linux (Tested on *Ubuntu 18.04 LTS*)
1. Connect a PCAN dongle between your host machine and the microcontroller. The required `socketcan` driver comes with the Ubuntu 18.04 LTS distribution so there is no driver to install.
2. Bring up the CAN interface with the baudrate set to 500kHz.
```
sudo ip link set can0 type can bitrate 500000
sudo ip link set up can0
```
3. Verify that `can0` shows up in `ifconfig`.
4. Use `candump` and `cantools` to sniff the packets with DBC decoding.
- Display CAN messages on `stdout` with DBC decoding and delta-time as timestamps:
```
candump can0 -c -t d | cantools decode --single-line boards/shared/CanMsgs/CanMsgs.dbc
```
- Monitor CAN traffic in a text-based user interface:
```
cantools monitor boards/shared/CanMsgs/CanMsgs.dbc -b socketcan -c can0 -b 500000
```

## CMakeLists
- Each board project (e.g. PDM, FSM) shall have its `CMakeLists.txt` and linker directives generated by a **STM32CubeMX** project in **CLion**. Here is a guide: https://blog.jetbrains.com/clion/2019/02/clion-2019-1-eap-clion-for-embedded-development-part-iii/.
- If applicable, compile a static library (e.g. `.a` file) and place the code and `CMakeLists.txt` under `lib/`. The developer is responsible for writing their own `CMakeLists.txt` for the static library.
