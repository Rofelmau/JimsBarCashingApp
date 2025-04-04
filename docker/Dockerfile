# Basis-Image
FROM ubuntu:20.04

# Umgebungsvariablen setzen
ENV DEBIAN_FRONTEND=noninteractive
ENV QT_VERSION=5.15.2
ENV RPI_SYSROOT=/opt/rpi-sysroot
ENV RPI_TOOLCHAIN=/opt/toolchain

# Notwendige Pakete installieren
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    unzip \
    python3 \
    python3-pip \
    python3-setuptools \
    ninja-build \
    g++-arm-linux-gnueabihf \
    pkg-config \
    libfontconfig1-dev \
    libdbus-1-dev \
    libfreetype6-dev \
    libicu-dev \
    libsqlite3-dev \
    libssl-dev \
    libpng-dev \
    libjpeg-dev \
    && rm -rf /var/lib/apt/lists/*

# Cross-Compiler herunterladen und einrichten
RUN wget -qO- https://releases.linaro.org/components/toolchain/binaries/latest-7/arm-linux-gnueabihf/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf.tar.xz | tar -xJ -C /opt && \
    mv /opt/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf $RPI_TOOLCHAIN

# Sysroot vom Raspberry Pi kopieren (dieser Schritt erfordert, dass Sie den Sysroot bereits vorbereitet haben)
# Kopieren Sie den Sysroot in das Verzeichnis /opt/rpi-sysroot im Docker-Image

# Qt-Quellcode herunterladen und entpacken
RUN wget https://download.qt.io/archive/qt/5.15/$QT_VERSION/single/qt-everywhere-src-$QT_VERSION.tar.xz && \
    tar -xJf qt-everywhere-src-$QT_VERSION.tar.xz -C /opt && \
    rm qt-everywhere-src-$QT_VERSION.tar.xz

# Qt konfigurieren und bauen
RUN mkdir -p /opt/qt-build && cd /opt/qt-build && \
    /opt/qt-everywhere-src-$QT_VERSION/configure \
        -release \
        -opengl es2 \
        -device linux-rasp-pi4-v3d \
        -device-option CROSS_COMPILE=$RPI_TOOLCHAIN/bin/arm-linux-gnueabihf- \
        -sysroot $RPI_SYSROOT \
        -prefix /usr/local/qt5pi \
        -opensource \
        -confirm-license \
        -nomake examples \
        -nomake tests && \
    make -j$(nproc) && \
    make install
