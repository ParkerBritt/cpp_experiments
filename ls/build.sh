#!/bin/bash
cd build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && \
  cp ./compile_commands.json .. && \
  make && \
  cd .. && \
  ./build/pls
