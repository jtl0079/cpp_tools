# ==============================================================================
# generate_recursive_aggregate_headers(ROOT_DIR)
#
# 给一个目录 ROOT_DIR：
#   1. 自动找到其下所有叶子目录（无子目录）
#   2. 从每个叶子往上递归生成聚合头，直到 ROOT_DIR 本身
#   3. 每一层都生成 <name>.hpp，放在同级目录
#   4. 不重复 include
#   5. 不跨层
# ==============================================================================

# --- 生成一个目录的聚合头（单层） ---
function(_gen_single DIR)
    get_filename_component(NAME "${DIR}" NAME)
    get_filename_component(PARENT "${DIR}" DIRECTORY)
    set(OUT "${PARENT}/${NAME}.hpp")

    file(GLOB HEADERS
        LIST_DIRECTORIES false
        "${DIR}/*.h"
        "${DIR}/*.hpp"
    )

    list(REMOVE_ITEM HEADERS "${OUT}")  # 排除自己
    list(REMOVE_DUPLICATES HEADERS)
    list(SORT HEADERS)

    set(TXT "// Auto-generated aggregate for ${NAME}\n#pragma once\n\n")

    foreach(h ${HEADERS})
        file(RELATIVE_PATH REL "${CMAKE_SOURCE_DIR}/include" "${h}")
        string(APPEND TXT "#include <${REL}>\n")
    endforeach()

    if(EXISTS "${OUT}")
        file(READ "${OUT}" OLD)
        if(OLD STREQUAL TXT)
            return()
        endif()
    endif()

    file(WRITE "${OUT}" "${TXT}")
    message(STATUS "[aggregate] Generated: ${OUT}")
endfunction()


# --- 主函数：只要 1 个参数 ---
function(generate_recursive_aggregate_headers_to_upper_dir_for_each_level ROOT)
    get_filename_component(ROOT_ABS "${ROOT}" REALPATH)

    # 递归扫描所有目录
    file(GLOB_RECURSE ALL_DIRS
        LIST_DIRECTORIES true
        RELATIVE "${ROOT_ABS}"
        "${ROOT_ABS}/*"
    )

    set(LEAVES "")

    # 找到所有叶子目录
    foreach(rel ${ALL_DIRS})
        set(full "${ROOT_ABS}/${rel}")
        if(IS_DIRECTORY "${full}")
            file(GLOB SUBS LIST_DIRECTORIES true "${full}/*")
            set(has_subdir FALSE)
            foreach(s ${SUBS})
                if(IS_DIRECTORY "${s}")
                    set(has_subdir TRUE)
                    break()
                endif()
            endforeach()

            if(NOT has_subdir)
                list(APPEND LEAVES "${full}")
            endif()
        endif()
    endforeach()

    # 从每个叶子开始往上走到 ROOT
    foreach(leaf ${LEAVES})
        set(cur "${leaf}")

        while(TRUE)
            _gen_single("${cur}")

            if("${cur}" STREQUAL "${ROOT_ABS}")
                break()
            endif()

            get_filename_component(cur "${cur}" DIRECTORY)
        endwhile()
    endforeach()
endfunction()
