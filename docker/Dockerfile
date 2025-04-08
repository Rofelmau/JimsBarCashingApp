# Dockerfile.arm64
FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    qt5-default \
    qtbase5-dev \
    qtdeclarative5-dev \
    qttools5-dev-tools \
    qttools5-dev \
    sqlite3 \
    libsqlite3-dev \
    && apt-get clean

# Optional: Ein User ohne root, falls du es brauchst
