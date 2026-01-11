cmake_minimum_required(VERSION 3.22)

project("ALL")

# debug模式,生成文件加上后缀d
set(CMAKE_DEBUG_POSTFIX "d")

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)

# 平台检测宏
if(WIN32)
    message("检测到 Windows 平台")
    add_definitions(-DWIN32_LEAN_AND_MEAN) # 减少Windows头文件体积
elseif(UNIX)
    message("检测到 Linux 平台")
endif()

IF(CMAKE_BUILD_TYPE STREQUAL Debug)
    # 添加宏定义
    add_definitions(-DDEBUG)
    message("Debug 模式")

    # 检查编译器是否是GCC
    if(CMAKE_COMPILER_IS_GNUCC)
        message("使用 GCC 编译器")
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

    # 检查编译器是否是GCC
    if(CMAKE_COMPILER_IS_GNUCC)
        message("使用 GCC 编译器")
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

# 设置默认为静态库
set(BUILD_SHARED_LIBS OFF)

# 设置依赖库路径
set(Boost_ROOT "D:/work/boost/boost_1_90_0" CACHE PATH "Path to Boost installation")

# 定义Boost编译的配置宏
# 关于boost.log的编译选项，参文档.配置与构建库
add_definitions(-DBOOST_LOG_USE_COMPILER_TLS)

#[[
使用宏BOOST_LOG_USE_COMPILER_TLS将提升库性能但需承担以下限制：
应用程序可执行文件必须与 Boost.Log 库链接，不应在运行时动态加载。
应用程序不得在全局构造函数或析构函数中使用日志记录功能。
]]
if(WIN32)
    add_definitions(-DBOOST_USE_WINAPI_VERSION=0x0A00)

#[[boost.asio中定义_WIN32_WINNT 为0x0601，而boost默认编译时会使用当前系统的值而不是boost.asio中的值，在使用boost.log时会找不到符号（值“v2s_mt_nt6”不匹配值“v2s_mt_nt62）
    这里强制指定BOOST_USE_WINAPI_VERSION为0x0A00，不同的Windows系统应该赋什么值，参照Windows关于_WIN32_WINNT的定义和描述。
    ]]
else()
endif()

# 添加子目录
add_subdirectory(src)

# 拷贝配置文件
add_custom_target(DeployConfig
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/config"
    "${CMAKE_INSTALL_PREFIX}/bin/config"
    COMMENT "拷贝配置文件到安装目录中"
)