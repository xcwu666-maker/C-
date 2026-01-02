cmake_minimum_required(VERSION 3.10)
project(DroneLogisticsSystem)

# 设置 C++ 标准为 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含头文件目录 (include 文件夹)
include_directories(include)

# 自动搜寻 src 目录下所有的 .cpp 源文件
file(GLOB SOURCES "src/*.cpp")

# 生成可执行程序 DroneApp
add_executable(DroneApp ${SOURCES})