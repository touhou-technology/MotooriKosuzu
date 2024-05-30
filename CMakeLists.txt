cmake_minimum_required(VERSION 3.22)
project(MotooriKosuze)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)

#file
FILE(GLOB_RECURSE SRC_HEADER "src/*.h")
FILE(GLOB_RECURSE SRC_SOURCE "src/*cpp")
FILE(GLOB_RECURSE SRC_HPP "src/*.hpp")

add_executable (MotooriKosuze 
	${SRC_HEADER} ${SRC_SOURCE} ${SRC_PP}
	)


add_subdirectory(lib)
target_link_libraries(MotooriKosuze PUBLIC nlohmann_json::nlohmann_json brynet httplib::httplib)
target_compile_definitions(MotooriKosuze PUBLIC _SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING)

# Find our pre-installed DPP package (using FindDPP.cmake).
find_package(DPP REQUIRED)
 
# Include the DPP directories.
target_include_directories(MotooriKosuze PRIVATE ${DPP_INCLUDE_DIR})

target_link_libraries(MotooriKosuze ${DPP_LIBRARIES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MotooriKosuze PROPERTY CXX_STANDARD 20)
endif()