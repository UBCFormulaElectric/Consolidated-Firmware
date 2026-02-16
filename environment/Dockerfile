FROM debian:12.9-slim AS builder

# wget: Downloading files from the internet
# unzip: Tool to extract zipped archives
RUN apt update -y && apt install -y xz-utils unzip wget build-essential

ADD "https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz" /usr/local/bin
RUN ls /usr/local/bin && tar -xvf /usr/local/bin/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz -C /

# Install STM32CubeMX for auto-generating code.
#COPY scripts/install_stm32cubemx.py /root/environment/
#COPY data/en.stm32cubemx-lin-v6-12-0.zip /root/environment
#RUN python3 install_stm32cubemx.py --install-dir /usr/local/STM32CubeMX --cube-zip en.stm32cubemx-lin-v6-12-0.zip

ADD "https://github.com/protocolbuffers/protobuf/releases/download/v29.4/protoc-29.4-linux-x86_64.zip" /usr/local/bin
RUN unzip /usr/local/bin/protoc-29.4-linux-x86_64.zip -d /protoc

ENV RUSTUP_HOME=/usr/local/rustup \
    CARGO_HOME=/usr/local/cargo \
    PATH=/usr/local/cargo/bin:$PATH \
    RUST_VERSION=1.93.1
RUN wget --progress=dot:giga "https://static.rust-lang.org/rustup/archive/1.28.2/x86_64-unknown-linux-gnu/rustup-init"; \
    chmod +x rustup-init; \
    ./rustup-init -y --no-modify-path --profile minimal --default-toolchain $RUST_VERSION --default-host x86_64-unknown-linux-gnu; \
    rm rustup-init; \
    chmod -R a+w $RUSTUP_HOME $CARGO_HOME;
ADD scripts/code_generation/jsoncan-rust /jsoncan-rust
WORKDIR /jsoncan-rust
RUN cargo build --release

FROM debian:12.9-slim AS environment

# COPY over builds from builder
COPY --from=builder /arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi /usr/local
COPY --from=builder /protoc/bin/protoc /usr/bin
COPY --from=builder /jsoncan-rust/target/release/cppcodegen /usr/local/bin/cppcodegen
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
RUN apt update -y && apt install -y --no-install-recommends \
    python3 \
    pip \
    git \
#    xvfb \
    libncurses5 \
    valgrind \
    build-essential
#    libgtk-3-0 \
#    libxi6 \
#    libxtst6 \
#    libxrender1 \
#    protobuf-compiler

# Install python package dependencies.
RUN echo "[global]\n break-system-packages = true" > /etc/pip.conf
ADD environment/requirements.txt .
RUN pip3 install -r requirements.txt
RUN rm requirements.txt