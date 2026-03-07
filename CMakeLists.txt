cmake_minimum_required(VERSION 3.22)

project("ALL")

# debug模式,生成文件加上后缀d
set(CMAKE_DEBUG_POSTFIX "d")

# 设置安装路径
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)

# 强制要求编译器支持该标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 禁用编译器特定扩展
set(CMAKE_CXX_EXTENSIONS OFF)

# 平台检测宏
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message("检测到 Windows 平台")
    add_definitions(-DWIN32_LEAN_AND_MEAN) # 减少Windows头文件体积
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message("检测到 Linux 平台")
endif()

IF(CMAKE_BUILD_TYPE STREQUAL Debug)
    # 添加宏定义
    add_definitions(-DDEBUG)
    message("Debug 模式")

    # 检查编译器是否是GCC/Clang
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message("使用 GCC/Clang 编译器")
        message("编译器版本为：${CMAKE_C_COMPILER_VERSION}")

        if(${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER 10)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Og -g")
        else()
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fcoroutines -Og -g")
        endif()

    # Windows MSVC 编译器设置
    elseif(MSVC)
        message("使用 MSVC 编译器")

        # /Zi: 生成调试信息 /Od: 禁用优化 /RTC1: 运行时检查
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /Od /MDd /RTC1")

        # 启用调试迭代器
        add_definitions(-D_DEBUG -D_ITERATOR_DEBUG_LEVEL=2)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    endif()
ELSEIF(CMAKE_BUILD_TYPE STREQUAL Release)
    # 添加宏定义
    add_definitions(-DNODEBUG)
    message("Release 模式")

    # 检查编译器是否是GCC/Clang
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message("使用 GCC/Clang 编译器")
        message("编译器版本为：${CMAKE_C_COMPILER_VERSION}")

        if(${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER 10)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Ofast -s")
        else()
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fcoroutines -Ofast -s")
        endif()

    # Windows MSVC 编译器设置
    elseif(MSVC)
        message("使用 MSVC 编译器")

        # /O2: 优化速度 /Gy: 函数级链接
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /Gy /MD")

        # 链接时优化
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    endif()
ELSE()
    message("未指定编译模式,退出编译")
    return()
ENDIF()

# 设置Boost默认为静态库
set(Boost_USE_STATIC_LIBS ON)

# 设置Boost路径
set(Boost_ROOT "D:/work/boost/boost_1_90_0" CACHE PATH "Path to Boost installation")

# 设置libzip路径
set(libzip_ROOT "D:/work/libzip" CACHE PATH "Path to libzip installation")

# 添加子目录
add_subdirectory(src) # 拷贝配置文件
add_custom_target(DeployConfig
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/config"
    "${CMAKE_INSTALL_PREFIX}/bin/config"
    COMMENT "拷贝配置文件到安装目录中"
)