# Base image is Ubuntu Linux 23.04.
FROM ubuntu:23.04

# Install base dependencies.
RUN apt-get update -y && apt-get -y install
RUN apt-get install -y git wget unzip

# Install C/C++ tools.
RUN apt-get install -y cmake gcc gdb libncurses5
# Note: clang-format requires libncurses5

# Install Java 11, which is required by STM32CubeMX.
RUN apt-get install -y openjdk-11-jdk

# Install OpenOCD, used for flashing and debugging embedded targets.
RUN apt-get install -y openocd

# Copy over environment setup scripts.
WORKDIR /usr/src/environment
COPY . .

# Hack to get around "externally managed environment" error blocking me from installing pip packages system-wide.
# https://stackoverflow.com/questions/75608323/how-do-i-solve-error-externally-managed-environment-everytime-i-use-pip3
ENV PIP_BREAK_SYSTEM_PACKAGES 1

# Install Python Pip packages.
RUN apt-get install -y python3-pip
RUN pip install -r requirements.txt

# Install ARM embedded toolchain, used to compile binaries for embedded targets.
RUN wget "https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/RC2.1/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2?revision=6e63531f-8cb1-40b9-bbfc-8a57cdfc01b4&la=en&hash=F761343D43A0587E8AC0925B723C04DBFB848339" \
    -O "gcc-arm-none-eabi.XXXXXXXX.tar.bz2"
RUN tar -xvf "gcc-arm-none-eabi.XXXXXXXX.tar.bz2" -C /usr/local/bin
RUN echo 'export PATH="$PATH:/usr/local/bin/gcc-arm-none-eabi-9-2019-q4-major/bin"' >> ~/.bashrc

# Install and unpack STM32CubeMX for automatically generating driver code for STM32 microcontrollers.
RUN python3 install_cube.py /usr/local/STM32CubeMX data/en.STM32CubeMX_v5-3-0.zip

# Remove environment setup scripts now that setup is complete.
WORKDIR /usr/src/
RUN rm -rf environment
