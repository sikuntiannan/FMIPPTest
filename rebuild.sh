#! /bin/bash
rm -rf build
mkdir build
cd build
#清理构建
cmake --build . --target clean
#生成编译方式
cmake -DCMAKE_BUILD_TYPE=Debug ..
#并行：编译、安装
cmake --build . --target install --target DeployQT --target DeployConfig --parallel 
