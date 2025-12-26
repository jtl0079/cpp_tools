# CMakeLists.txt for the cpptools library.
# Location: C:\..\libs\cpptools\
cmake_minimum_required(VERSION 3.28)
project(cpptools 
    VERSION 0.1.0
    LANGUAGES CXX
)

message(STATUS "TOP LEVEL CMake Loaded")

# preparation include<lib> start
include(FetchContent)
include(cmake/cpp_tools_bootstrap_cmake_tools.cmake)
cpp_tools_bootstrap_cmake_tools()




# --- 修正 1：使用 CMAKE_CURRENT_LIST_DIR 才是 cpptools 子模块路径 ---
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")
message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")

# --- 修正 2：确认模块已加载 ---
include(GenerateRecursiveAggregateHeadersToUpperDirForEachLevel)
include(GenerateAggregateHeadersForEachLevelIgnoreBuild)


# --- 修正 3：正确调用（位置参数，不要使用 SRC_DIR=xxx 这种） ---
set(AUTO_GENERATED_ROOT "${CMAKE_BINARY_DIR}/auto_generated")



generate_aggregate_headers_for_each_level_ignore_build(
    "${CMAKE_CURRENT_LIST_DIR}/include"
    "${CMAKE_CURRENT_LIST_DIR}/include"
)


generate_recursive_aggregate_headers_to_upper_dir_for_each_level(
    "${CMAKE_CURRENT_LIST_DIR}/include/cpptools"
)

# --- 定义库 target ---
add_library(cpptools INTERFACE)

target_include_directories(cpptools
  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# binary root 必须最先导出
include(cmake/msvs/cpptools_export_binary_root_props.cmake)
cpptools_export_binary_root_props()



# -------- SDL framework（可选） --------

option(CPPTOOLS_ENABLE_SDL3 "Enable SDL3 support" ON)

if(CPPTOOLS_ENABLE_SDL3)
  include(cmake/dependencies/sdl/sdl3/sdl3.cmake)
  cpptools_enable_sdl3()
endif()


option(CPPTOOLS_ENABLE_FFMPEG "Enable FFmpeg support" ON)

if(CPPTOOLS_ENABLE_FFMPEG)
  include(cmake/dependencies/ffmpeg/ffmpeg.cmake)
  #cpptools_enable_ffmpeg()
endif()



