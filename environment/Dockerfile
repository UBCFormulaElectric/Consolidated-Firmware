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
# openocd: Our embedded GDB server of choice
# usbutils: USB tools (lsusb)
RUN apt update -y && apt install -y \
    wget \
    unzip \
    pip \
    git \
    git-lfs \
    xvfb \
    libncurses5 \
    valgrind \

COPY . /home/environment/

# Install SEGGER JLink tools.
WORKDIR /home/environment/data
RUN mkdir /opt/SEGGER
RUN tar -xf JLink_Linux_V792c_x86_64.tgz -C /opt/SEGGER

# Install python package dependencies.
WORKDIR /home/environment/scripts
RUN pip3 install -r ../requirements.txt

# Install GNU ARM Embedded Toolchain, for compiling/debugging on embedded.
RUN /bin/sh install_gcc_arm_none_eabi.sh /usr/local

# Install STM32CubeMX for auto-generating code.
RUN python3 install_stm32cubemx.py --install-dir /usr/local/STM32CubeMX --cube-zip ../data/en.STM32CubeMX_v5-3-0.zip

# Cleanup copied directory.
WORKDIR /root/
RUN rm -rf environment

# Environment variable "force_color_prompt" needs to be set to "yes" to get colored output in shell.
# Hacky fix since we currently don't volume the bashrc, append the line "force_color_prompt=yes" to the top of root's bashrc.
RUN sed -i '1s/^/force_color_prompt=yes\n\n/' /root/.bashrc
