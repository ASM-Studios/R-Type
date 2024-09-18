FROM ubuntu:latest

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install apt-utils -y && \
    apt-get install git curl zip unzip tar cmake ninja-build g++ -y && \
    apt-get install autoconf autoconf-archive libtool pkg-config automake -y && \
    apt-get install libx11-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev libgl1-mesa-dev -y

RUN git clone https://github.com/Microsoft/vcpkg.git /root/vcpkg

WORKDIR /root/vcpkg

ENV VCPKG_ROOT=/root/vcpkg

RUN ./bootstrap-vcpkg.sh

WORKDIR /project

COPY . .

RUN ./release.sh
