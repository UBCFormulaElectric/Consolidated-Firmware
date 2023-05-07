FROM ubuntu:23.04

# Install dependencies
RUN apt-get update -y && apt-get -y install
RUN apt-get install -y git wget unzip

# Install Python and Python packages
RUN apt-get install -y python3.11 python3-pip pipenv
RUN pipenv install

# Install C/C++ tools
RUN apt-get install -y cmake gcc gdb libncurses5
# Note: clang-format fails without installing libncurses5

# Install Java 11, required by STM32CubeMX
RUN apt-get install -y openjdk-11-jdk
    
# Copy Consolidated-Firmware repository temporarily for running installation scripts
COPY . /usr/src/setup/Consolidated-Firmware
WORKDIR /usr/src/setup/Consolidated-Firmware

# Install ARM toolchain, for compiling for ARM
RUN scripts/environment_setup/install_gcc_arm_none_eabi.sh /usr/local

# Install and unpack STM32CubeMX for automatically generating drivers
RUN python3 scripts/environment_setup/install_cube.py /usr/local/STM32CubeMX tools/en.STM32CubeMX_v5-3-0.zip

# Install OpenOCD for embedded debugging
RUN apt-get install -y openocd

# Remove temporary repo installation directory
WORKDIR /usr/src/
RUN rm -rf setup
