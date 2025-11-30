# ============================================
#  Module CMake Template (Auto Aggregate Header)
#  模块聚合头：<foldername>.h
#  父聚合头（父目录同名）：<parentdirname>.h （引用模块聚合头）
# ============================================
cmake_minimum_required(VERSION 3.15)

# 模块名 = 文件夹名
get_filename_component(MODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${MODULE_NAME})

# 跟目录头文件路径（视项目结构调整）
set(CPPTOOLS_INCLUDE_ROOT
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../..
)

# ================= OpenMP 依赖 =================
find_package(OpenMP REQUIRED)

# ================= 源文件 =================
file(GLOB MODULE_SOURCES
    ${CPPTOOLS_INCLUDE_ROOT}/src/framework/${MODULE_NAME}/*.cpp
)

list(LENGTH MODULE_SOURCES SRC_COUNT)

if(SRC_COUNT EQUAL 0)
    message(STATUS "[cpptools::framework::${MODULE_NAME}] Header-only INTERFACE module.")
    add_library(${MODULE_NAME} INTERFACE)
    target_include_directories(${MODULE_NAME}
        INTERFACE
            ${CPPTOOLS_INCLUDE_ROOT}
    )
else()
    message(STATUS "[cpptools::framework::${MODULE_NAME}] Found ${SRC_COUNT} source file(s) — STATIC library.")
    add_library(${MODULE_NAME} STATIC ${MODULE_SOURCES})
    target_include_directories(${MODULE_NAME}
        PUBLIC
            ${CPPTOOLS_INCLUDE_ROOT}
    )
    target_link_libraries(${MODULE_NAME}
        PUBLIC
            OpenMP::OpenMP_CXX
    )
endif()

# ==================================================
# 自动生成模块聚合头 <MODULE_NAME>.h（优先 .h 格式）
# ==================================================

# 扫描当前目录的所有头文件（支持 .h 和 .hpp）
file(GLOB MODULE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

# 2. 构造目标内容 (收集到变量中)
set(AGG_NAME_H   "${MODULE_NAME}.h")
set(AGG_NAME_HPP "${MODULE_NAME}.hpp")

# 2. 构造目标内容 (收集到变量中)
set(AGG_NAME_H   "${MODULE_NAME}.h")
set(AGG_NAME_HPP "${MODULE_NAME}.hpp")

set(INTERNAL_AGG_CONTENT "#pragma once

// ============================================
// Auto Generated Header
// Module : cpptools::framework::${MODULE_NAME}
// Do not edit manually
// ============================================

")

foreach(h ${MODULE_HEADERS})
    get_filename_component(name ${h} NAME)
    # 排除聚合头自身
    if(NOT name STREQUAL "${AGG_NAME_H}" AND NOT name STREQUAL "${AGG_NAME_HPP}")
        string(APPEND INTERNAL_AGG_CONTENT "#include \"${name}\"\n")
    endif()
endforeach()


# 3. 比较并有条件地写入文件
set(AGG_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_NAME}.h")
unset(EXISTING_CONTENT)

if(EXISTS ${AGG_HEADER})
    # 读取现有内容
    file(READ ${AGG_HEADER} EXISTING_CONTENT)
endif()

# 比较新的内容是否与现有内容不同
if(NOT INTERNAL_AGG_CONTENT STREQUAL EXISTING_CONTENT)
    file(WRITE ${AGG_HEADER} "${INTERNAL_AGG_CONTENT}")
    message(STATUS "[${MODULE_NAME}] ✅ Updated internal aggregate: ${MODULE_NAME}.h (Content changed)")
else()
    message(STATUS "[${MODULE_NAME}] ⏸ Internal aggregate: ${MODULE_NAME}.h (Content unchanged. Skipping write)")
endif()




# ==================================================
# 在父目录生成/更新父聚合头 <parentdirname>.h
#
# 每个模块插入一行：
# #include "modname/modname.h"
# ==================================================
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

set(PARENT_AGG_HEADER
    "${PARENT_DIR}/${MODULE_NAME}.h"   # 注意：用模块名命名，不是父目录名
)


# 1. 构造目标内容 (收集到变量中)
set(EXTERNAL_AGG_CONTENT "#pragma once

// ============================================
// Auto Generated Header - Bridge File
// Module : cpptools::framework::${MODULE_NAME}
// Do not edit manually
// ============================================


#include \"${MODULE_NAME}/${MODULE_NAME}.h\"\n"
)

# 2. 比较并有条件地写入文件
unset(EXISTING_PARENT_CONTENT)

if(EXISTS ${PARENT_AGG_HEADER})
    file(READ ${PARENT_AGG_HEADER} EXISTING_PARENT_CONTENT)
endif()

if(NOT EXTERNAL_AGG_CONTENT STREQUAL EXISTING_PARENT_CONTENT)
    file(WRITE ${PARENT_AGG_HEADER} "${EXTERNAL_AGG_CONTENT}")
    message(STATUS "[${MODULE_NAME}] ✅ Updated external bridge header: ${MODULE_NAME}.h (Content changed)")
else()
    message(STATUS "[${MODULE_NAME}] ⏸ External bridge header: ${MODULE_NAME}.h (Content unchanged. Skipping write)")
endif()


