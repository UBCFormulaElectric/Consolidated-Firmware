# Consolidated-Firmware
A consolidated repository for gathering all firmware under one roof.

## Table of Contents
- [Project Setup](#project-setup)
  - [Environment Dependencies](#environment-dependencies)
  - [Python Package Dependencies](#python-package-dependencies)
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
### Ubuntu 18.04
* Install git by running `sudo apt install git`
* Install `git-lfs` by following the instructions here: https://git-lfs.github.com/. Don't forget to run `git lfs install` afterwards.
* Install python3 Python 3+ (*Python < 3 will NOT work*) by running `sudo apt install python3`
* Install pipenv by running `sudo pip3 install pipenv`
* Open `terminal`, navigate to the folder you want to store this repository in, then run: 
```
git clone https://github.com/UBCFormulaElectric/Consolidated-Firmware.git
cd Consolidated-Firmware
git lfs pull
```
* Install CubeIde by going to `Consolidated-Firmware/tools` and running `./st-stm32cubeide_1.2.0_5034_20200108_0926_amd64.sh`

### Windows
1) Install git by following the instructions [here](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git)
* Install `git-lfs` by following the instructions here: https://git-lfs.github.com/. Don't forget to run `git lfs install` afterwards.
* Install python3 Python 3+ (*Python < 3 will NOT work*) from [here](https://www.python.org/downloads/)
* Open `Python Shell` and run `pip install pipenv`
* Open `git bash`, navigate to the folder you want to store this repository in, then run: 
```
git clone https://github.com/UBCFormulaElectric/Consolidated-Firmware.git
cd Consolidated-Firmware
git lfs pull
```
* Install CubeIDE *1.2.0* from [here](https://www.st.com/en/development-tools/stm32cubeide.html)

## Continuous Integration (CI)
We run (and require) continuous integration on every pull request before it is merged. This automatically makes sure the code builds, and checks for formatting errors.

1. **Build Check**: If the code compiles in CubeIDE, it should also compile in CI.
2. **Formatting Check**: Run the following commands (starting from the **root directory** of this project) to fix formatting (CI runs this and then checks if any code was modified):
  * *Windows and Ubuntu 18.04:*
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
### Ubuntu 18.04
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
cantools monitor boards/shared/CAN/CanMsgs.dbc -b socketcan -c can0 -B 500000
```
## Periodic Task Scheduling
We follow rate-monotonic scheduling to assign priorities to periodic tasks. This priority-assignment algorithm rules that every periodic task must have higher priority than other periodic tasks with longer cycle duration than itself. For example, if we have a 1Hz task and a 10Hz task, the 10Hz task must have a higher priority than the 1Hz task. The exact priority value isn't important, as long as the relative priorities follow rate-monotonic scheduling.

It is conventional for the frequency of periodic tasks in embedded systems to be a power of 10. For us, we should only ever need to use 1Hz, 10Hz, 100Hz, and 1kHz. Here's a table you can follow to assign priority values for periodic tasks in CMSIS-RTOS:

| Frequency     | Priority              |
| ------------- | --------------------- |
| 1Hz           | osPriorityLow         |
| 10Hz          | osPriorityBelowNormal |
| 100Hz         | osPriorityNormal      |
| 1000Hz        | osPriorityAboveNormal |

## CMakeLists
- Each board project (e.g. PDM, FSM) shall have its `CMakeLists.txt` and linker directives generated by a **STM32CubeMX** project in **CLion**. Here is a guide: https://blog.jetbrains.com/clion/2019/02/clion-2019-1-eap-clion-for-embedded-development-part-iii/.
- If applicable, compile a static library (e.g. `.a` file) and place the code and `CMakeLists.txt` under `lib/`. The developer is responsible for writing their own `CMakeLists.txt` for the static library.

## FAQ
### What is the correct orientation for plugging in the J-Link mini connector?
The red marking on the ribbon cable indicates pin 1. Align the red marking with the pin 1 marker next to the J-Link mini connector.

<img src="https://github.com/UBCFormulaElectric/Consolidated-Firmware/blob/master/images/jlink_connector.png" width="50%" height="50%"/>
