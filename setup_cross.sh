#/bin/bash
mkdir build32
cd build32
cmake -DCMAKE_TOOLCHAIN_FILE=../ToolchainFiles/linux-cross-i686-mingw32.cmake ..
cd ..
mkdir build64
cd build64
cmake -DCMAKE_TOOLCHAIN_FILE=../ToolchainFiles/linux-cross-x86_64.cmake ..

