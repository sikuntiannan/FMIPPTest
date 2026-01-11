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
set(Boost_ROOT "D:/work/boost/boost_1_90_0")
find_package(Boost 1.85.0 REQUIRED COMPONENTS filesystem)

if(Boost_FOUND)
else()
    message(STATUS "boost librarie not found")
    return()
endif()

# 添加头文件路径
include_directories(${Boost_INCLUDE_DIRS})

# 设置FMIPP头文件路径
include_directories("D:/work/fmipp/")

# 添加库链接路径
link_directories(${Boost_LIBRARY_DIRS})

# 设置FMIPP库路径
set(FMIPP_LIB_DIR "D:/work/fmipp/build/${CMAKE_BUILD_TYPE}")

if(EXISTS "${FMIPP_LIB_DIR}")
    link_directories(${FMIPP_LIB_DIR})
else()
    message(WARNING " FMIPP library directory not found: ${FMIPP_LIB_DIR} ")
    return()
endif()

# 添加需编译文件
file(GLOB CPP_File ./*.cpp)

# 设置编译目标
add_executable(${PROJECT_NAME} ${CPP_File})

# 添加库链接选项
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})

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
    file(GLOB FMIPP_DLL "${FMIPP_LIB_DIR}/*.dll")
    install(FILES ${FMIPP_DLL} DESTINATION bin)

elseif(UNIX)
    # 待定
endif(WIN32)

# 添加依赖项
# add_dependencies(${PROJECT_NAME})