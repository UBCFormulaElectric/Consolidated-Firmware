FROM debian:12.9-slim AS builder

# wget: Downloading files from the internet
# unzip: Tool to extract zipped archives
RUN apt update -y && apt install -y bzip2

ADD "https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2?rev=78196d3461ba4c9089a67b5f33edf82a&hash=D484B37FF37D6FC3597EBE2877FB666A41D5253B" /usr/local/bin
RUN tar -xvf /usr/local/bin/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2 -C /usr/local/bin --strip=1

RUN ls /usr/local/bin

# Install STM32CubeMX for auto-generating code.
#COPY scripts/install_stm32cubemx.py /root/environment/
#COPY data/en.stm32cubemx-lin-v6-12-0.zip /root/environment
#RUN python3 install_stm32cubemx.py --install-dir /usr/local/STM32CubeMX --cube-zip en.stm32cubemx-lin-v6-12-0.zip

FROM debian:12.9-slim AS environment

# COPY over builds from builder
COPY --from=builder /usr/local/bin/arm-none-eabi/ /usr/local/bin/arm-none-eabi/
#COPY --from=builder /usr/local/STM32CubeMX /usr/local/STM32CubeMX

# pip: For installing python packages
# git: For pulling external repos, invoked via cmake
# git-lfs: Large file storage, used for saving zipped STM32CubeMX installer
# xvfb: Virtual frame buffer service, mocks out a display which doesn't exist on the CI machines
# (required to run STM32CubeMX without a GUI)
# libncurses5: Library required by clang-format
# valgrind: Used to check for memory leaks during x86 tests
# libgtk-3-0: Random library required for running STM32CubeMX in container
# libxi6: Random library required for running STM32CubeMX V6 in container
# libxtst6: Random library required for running STM32CubeMX V6 in container
# libxrender1: Random library required for running STM32CubeMX V6 in container
RUN apt update -y && apt install -y \
    pip \
    git \
#    xvfb \
    libncurses5 \
    valgrind \
#    libgtk-3-0 \
#    libxi6 \
#    libxtst6 \
#    libxrender1 \
    protobuf-compiler

# Install python package dependencies.
RUN echo "[global]\n break-system-packages = true" > /etc/pip.conf
COPY requirements.txt .
RUN pip3 install -r requirements.txt
RUN rm requirements.txt