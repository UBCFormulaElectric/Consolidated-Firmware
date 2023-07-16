FROM ubuntu:22.04

# Install dependencies:
# wget: Downloading files from the internet
# unzip: Tool to extract zipped archives
# pip: For installing python packages
# git: For pulling external repos, invoked via cmake
# git-lfs: Large file storage, used for saving zipped STM32CubeMX installer
# openjdk-11-jdk: Java, required to install and run STM32CubeMX
# xvfb: Virtual frame buffer service, mocks out a display which doesn't exist on the CI machines
# (required to run STM32CubeMX without a GUI)
# libncurses5: Library required by clang-format
# valgrind: Used to check for memory leaks during x86 tests
RUN apt update -y && apt install -y \
    wget \
    unzip \
    pip \
    git \
    git-lfs \
    openjdk-11-jdk \
    xvfb \
    libncurses5 \
    valgrind 

# Copy over environment directory for running installation scripts.
COPY . /home/environment/
WORKDIR /home/environment/scripts

# Install python package dependencies.
RUN pip3 install -r ../requirements.txt

# Install GNU ARM Embedded Toolchain, for compiling/debugging on embedded.
RUN /bin/sh install_gcc_arm_none_eabi.sh /usr/local

# Install STM32CubeMX for auto-generating code.
RUN python3 install_stm32cubemx.py --install-dir /usr/local/STM32CubeMX --cube-zip ../data/en.STM32CubeMX_v5-3-0.zip

# Cleanup copied directory.
WORKDIR /home/
RUN rm -rf environment
