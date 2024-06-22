#!/bin/bash
cd build
cmake .. && \
  cp ./compile_commands.json .. && \
  make && \
  cd .. && \
  ./build/bin/tests
  ./build/bin/pls
