FROM ubuntu:22.04

WORKDIR /app

RUN apt update && apt install -y python3-pip build-essential pkg-config \
    cmake vim && pip install -U --no-cache-dir conan==1.58.0

