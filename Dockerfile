# Base image is Ubuntu Linux
FROM ubuntu:23.04
ARG DEBIAN_FRONTEND=noninteractive

# Install misc. dependencies
RUN apt-get update -y && apt-get install -y \
    git \
    wget \
    unzip \
    cmake \
    gcc \
    gdb \
    libncurses5 \
    openjdk-11-jdk \
    python3 \
    python3-pip \
    openocd

# Note: clang-format fails without installing libncurses5
# Note 2: STM32CubeMX requires Java 11

# Copy Consolidated-Firmware repository temporarily for running installation scripts
COPY . /usr/src/setup/Consolidated-Firmware
WORKDIR /usr/src/setup/Consolidated-Firmware

# # Install Python packages
# RUN pip install -r requirements.txt

# Install ARM toolchain, for compiling for ARM
RUN scripts/environment_setup/install_gcc_arm_none_eabi.sh /usr/local

# Install and unpack STM32CubeMX for automatically generating HAL drivers
RUN python3 scripts/environment_setup/install_cube.py /usr/local/STM32CubeMX tools/en.STM32CubeMX_v5-3-0.zip

# Remove temporary repo installation directory
WORKDIR /usr/src/
RUN rm -rf setup
