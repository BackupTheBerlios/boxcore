#/bin/bash
mkdir build32
cd build32
cmake -DCMAKE_TOOLCHAIN_FILE=../ToolchainFiles/linux-cross-i686.cmake ..
