FROM ubuntu:22.04

WORKDIR /app
ARG BUILD_DIR=cmake-build

RUN apt update && apt install -y python3-pip build-essential pkg-config \
    cmake vim && pip install -U --no-cache-dir conan==1.58.0

# install deps
RUN apt install -y libfontenc-dev libxaw7-dev libxkbfile-dev libxmuu-dev \
    libxres-dev libxss-dev libxv-dev libxvmc-dev libxcb-xkb-dev \
    libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-render-util0-dev \
    libxcb-xinerama0-dev libxcb-util-dev libxcb-util0-dev libx11-xcb-dev \
    libxcomposite-dev libxcursor-dev libxfixes-dev libxdamage-dev libxi-dev \
    libxinerama-dev libxrandr-dev libxtst-dev libxxf86vm-dev libxcb-randr0-dev \
    libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-dri3-dev uuid-dev \
    libvdpau-dev libva-dev

# modify conan config
RUN conan profile new default --detect && \
    conan profile update settings.compiler.libcxx=libstdc++11 default

RUN mkdir ${BUILD_DIR}
COPY conanfile.txt .
RUN conan install . -if ${BUILD_DIR} --build=missing
