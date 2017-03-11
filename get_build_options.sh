#!/bin/bash
ROOT_DIR=$PWD
SRC_DIR=$ROOT_DIR
TMP_DIR=/tmp/build/
PREFIX=$ROOT_DIR
mkdir -p $TMP_DIR

osType=$(uname)
case "$osType" in
    "Darwin")
        {
            NCPUS=$(sysctl -n hw.ncpu);
            CC=clang
            CXX=clang++
            BUILD_OPTS=-j$((NCPUS+1)) CFLAGS="-O3" CXXFLAGS="-O3"
			DYLIB_EXT=".dylib"
        } ;;
    "Linux")
        {
            NCPUS=$(grep -c ^processor /proc/cpuinfo)
            BUILD_OPTS=-j$((NCPUS+1)) CFLAGS="-O3" CXXFLAGS="-O3"
			DYLIB_EXT=".so"
        } ;;
    *)
        {
            echo "Unsupported OS, exiting"
            exit
        } ;;
esac

# Setup build option for make
USE_CLANG="CC=$CLANG CXX=$CLANGPP CFLAGS=-O3 CXXFLAGS=-O3"
CMAKE_RELEASE_BUILD="-DCMAKE_BUILD_TYPE=Release"
CMAKE_USE_CLANG="-DCMAKE_CXX_COMPILER=${CLANGPP} -DCMAKE_C_COMPILER=${CLANG}"

# Display build configurations
printf  "========= Build configuration =========\n"
printf "ROOT_DIR: %s\n" "$ROOT_DIR"
printf "SRC_DIR: %s\n" "$SRC_DIR"
printf "ROOT_DIR: %s\n" "$ROOT_DIR"
printf "TMP_DIR: %s\n" "$TMP_DIR"
printf "BUILD_OPTS: %s\n" "$BUILD_OPTS"
printf  "=======================================\n"
