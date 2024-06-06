cmake_minimum_required(VERSION 3.22)
project(MotooriKosuzu)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)

# 将源代码添加到此项目的可执行文件。
FILE(GLOB_RECURSE SRC_HEADER "src/*.h")
FILE(GLOB_RECURSE SRC_SOURCE "src/*cpp")
FILE(GLOB_RECURSE SRC_HPP "src/*.hpp")

add_executable (${PROJECT_NAME}
	${SRC_HEADER} ${SRC_SOURCE} ${SRC_HPP}
	)

# Find our pre-installed DPP package (using FindDPP.cmake).
find_package(DPP REQUIRED)
find_package(jsoncpp REQUIRED)

# Include JSONCPP directories.
target_link_libraries(${PROJECT_NAME} jsoncpp)

# Include the DPP directories.
target_link_libraries(${PROJECT_NAME} ${DPP_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE ${DPP_INCLUDE_DIR})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()
