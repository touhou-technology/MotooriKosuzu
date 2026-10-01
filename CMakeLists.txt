cmake_minimum_required(VERSION 3.28)
project(MotooriKosuzu LANGUAGES CXX)

# GCC
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 26)

# include CPM
include(cmake/CPM.cmake)

CPMAddPackage("gh:brainboxdotcc/DPP@10.1.6")

# MotooriKosuzu
add_executable(MotooriKosuzu
  src/main.cpp
)

target_link_libraries(MotooriKosuzu dpp)

file(CREATE_LINK
  ${CMAKE_SOURCE_DIR}/config/.mk.json
  ${CMAKE_BINARY_DIR}/.mk.json
  SYMBOLIC
)
