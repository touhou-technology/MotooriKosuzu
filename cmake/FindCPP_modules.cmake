# FindCPP_modules.cmake
#
# 用法：
#   find_cpp_modules(
#       TARGET      my_target
#       SRC_DIR     ${CMAKE_SOURCE_DIR}/src
#   )
#
# 需要：
#   CMake >= 3.28
#   C++20 Modules（clang / gcc 新版本）

function(find_cpp_modules)
  cmake_parse_arguments(
    FCM
    ""
    "TARGET;SRC_DIR"
    ""
    ${ARGN}
  )

  if(NOT FCM_TARGET)
    message(FATAL_ERROR "find_cpp_modules: TARGET is required")
  endif()

  if(NOT FCM_SRC_DIR)
    message(FATAL_ERROR "find_cpp_modules: SRC_DIR is required")
  endif()

  if(NOT IS_DIRECTORY "${FCM_SRC_DIR}")
    message(FATAL_ERROR "SRC_DIR does not exist: ${FCM_SRC_DIR}")
  endif()

  # 支持的 module 接口文件扩展名
  set(_MODULE_EXTENSIONS
    "*.ixx"
    "*.cppm"
    "*.mpp"
  )

  set(_MODULE_FILES "")
  foreach(ext IN LISTS _MODULE_EXTENSIONS)
    file(GLOB_RECURSE _FOUND
      CONFIGURE_DEPENDS
      "${FCM_SRC_DIR}/${ext}"
    )
    list(APPEND _MODULE_FILES ${_FOUND})
  endforeach()

  if(_MODULE_FILES STREQUAL "")
    message(STATUS "No C++ module interface files found in ${FCM_SRC_DIR}")
    return()
  endif()

  set(_MODULE_SOURCES "")

  foreach(file IN LISTS _MODULE_FILES)
    # 生成相对路径
    file(RELATIVE_PATH rel "${FCM_SRC_DIR}" "${file}")

    # 用路径生成一个稳定的 module 名（可避免重名）
    # src/math/vector.ixx -> math.vector
    get_filename_component(name "${rel}" NAME_WE)
    get_filename_component(path "${rel}" PATH)

    if(path STREQUAL "")
      set(module_name "${name}")
    else()
      string(REPLACE "/" "." path_dotted "${path}")
      set(module_name "${path_dotted}.${name}")
    endif()

    # 记录 module 源文件
    list(APPEND _MODULE_SOURCES
      FILE_SET cxx_modules
      TYPE CXX_MODULES
      BASE_DIRS "${FCM_SRC_DIR}"
      FILES "${file}"
    )

    message(STATUS "Found C++ module: ${module_name} (${file})")
  endforeach()

  # 挂到 target 上
  target_sources(${FCM_TARGET}
    PUBLIC
    ${_MODULE_SOURCES}
  )
endfunction()
