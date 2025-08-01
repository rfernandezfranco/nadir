#!/bin/bash
set -ex
echo "--- Starting build script ---"
rm -rf build
mkdir build
cd build
echo "--- Running cmake ---"
cmake ..
echo "--- Running make ---"
make
echo "--- Build script finished ---"
