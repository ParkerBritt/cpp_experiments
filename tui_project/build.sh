#!/bin/env bash
set -e

rm -rf ./build
mkdir ./build 
cd build 
cmake .. 
make 
cd .. 
./build/bin/tui_application
