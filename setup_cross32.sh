#/bin/bash
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../ToolchainFiles/linux-cross-i686.cmake ..
