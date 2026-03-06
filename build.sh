#! /bin/bash
cd build
# 获取CPU核心数
CPU_CORES=$(nproc)
#并行：编译、安装
cmake --build . --target install --parallel $CPU_CORES

#返回到原始目录
cd ..

# 检查构建结果
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo "Build and install completed successfully!"