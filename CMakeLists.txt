cmake_minimum_required(VERSION 3.29)
project(WebPulseChecker)

set(CMAKE_CXX_STANDARD 23)

# 添加第三方库
# 添加源文件
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(WebPulseChecker ${SOURCES})
# 添加头文件
target_include_directories(WebPulseChecker PRIVATE 3rd)
target_include_directories(WebPulseChecker PRIVATE include)

# 添加编译时的宏定义
# 项目版本
add_definitions(-DPROJECT_VERSION="${project_version}:${identifiers}")
# 项目名称
add_definitions(-DPROJECT_NAME="${project_name}")
# 编译系统版本
add_definitions(-DBUILD_SYSTEM_VERSION="${CMAKE_SYSTEM}")
# 编译器版本
add_definitions(-DBUILD_COMPILER_VERSION="${COMPILER_VERSION}")
# 编译时间
add_definitions(-DBUILD_DATETIME="${BUILD_TIME}")


# 添加编译选项
# debug模式下添加调试信息
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-stack-protector")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-var-tracking")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize-address-use-after-scope")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=signed-integer-overflow")
endif()
# 添加错误检查
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")
# 添加编译选项
# release模式下添加优化选项
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    # 只包含真正需要的库
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--as-needed")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    # 添加编译选项，静态编译
    set(CMAKE_EXE_LINKER_FLAGS "-static")
    # 静态链接标准库
    target_link_libraries(WebPulseChecker PRIVATE -static-libgcc -static-libstdc++)
endif ()
