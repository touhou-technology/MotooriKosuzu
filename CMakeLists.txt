cmake_minimum_required(VERSION 3.10)
project(MotooriKosuzu)

set(CMAKE_CXX_STANDARD 20)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


# Find our pre-installed DPP package (using FindDPP.cmake).
find_package(Threads REQUIRED)
find_package(HTTPLIB REQUIRED)
find_package(JSONCPP REQUIRED)
find_package(CURL REQUIRED)

FILE(GLOB_RECURSE MAIN "src/Application.cpp")

FILE(GLOB_RECURSE APP_HEADER "src/*.h")
FILE(GLOB_RECURSE APP_SOURCE "src/*cpp")
FILE(GLOB_RECURSE APP_HPP "src/*.hpp")

# 将源代码添加到此项目的可执行文件。
add_executable(${PROJECT_NAME} 
    ${MAIN}
    ${APP_HEADER} ${APP_SOURCE} ${APP_HPP}
)

find_package(DPP REQUIRED)

# 包含源文件目录和 jsoncpp 的包含目录
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src ${JSON_INC_PATH} ${DPP_INCLUDE_DIR})


# Include the DPP directories.
target_include_directories(${PROJECT_NAME} PRIVATE ${DPP_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${DPP_LIBRARIES})