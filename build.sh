#!/bin/sh

cd build
cmake ..
make -j$(nproc)
cd ..
./bin/project_2
