cmake_minimum_required(VERSION 3.22)
project(TouHouProject_Ru_koto)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)

# 将源代码添加到此项目的可执行文件。
FILE(GLOB_RECURSE SRC_HEADER "src/*.h")
FILE(GLOB_RECURSE SRC_SOURCE "src/*cpp")
FILE(GLOB_RECURSE SRC_HPP "src/*.hpp")

add_executable (TouHouProject_Ru_koto 
	${SRC_HEADER} ${SRC_SOURCE} ${SRC_HPP}
	)

# Find our pre-installed DPP package (using FindDPP.cmake).
find_package(DPP REQUIRED)
 
# Include the DPP directories.
target_include_directories(TouHouProject_Ru_koto PRIVATE ${DPP_INCLUDE_DIR})

target_link_libraries(TouHouProject_Ru_koto ${DPP_LIBRARIES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TouHouProject_Ru_koto PROPERTY CXX_STANDARD 20)
endif()
