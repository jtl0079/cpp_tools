# ============================================================================
# generate_aggregate_headers_for_each_level_ignore_build(<source_dir> <dest_dir>)
#
# - 在 source_dir 下扫描所有目录（忽略 build）
# - 找到 "叶子目录"（无子目录）
# - 在 dest_dir 中生成对应的聚合头 <dir_name>.hpp
# - 聚合头自动 include 该目录下所有 *.hpp / *.h
# ============================================================================

function(generate_aggregate_headers_for_each_level_ignore_build source_dir dest_dir)
    message(STATUS "[generate_aggregate_headers_for_each_level_ignore_build] Start: source = ${source_dir}")
    message(STATUS "[generate_aggregate_headers_for_each_level_ignore_build] Start: dest   = ${dest_dir}")

    if(NOT IS_DIRECTORY "${source_dir}")
        message(FATAL_ERROR "[generate_aggregate_headers_for_each_level_ignore_build] source_dir does not exist: ${source_dir}")
    endif()

    # 扫描目录（包含空目录）
    file(GLOB_RECURSE all_dirs
        LIST_DIRECTORIES true
        RELATIVE "${source_dir}"
        "${source_dir}/*"
    )

    foreach(path IN LISTS all_dirs)
        set(full_path "${source_dir}/${path}")

        # 跳过 build 目录
        if(path MATCHES "(^|/|\\\\)build($|/|\\\\)")
            continue()
        endif()

        # 不是目录 → 跳过
        if(NOT IS_DIRECTORY "${full_path}")
            continue()
        endif()

# 获取目录名（最后一层）
        get_filename_component(dir_name "${path}" NAME)

        # 查找目录下的头文件 (*.h *.hpp)
        file(GLOB hdrs
            RELATIVE "${source_dir}/${path}"
            "${full_path}/*.h"
            "${full_path}/*.hpp"
        )

        # 没有头文件 → 可以选择跳过，这里跳过
        if(hdrs STREQUAL "")
            continue()
        endif()

        # 生成目标目录结构
        set(target_dir "${dest_dir}/${path}")
        if(NOT EXISTS "${target_dir}")
            file(MAKE_DIRECTORY "${target_dir}")
        endif()

        # 聚合头文件路径
        set(out_file "${target_dir}/${dir_name}.hpp")

        # 开始写内容
        set(content "#pragma once\n\n")

        foreach(h IN LISTS hdrs)
            # 排除生成自己的同名头
            if(h STREQUAL "${dir_name}.hpp")
                continue()
            endif()

            string(APPEND content "#include <${path}/${h}>\n")
        endforeach()

        file(WRITE "${out_file}" "${content}")

        file(RELATIVE_PATH pretty "${CMAKE_BINARY_DIR}" "${out_file}")
        message(STATUS "[generate_aggregate_headers_for_each_level_ignore_build] Generated: ${pretty}")

    endforeach()

    message(STATUS "[generate_aggregate_headers_for_each_level_ignore_build] End")
endfunction()

