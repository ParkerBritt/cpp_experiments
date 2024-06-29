#!/bin/bash
cd build
cmake .. && \
  make && \
  cd .. && \
  ./build/bin/tui_application
