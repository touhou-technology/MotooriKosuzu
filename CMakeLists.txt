cmake_minimum_required(VERSION 3.10)
project(MotooriKosuzu)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)

FILE(GLOB_RECURSE MAIN "src/Application.cpp")

FILE(GLOB_RECURSE APP_HEADER "src/*.h")
FILE(GLOB_RECURSE APP_SOURCE "src/*cpp")
FILE(GLOB_RECURSE APP_HPP "src/*.hpp")

# 将源代码添加到此项目的可执行文件。
add_executable (MotooriKosuzu 
	${MAIN}
	${APP_HEADER} ${APP_SOURCE} ${APP_HPP}
	)

# Find our pre-installed DPP package (using FindDPP.cmake).
find_package(HTTPLIB REQUIRED)
find_package(JSONCPP REQUIRED)
find_package(DPP REQUIRED)
 
# Include the DPP directories.
target_include_directories(MotooriKosuzu PRIVATE ${DPP_INCLUDE_DIR})
target_include_directories(MotooriKosuzu PRIVATE ${HTTPLIB_INCLUDE_DIR})
target_include_directories(MotooriKosuzu PRIVATE ${JSONCPP_INCLUDE_DIR})

target_link_libraries(MotooriKosuzu 
${DPP_LIBRARIES} 
${HTTPLIB_INCLUDE_DIR}
${JSONCPP_INCLUDE_DIR}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MotooriKosuzu PROPERTY CXX_STANDARD 20)
endif()