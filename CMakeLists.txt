# CMakeLists.txt for the cpptools library.
# Location: C:\Cpp_Files\ProjectTest\ProjectTest\libs\cpptools\
cmake_minimum_required(VERSION 3.15)
project(cpptools LANGUAGES CXX)

message(STATUS "TOP LEVEL CMake Loaded")

# --- 修正 1：使用 CMAKE_CURRENT_LIST_DIR 才是 cpptools 子模块路径 ---
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")
message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")

# --- 修正 2：确认模块已加载 ---
include(GenerateRecursiveAggregateHeadersToUpperDirForEachLevel)
include(GenerateAggregateHeadersForEachLevelIgnoreBuild)


# --- 修正 3：正确调用（位置参数，不要使用 SRC_DIR=xxx 这种） ---
set(AUTO_GENERATED_ROOT "${CMAKE_BINARY_DIR}/auto_generated")






generate_recursive_aggregate_headers_to_upper_dir_for_each_level(
    "${CMAKE_CURRENT_LIST_DIR}/include/cpptools"
)

