# 指定CMake最低版本
cmake_minimum_required(VERSION 3.22)

# 设置项目名称和版本
project(Project VERSION 1.0)

	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加可执行文件
aux_source_directory(./src SRC_LIST)

add_executable(MotooriKosuzu ${SRC_LIST})

# 查找并链接所需的库
find_package(Threads REQUIRED)

find_package(httplib REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(DPP REQUIRED)

target_link_libraries(MotooriKosuzu
    httplib::httplib
    jsoncpp_lib
    ${DPP_LIBRARIES}
)

target_include_directories(MotooriKosuzu PRIVATE
    httplib::httplib
    jsoncpp_lib
    ${DPP_INCLUDE_DIR}
)