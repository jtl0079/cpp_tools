# ============================================================================
# generate_aggregate_header_for_each_level_ignore_build(<source_dir> <dest_dir>)
#
# - 递归复制目录结构（不复制文件），dest_dir 不存在则创建
# - 会自动创建所有子目录
# - 支持 Windows / Linux / macOS
# - 创建时打印日志
# ============================================================================


# GenerateAggregateHeader.cmake
#
# 目的: 提供两种头文件聚合功能：
# 1. generate_module_aggregate_header: 为单一目录下的头文件创建聚合头文件。
# 2. generate_recursive_aggregate_headers: 递归遍历目录结构，在每层生成聚合头文件。

# --- 内部辅助函数：通用文件写入逻辑 (带内容检查) ---
# _write_if_changed(<FILEPATH> <CONTENT>)
function(_write_if_changed _filepath _content)
    unset(_existing)
    if(EXISTS "${_filepath}")
        file(READ "${_filepath}" _existing)
    endif()

    if(NOT "${_content}" STREQUAL "${_existing}")
        file(WRITE "${_filepath}" "${_content}")
        message(STATUS "[AggregateHeader] Wrote: ${_filepath}")
    else()
        message(STATUS "[AggregateHeader] Unchanged: ${_filepath}")
    endif()
endfunction()

# --- 内部辅助函数：在不支持 COMPARE LENGTH 的旧版 CMake 中实现长度排序 ---
# _sort_by_length(<LIST_VAR>)
function(_sort_by_length _list_var)
    set(_list ${${_list_var}})
    set(_list_sorted)

    # 步骤 A: 创建 (长度;路径) 对
    # 例如: "15;cpptools/framework/openmp"
    foreach(_item ${_list})
        string(LENGTH "${_item}" _len)
        # 使用分号作为分隔符
        list(APPEND _list_sorted "${_len};${_item}")
    endforeach()
    
    # 步骤 B: 按字符串排序。由于长度在最前面，会按长度进行排序（升序）。
    list(SORT _list_sorted)

    set(_result_list)
    # 步骤 C: 提取路径 (移除长度前缀)
    foreach(_item ${_list_sorted})
        # 查找第一个分号的位置
        string(FIND "${_item}" ";" _semicolon_pos)
        # 提取分号之后的部分 (即路径)
        math(EXPR _start_pos "${_semicolon_pos} + 1")
        string(SUBSTRING "${_item}" ${_start_pos} -1 _path)
        list(APPEND _result_list "${_path}")
    endforeach()

    # 将结果返回给调用者（PARENT_SCOPE）
    set(${_list_var} ${_result_list} PARENT_SCOPE)
endfunction()


# =================================================================================
# 1. 简单聚合函数：为单个目录下的头文件生成一个聚合头
# =================================================================================
# Usage:
# generate_module_aggregate_header(
#     MODULE_DIR <path/to/source/dir>
#     OUTPUT_DIR <path/to/output/dir>
#     MODULE_NAME <optional_name_for_aggregate_file>
# )
function(generate_module_aggregate_header)
    cmake_parse_arguments(GMAH "" "MODULE_DIR;OUTPUT_DIR;MODULE_NAME" "" ${ARGN})

    if(NOT GMAH_MODULE_DIR OR NOT GMAH_OUTPUT_DIR)
        message(FATAL_ERROR "generate_module_aggregate_header: MODULE_DIR and OUTPUT_DIR are required")
    endif()

    file(TO_CMAKE_PATH "${GMAH_MODULE_DIR}" _MOD_DIR)
    file(TO_CMAKE_PATH "${GMAH_OUTPUT_DIR}" _OUT_DIR)

    # 确定聚合头文件的名称
    if(NOT GMAH_MODULE_NAME)
        get_filename_component(_MODULE_NAME "${_MOD_DIR}" NAME)
    else()
        set(_MODULE_NAME "${GMAH_MODULE_NAME}")
    endif()

    # 1. 确保输出目录存在
    file(MAKE_DIRECTORY "${_OUT_DIR}")
    set(_AGG_H_PATH "${_OUT_DIR}/${_MODULE_NAME}.h")

    # 2. 收集 MODULE_DIR 下的所有头文件（非递归）
    file(GLOB _MODULE_HEADERS "${_MOD_DIR}/*.h" "${_MOD_DIR}/*.hpp")

    # 3. 构建聚合内容
    set(_AGG_CONTENT "// Auto Generated Aggregate Header - Module: ${_MODULE_NAME}\n#pragma once\n\n")
    foreach(_h ${_MODULE_HEADERS})
        get_filename_component(_hname ${_h} NAME)
        # 排除同名文件，防止自引用
        if(NOT _hname STREQUAL "${_MODULE_NAME}.h" AND NOT _hname STREQUAL "${_MODULE_NAME}.hpp")
            string(APPEND _AGG_CONTENT "#include \"${_hname}\"\n")
        endif()
    endforeach()

    # 4. 写入文件
    _write_if_changed("${_AGG_H_PATH}" "${_AGG_CONTENT}")
