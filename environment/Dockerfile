FROM ubuntu:22.04

# Install dependencies:
# wget: Downloading files from the internet
# unzip: Tool to extract zipped archives
# pip: For installing python packages
# git: For pulling external repos, invoked via cmake
# git-lfs: Large file storage, used for saving zipped STM32CubeMX installer
# protobuf-compiler: For compiling .proto files into Python or C
# xvfb: Virtual frame buffer service, mocks out a display which doesn't exist on the CI machines
# (required to run STM32CubeMX without a GUI)
# valgrind: Used to check for memory leaks during x86 tests
# clang-format: C/C++ linter, used to check formatting in CI
# libgtk-3-0: Random library required for running STM32CubeMX in container
# libxi6: Random library required for running STM32CubeMX V6 in container
# libxtst6: Random library required for running STM32CubeMX V6 in container
# libxrender1: Random library required for running STM32CubeMX V6 in container
RUN apt update -y && apt install -y \
    wget \
    unzip \
    pip \
    git \
    git-lfs \
    protobuf-compiler \
    xvfb \
    valgrind \
    clang-format \
    libgtk-3-0 \
    libxi6 \
    libxtst6 \
    libxrender1

COPY . /root/environment/

# Install python package dependencies.
WORKDIR /root/environment/scripts
RUN pip3 install -r ../requirements.txt

# Install GNU ARM Embedded Toolchain, for compiling/debugging on embedded.
RUN /bin/sh install_gcc_arm_none_eabi.sh /usr/local

# Install STM32CubeMX for auto-generating code.
RUN python3 install_stm32cubemx.py --install-dir /usr/local/STM32CubeMX --cube-zip ../data/en.stm32cubemx-lin-v6-9-2.zip

# Cleanup copied directory.
WORKDIR /root/
RUN rm -rf environment
