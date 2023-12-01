#!/bin/bash

if [ -f "/etc/os-release" ]; then
    . /etc/os-release
elif [ -f "/etc/arch-release" ]; then
    export ID=arch
elif [ "$(uname)" == "Darwin" ]; then
    export ID=darwin
else
    echo "/etc/os-release missing."
    exit 1
fi

fbe_fedora_packages=(
    boost-devel
    llvm
    llvm-devel
    clang
    clang-tools-extra
    lld
    cmake
    fmt-devel
    flex
    bison
    git
    lz4-devel
    ninja-build
    zlib-static
    xxhash-devel
    uuid-devel
    google-benchmark-devel
)

fbe_debian_packages=(
    cloc
    curl
    git
    python2
    lld
    #cmake version of ubuntu is also too low.
    cmake
    ninja-build
    gcc
    g++
    flex
    bison
    llvm
    clang
    clangd
    clang-tidy
    libxxhash-dev
    libboost-dev
    libfmt-dev
    libbenchmark-dev
)

fbe_darwin_packages=(
    cmake
    ninja
    fmt
    doxygen
    flex
    bison
    xxhash
    boost
    llvm
)

case "$ID" in
ubuntu | debian)
    apt-get install -y "${fbe_debian_packages[@]}"
    ;;
fedora)
    dnf install -y "${fbe_fedora_packages[@]}"
    ;;
darwin)
    brew install -f "${fbe_darwin_packages[@]}"
    ;;
*)
    echo "Your system ($ID) is not supported by this script. Please install dependencies manually."
    exit 1
    ;;
esac
