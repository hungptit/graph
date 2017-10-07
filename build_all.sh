#!/bin/bash

printf "Build fmt\n"
./build_using_cmake.sh fmt > /dev/null

printf "Build Catch\n"
./build_using_cmake.sh Catch > /dev/null

printf "Build Celero\n"
./build_using_cmake.sh Celero > /dev/null

printf "Build cereal\n"
./build_using_cmake.sh cereal "-DJUST_INSTALL_CEREAL=TRUE" > /dev/null 
