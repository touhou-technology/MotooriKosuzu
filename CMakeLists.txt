cmake_minimum_required(VERSION 3.28)
project(MotooriKosuzu LANGUAGES CXX)

# GCC
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 26)

# include CPM
include(cmake/CPM.cmake)

# include FindCPP_modules
include(cmake/FindCPP_modules.cmake)

CPMAddPackage("gh:brainboxdotcc/DPP@10.1.5")

# #simdjson
# CPMAddPackage(
#   NAME simdjson
#   GITHUB_REPOSITORY "simdjson/simdjson"
#   GIT_TAG "v4.6.4"
# )
#
# #EnTT
# CPMAddPackage(
#   NAME entt
#   GITHUB_REPOSITORY "skypjack/entt"
#   GIT_TAG "main"
#   OPTIONS "ENTT_INCLUDE_NATVIS ON;"
# )

# FTXUI
CPMAddPackage(
  NAME FTXUI
  GITHUB_REPOSITORY "ArthurSonzogni/FTXUI"
  GIT_TAG "v7.0.0"
  OPTIONS "FTXUI_BUILD_MODULES ON;CMAKE_GENERATOR Ninja"
)

# spdlog
CPMAddPackage(
  NAME spdlog
  GITHUB_REPOSITORY gabime/spdlog
  VERSION "1.17.0"
  OPTIONS "SPDLOG_FMT_EXTERNAL OFF"
)

# google benchmark
CPMAddPackage(
  URI "gh:google/benchmark@1.9.5"
  OPTIONS "BENCHMARK_ENABLE_TESTING Off"
)

# chibi-scheme
# src: cmake -B build .. && sudo make install
set(chibi-scheme_DIR "/usr/local/lib64/cmake/chibi")
find_package(chibi-scheme CONFIG REQUIRED)

# PCH
add_library(PCH STATIC src/pch/dummy.cpp)

target_precompile_headers(PCH PUBLIC
  src/pch/pch.hpp
)

target_link_libraries(PCH PUBLIC
  stdc++exp

  ftxui::screen
  ftxui::dom
  ftxui::component

  spdlog::spdlog

  chibi::libchibi-scheme
)

# PCH End

# core添加核心依赖
add_library(core STATIC)

target_compile_features(core
  PUBLIC
  cxx_std_26
)

target_include_directories(core
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_sources(core
  PUBLIC
  src/core/web.h
  src/core/web.cpp

  src/core/core.hpp
  src/core/reflection.hpp
)

target_link_libraries(core
  PUBLIC
  stdc++exp

  ftxui::screen
  ftxui::dom
  ftxui::component

  spdlog::spdlog

  # EnTT::EnTT
  # simdjson::simdjson
  chibi::libchibi-scheme

  PCH
)

target_compile_options(core PRIVATE
  -freflection
  -fmodules
)

# core end

# Scheme
add_executable(Scheme
  src/scheme/scheme.cpp
)

target_link_libraries(Scheme
  PRIVATE core)

target_compile_options(Scheme PRIVATE
  -freflection
  -fmodules
)

# Scheme End

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
# Test End

# Benchmark
add_executable(mkbenchmark
  src/benchmark/benchmark.cpp
)

target_link_libraries(mkbenchmark
  PRIVATE core benchmark::benchmark
)

target_compile_options(mkbenchmark PRIVATE
  -freflection
)
# Benchmark

file(CREATE_LINK
  ${CMAKE_SOURCE_DIR}/config/.mk.json
  ${CMAKE_BINARY_DIR}/.mk.json
  SYMBOLIC
)
