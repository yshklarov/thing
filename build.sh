#!/bin/bash

WARNING_FLAGS="-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused"

LINKER_FLAGS="-lraylib"

COMPILER_FLAGS="-O3"

CPPSTD="c++17"

clang++ -std=$CPPSTD $WARNING_FLAGS $COMPILER_FLAGS $LINKER_FLAGS thing.cpp -o build/thing && ./build/thing
