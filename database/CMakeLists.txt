# 设置CMake最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(DataBaseProject)

# 指定C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 将所有源文件加入到变量中
set(SOURCES
    src/database.cpp
    src/model.cpp
    DataBaseThread.cpp
)

# 生成可执行文件
add_executable(DataBaseThread ${SOURCES})

# 链接SQLite3库
find_package(SQLite3 REQUIRED)
target_link_libraries(DataBaseThread SQLite::SQLite3)
