#! /bin/bash

# 默认构建类型为Debug
BUILD_TYPE=${1:-Debug}

echo "Cleaning build directory..."
rm -rf build
mkdir build
cd build
echo "Configuring $BUILD_TYPE build..."
# 配置 CMake 参数：
#   -DCMAKE_BUILD_TYPE: 设置构建类型 (Debug/Release)
#   -DCMAKE_CXX_FLAGS: 设置 C++ 编译标志
#   -DCMAKE_C_FLAGS: 设置 C 编译标志
#   -DFMIPP_DIR: 设置 FMIPP 库路径
#   -DBoost_ROOT: 设置 Boost 库路径
#   -Dlibzip_ROOT: 设置 libzip 库路径
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
      -DCMAKE_CXX_FLAGS="-m64" \
      -DCMAKE_C_FLAGS="-m64" \
      -DFMIPP_DIR="/home/jiutian/work/fmipp/install" \
      -DBoost_ROOT="/usr/local/" \
      -Dlibzip_ROOT="/home/jiutian/work/libzip-1.11.4/install" \
      ..

# 获取CPU核心数
CPU_CORES=$(nproc)
echo "Building and installing with $CPU_CORES parallel jobs..."
#并行：编译、安装
cmake --build . --target install --target DeployConfig --parallel $CPU_CORES

#返回到原始目录
cd ..

# 检查构建结果
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo "Build and install completed successfully!"