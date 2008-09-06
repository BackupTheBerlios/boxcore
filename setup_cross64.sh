#/bin/bash
mkdir build64
cd build64
cmake -DCMAKE_TOOLCHAIN_FILE=../ToolchainFiles/linux-cross-x86_64.cmake ..
