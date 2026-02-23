cmake_minimum_required(VERSION 3.28)
project(MotooriKosuzu LANGUAGES CXX)

# GCC
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 26)

#

#set(CMAKE_C_COMPILER "/opt/gcc-trunk/bin/gcc")
set(CMAKE_CXX_COMPILER "/opt/gcc-trunk/bin/g++")

# include CPM
include(cmake/CPM.cmake)

CPMAddPackage("gh:brainboxdotcc/DPP@10.1.4")

# FTXUI
CPMAddPackage(
  NAME FTXUI
  GITHUB_REPOSITORY "ArthurSonzogni/FTXUI"
  GIT_TAG "main"
  #OPTIONS "FTXUI_BUILD_MODULES ON;CMAKE_GENERATOR Ninja"
)

# spdlog
CPMAddPackage(
  NAME spdlog
  GITHUB_REPOSITORY gabime/spdlog
  VERSION "1.17.0"
  OPTIONS "SPDLOG_FMT_EXTERNAL OFF"
)

# core添加核心依赖
add_library(core)

target_compile_features(core
  PUBLIC
  cxx_std_26
)

target_sources(core
  PRIVATE
  src/core/core.hpp
  src/core/core.cpp
  src/core/web.h
  src/core/web.cpp
)

target_link_libraries(core
  PUBLIC
  ftxui::screen
  ftxui::dom
  ftxui::component

  spdlog::spdlog
)

target_compile_options(core PRIVATE
  -freflection
)

# core end

# MotooriKosuzu
add_executable(MotooriKosuzu
  src/main.cpp
)

target_link_libraries(MotooriKosuzu
  PRIVATE dpp core)

target_compile_options(MotooriKosuzu PRIVATE
  -freflection
)
#END

# TUI
add_executable(tui
  src/utils/tui.cpp
)

target_link_libraries(tui
  PRIVATE core
)

target_compile_options(tui PRIVATE
  -freflection
)
#END
# Test
add_executable(test
  src/test/test.cpp
)

target_link_libraries(test
  PRIVATE core
)

target_compile_options(test PRIVATE
  -freflection
)
