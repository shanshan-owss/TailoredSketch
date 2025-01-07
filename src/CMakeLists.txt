cmake_minimum_required(VERSION 3.25)
project(TailoredSketch_Code)

set(CMAKE_CXX_STANDARD 17)

# 添加项目根目录和Sketch目录到包含路径
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/Sketch
)

# 只添加源文件到可执行文件
add_executable(${PROJECT_NAME} main.cpp)
