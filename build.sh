#!/bin/sh
makedir -p build
cd build
cmake ..
make -j$(nproc)
cd ..
./bin/project_2
