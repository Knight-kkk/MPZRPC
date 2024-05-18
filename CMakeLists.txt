# 设置cmake版本要求和项目名称
cmake_minimum_required(VERSION 3.0)
project(mprpc)


# 设置标准
# 添加编译选项
add_compile_options(-std=c++11)
# 可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 库文件输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# 头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/examples)
# 库文件搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)
# cmake子路径

add_subdirectory(src)
add_subdirectory(examples)