endfunction()


# =================================================================================
# 2. 递归聚合函数：从叶子节点到根节点生成聚合头文件
# =================================================================================
# Usage:
# generate_recursive_aggregate_headers(
#     ROOT_DIR <path/to/source/root/e.g., include>
#     OUTPUT_ROOT <path/to/output/root/e.g., build/generated_include>
# )
function(generate_recursive_aggregate_headers)
    cmake_parse_arguments(GRH "" "ROOT_DIR;OUTPUT_ROOT" "" ${ARGN})

    if(NOT GRH_ROOT_DIR)
        message(FATAL_ERROR "generate_recursive_headers: ROOT_DIR is required")
    endif()

    file(TO_CMAKE_PATH "${GRH_ROOT_DIR}" SRC_ROOT)

    if(NOT GRH_OUTPUT_ROOT)
        set(OUT_ROOT "${CMAKE_BINARY_DIR}/auto_generated/include")
    else()
        file(TO_CMAKE_PATH "${GRH_OUTPUT_ROOT}" OUT_ROOT)
    endif()

    # Find all directories (recursive)
    file(GLOB_RECURSE ALL_DIRS RELATIVE "${SRC_ROOT}" LIST_DIRECTORIES true "${SRC_ROOT}/*")

    # Add root itself
    list(APPEND ALL_DIRS "")

    # Sort from deepest to shallowest
    list(SORT ALL_DIRS COMPARE LENGTH)
    list(REVERSE ALL_DIRS)

    # Process directories
    foreach(rel_dir IN LISTS ALL_DIRS)
        set(SRC_DIR "${SRC_ROOT}/${rel_dir}")
        set(OUT_DIR "${OUT_ROOT}/${rel_dir}")

        file(MAKE_DIRECTORY "${OUT_DIR}")

        # Determine module name = folder name
        if(rel_dir STREQUAL "")
            get_filename_component(module_name "${SRC_ROOT}" NAME)
        else()
            get_filename_component(module_name "${SRC_DIR}" NAME)
        endif()

        set(output_file "${OUT_DIR}/${module_name}.h")

        # Collect direct headers
        file(GLOB direct_headers "${SRC_DIR}/*.h" "${SRC_DIR}/*.hpp")

        # Collect subdirectories
        file(GLOB children RELATIVE "${SRC_DIR}" "${SRC_DIR}/*")

        set(content "// Auto Generated: ${module_name}\n#pragma once\n\n")

        # Include subdirectory aggregator headers
        foreach(c ${children})
            if(IS_DIRECTORY "${SRC_DIR}/${c}")
                string(APPEND content "#include \"${c}/${c}.h\"\n")
            endif()
        endforeach()

        string(APPEND content "\n")

        # Include local headers
        foreach(h ${direct_headers})
            get_filename_component(hname "${h}" NAME)
            if(NOT hname STREQUAL "${module_name}.h")
                string(APPEND content "#include \"${hname}\"\n")
            endif()
        endforeach()

        _write_if_changed("${output_file}" "${content}")
    endforeach()
endfunction()