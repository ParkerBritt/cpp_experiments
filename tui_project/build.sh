#!/usr/bin/env bash
set -e

# rm -rf ./build
mkdir ./build  -p
cd build 
cmake .. 
make 
cd .. 
./build/bin/tui_application
