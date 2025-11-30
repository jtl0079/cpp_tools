# ============================================================================
# copy_directory_structure(<source_dir> <dest_dir>)
#
# - 递归复制目录结构（不复制文件），dest_dir 不存在则创建
# - 会自动创建所有子目录
# - 支持 Windows / Linux / macOS
# - 创建时打印日志
# ============================================================================

function(copy_directory_structure source_dir dest_dir)
    message(STATUS "[copy_directory_structure: Start] source_dir = ${source_dir}")
    message(STATUS "[copy_directory_structure: Start] dest_dir = ${dest_dir}")

    # -------- 参数合法性检查 --------
    if(NOT IS_DIRECTORY "${source_dir}")
        message(FATAL_ERROR "[CopyDir] source_dir (${source_dir}) Does not exist")
    endif()

    # 创建目标根目录
    file(MAKE_DIRECTORY "${dest_dir}")

    # -------- 扫描所有目录（包含空目录）--------
    file(GLOB_RECURSE dirs
        LIST_DIRECTORIES true
        RELATIVE "${source_dir}"
        "${source_dir}/*"
    )


 # -------- 遍历 --------
    foreach(path IN LISTS dirs)
        set(full_path "${source_dir}/${path}")

        if(IS_DIRECTORY "${full_path}")
            set(target_dir "${dest_dir}/${path}")

            # 创建目标子目录
            file(MAKE_DIRECTORY "${target_dir}")

            # 美化日志（显示相对路径）
            file(RELATIVE_PATH pretty "${CMAKE_BINARY_DIR}" "${target_dir}")
            message(STATUS "[CopyDir] Created: ${pretty}")
        endif()
    endforeach()
    message(STATUS "[copy_directory_structure: End]")

endfunction()