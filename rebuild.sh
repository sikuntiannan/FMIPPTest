#! /bin/bash

# 默认构建类型为Debug
BUILD_TYPE=${1:-Debug}

echo "Cleaning build directory..."
rm -rf build
mkdir build
cd build
echo "Configuring $BUILD_TYPE build..."
#设置编译类型为Debug，设置64位编译，设置编译参数为-m64
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_CXX_FLAGS="-m64" -DCMAKE_C_FLAGS="-m64" ..
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