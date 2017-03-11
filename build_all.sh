#!/bin/bash

printf "Build fmt\n"
./build_using_cmake.sh fmt > /dev/null

printf "Build googletest\n"
./build_using_cmake.sh googletest > /dev/null

printf "Build Celero\n"
./build_using_cmake.sh Celero > /dev/null

printf "Build cereal\n"
./build_using_cmake.sh cereal "-DJUST_INSTALL_CEREAL=TRUE" > /dev/null 
