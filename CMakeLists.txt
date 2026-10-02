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
  src/Application.cpp
  src/BambooSlips.h
  src/Bookshelf.hpp
  #src/Dictation.cpp
  #src/Dictation.h
  src/MotooriKosuzu.cpp
  src/MotooriKosuzu.h
  src/Stone.cpp
  src/Stone.h
  src/WritingBrush.cpp
  src/WritingBrush.h
)

target_link_libraries(MotooriKosuzu dpp curl)

file(CREATE_LINK
  ${CMAKE_SOURCE_DIR}/config/.mk.json
  ${CMAKE_BINARY_DIR}/.mk.json
  SYMBOLIC
)
