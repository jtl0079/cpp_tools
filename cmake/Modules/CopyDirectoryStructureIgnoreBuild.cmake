# ============================================================================
# copy_directory_structure_ignore_build(<source_dir> <dest_dir>)
#
# - 递归复制目录结构，但自动忽略所有名为 "build" 的目录
# - 不复制文件，只复制目录结构
# - 自动创建缺失的目录
# - 支持空目录
# ============================================================================

function(copy_directory_structure_ignore_build source_dir dest_dir)
    
    message(STATUS "[copy_directory_structure_ignore_build: Start] source_dir = ${source_dir}")
    message(STATUS "[copy_directory_structure_ignore_build: Start] dest_dir = ${dest_dir}")
    
    if(NOT IS_DIRECTORY "${source_dir}")
        message(FATAL_ERROR "[CopyDir] source_dir (${source_dir}) Does not exist")
    endif()

    # 创建目标根目录（根目录不存在时才 log）
    if(NOT EXISTS "${dest_dir}")
        file(MAKE_DIRECTORY "${dest_dir}")
        file(RELATIVE_PATH pretty "${CMAKE_BINARY_DIR}" "${dest_dir}")
        message(STATUS "[CopyDir] Created: ${pretty}")
    endif()


    # 扫描所有目录（包含空目录）
    file(GLOB_RECURSE dirs
        LIST_DIRECTORIES true
        RELATIVE "${source_dir}"
        "${source_dir}/*"
    )

    foreach(path IN LISTS dirs)
        set(full_path "${source_dir}/${path}")

        # 如果路径中包含 /build 或 \build → 忽略
        if(path MATCHES "(^|/|\\\\)build($|/|\\\\)")
            # message(STATUS "[CopyDir] Skip build dir: ${path}")
            continue()
        endif()

        if(IS_DIRECTORY "${full_path}")

            set(target_dir "${dest_dir}/${path}")

            # ⭐ 只有目录不存在时才输出日志
            if(NOT EXISTS "${target_dir}")
                file(MAKE_DIRECTORY "${target_dir}")

                file(RELATIVE_PATH pretty "${CMAKE_BINARY_DIR}" "${target_dir}")
                message(STATUS "[CopyDir] Created: ${pretty}")
            endif()

        endif()
    endforeach()
        message(STATUS "[copy_directory_structure_ignore_build: End]")

endfunction()
