# 启用新策略（使用大写变量）
cmake_policy(SET CMP0144 NEW)
cmake_minimum_required(VERSION 3.21)

# 设置项目名称
project("Test")

message(STATUS "${PROJECT_NAME} submodule Build directory                                   : ${CMAKE_BINARY_DIR}")
message(STATUS "${PROJECT_NAME} submodule Source directory                                  : ${CMAKE_SOURCE_DIR}")
message(STATUS "${PROJECT_NAME} submodule Current working directory                         : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "${PROJECT_NAME} submodule Binary directory for current target               : ${CMAKE_CURRENT_BINARY_DIR}")

add_definitions(-D${PROJECT_NAME}_SELF)

# 查找Boost
find_package(Boost 1.85.0 REQUIRED COMPONENTS filesystem)

if(Boost_FOUND)
    message(STATUS "Boost found: ${Boost_VERSION}")
    message(STATUS "Boost include directories: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost libraries: ${Boost_LIBRARIES}")

    # 添加头文件路径
    include_directories(${Boost_INCLUDE_DIRS})

    # 添加库链接路径
    link_directories(${Boost_LIBRARY_DIRS})
else()
    message(FATAL_ERROR "boost librarie not found")
    return()
endif()

# 查找libzip
find_package(libzip CONFIG REQUIRED)

if(libzip_FOUND)
    message(STATUS "libzip found: ${libzip_VERSION}")
    get_target_property(libzip_INCLUDE_DIRS libzip::zip INTERFACE_INCLUDE_DIRECTORIES)
    message(STATUS "libzip include directories: ${libzip_INCLUDE_DIRS}")
    string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE_UPPER)
    get_target_property(LIBZIP_CURRENT_LIBRARY libzip::zip IMPORTED_IMPLIB_${BUILD_TYPE_UPPER})
    message(STATUS "libzip libraries: ${LIBZIP_CURRENT_LIBRARY}")

else()
    message(FATAL_ERROR "libzip not found")
    return()
endif()

# 设置FMIPP路径
set(FMIPP_DIR "D:/work/fmipp/install" CACHE PATH "Path to FMIPP path")
message("The current FMIPP_DIR of this is ${FMIPP_DIR}")

# 设置FMIPP头文件路径
include_directories(${FMIPP_DIR}/include)

# 设置FMIPP库路径
set(FMIPP_LIB_DIR "${FMIPP_DIR}/lib" CACHE PATH "Path to FMIPP library path")

if(EXISTS "${FMIPP_LIB_DIR}")
    link_directories(${FMIPP_LIB_DIR})
else()
    message(FATAL_ERROR " FMIPP library directory not found: ${FMIPP_LIB_DIR} ")
    return()
endif()

# 添加需编译文件
file(GLOB CPP_File ./*.cpp)

# 设置编译目标
add_executable(${PROJECT_NAME} ${CPP_File})

# 添加库链接选项
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE libzip::zip)

# 添加FMIPP库
if(WIN32)
    file(GLOB FMIPP_lib "${FMIPP_LIB_DIR}/*.lib")
elseif(UNIX)
    # 待定
endif(WIN32)

target_link_libraries(${PROJECT_NAME} PUBLIC ${FMIPP_lib})

if(MSVC)
    # 移除控制台窗口 (GUI应用)  CONSOLE (控制台应用)  WINDOWS (图形界面应用) NATIVE (驱动程序) EFI_APPLICATION (UEFI应用) POSIX (已弃用)
    set(CMAKE_EXE_LINKER_FLAGS " ${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE ")

    # 添加库链接选项 添加Windows系统相关的依赖库
    target_link_libraries(${PROJECT_NAME})
else()
endif()

# 安装库文件
install(TARGETS ${PROJECT_NAME} DESTINATION bin OPTIONAL)

if(WIN32)
    # 拷贝FMIPP库的dll到安装路径的bin中
    file(GLOB FMIPP_DLL "${FMIPP_DIR}/bin/*.dll")
    install(FILES ${FMIPP_DLL} DESTINATION bin)

    # 拷贝libzip库的dll到安装路径的bin中
    file(GLOB LIBZIP_DLL "${libzip_ROOT}/bin/*.dll")
    install(FILES ${LIBZIP_DLL} DESTINATION bin)
elseif(UNIX) # 待定
endif(WIN32)

# 添加依赖项
# add_dependencies(${PROJECT_NAME})