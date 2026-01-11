#! /bin/bash
cd build
#并行：编译、安装、
cmake --build . --target install --parallel 
