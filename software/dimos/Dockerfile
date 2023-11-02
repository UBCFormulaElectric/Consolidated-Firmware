ARG BASE_VERSION=3.0.8
ARG SDK_BASE_VERSION=3.0.8
ARG APP_EXECUTABLE=dimos
ARG IMAGE_ARCH=arm64
ARG GPU=-vivante

# BUILD ------------------------------------------------------------------------
FROM --platform=linux/${IMAGE_ARCH} \
    torizon/wayland-base${GPU}:${SDK_BASE_VERSION} AS Build

ARG IMAGE_ARCH
ARG GPU

# stick to bookworm on /etc/apt/sources.list.d
RUN sed -i 's/sid/bookworm/g' /etc/apt/sources.list.d/debian.sources

# __deps__
RUN apt-get -q -y update && \
    apt-get -q -y install \
    build-essential \
    qmake6 \
    libgl-dev \
    libgl1 \
    qt6-base-private-dev \
    qt6-base-dev \
    qt6-declarative-dev \
    qt6-declarative-private-dev \
    libqt6opengl6-dev  \
    cmake && \
    apt-get clean && apt-get autoremove && \
    rm -rf /var/lib/apt/lists/*

COPY . /app
WORKDIR /app

#RUN qmake6 -o build/Makefile && \
#    cd build && \
#    make

RUN cmake -G 'Unix Makefiles' -S . -B ./build -DPLATFORM=dimos -DTARGET=deploy
RUN cmake --build ./build --target Dimos -- -j 14

# Deploy ------------------------------------------------------------------------
##
# Deploy Step
##
FROM --platform=linux/${IMAGE_ARCH} \
    torizon/wayland-base${GPU}:${BASE_VERSION} AS Deploy

ARG IMAGE_ARCH
ARG GPU
ARG APP_EXECUTABLE
ENV APP_EXECUTABLE ${APP_EXECUTABLE}

# SSH for remote debug
EXPOSE 2231
ARG SSHUSERNAME=torizon

# Make sure we don't get notifications we can't answer during building.
ENV DEBIAN_FRONTEND="noninteractive"

# stick to bookworm on /etc/apt/sources.list.d
RUN sed -i 's/sid/bookworm/g' /etc/apt/sources.list.d/debian.sources

# for vivante GPU we need some "special" sauce
RUN apt-get -q -y update && \
        if [ "${GPU}" = "-vivante" ]; then \
            apt-get -q -y install \
            imx-gpu-viv-wayland-dev \
        ; else \
            apt-get -q -y install \
            libgl1 \
        ; fi \
    && \
    apt-get clean && apt-get autoremove && \
    rm -rf /var/lib/apt/lists/*

# your regular RUN statements here
# Install required packages
RUN apt-get -q -y update && \
    apt-get -q -y install \
    file \
    curl \
    qt6-base-private-dev \
    qt6-base-dev \
    qt6-wayland \
    qt6-wayland-dev \
    qt6-declarative-dev \
    qt6-declarative-private-dev \
    qml6-module-qtqml \
    qml6-module-qtqml-workerscript \
    qml6-module-qtcore \
    qml6-module-qtquick \
    qml6-module-qtquick-window \
    qml6-module-qtquick-controls \
    qml6-module-qtquick-layouts \
    qml6-module-qtquick-templates \
# DO NOT REMOVE THIS LABEL: this is used for VS Code automation
    # __torizon_packages_prod_start__
    # __torizon_packages_prod_end__
# DO NOT REMOVE THIS LABEL: this is used for VS Code automation
    && \
    apt-get clean && apt-get autoremove && \
    rm -rf /var/lib/apt/lists/*

COPY --from=Build /app/build /app
USER torizon
WORKDIR /app

CMD [ "./Dimos" ]