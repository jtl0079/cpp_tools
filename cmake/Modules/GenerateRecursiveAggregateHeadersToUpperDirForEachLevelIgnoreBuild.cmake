# ==============================================================================
# generate_recursive_aggregate_headers(ROOT_DIR)
#
# 给一个目录 ROOT_DIR：
#   1. 自动找到其下所有叶子目录
#   2. 从每个叶子一路向上递归到 ROOT_DIR
#   3. 每一层生成一个同级的 <dir>.hpp 聚合头
#   4. 忽略一切 build 目录
# ==============================================================================


# --- 判断目录是否包含 build ---
function(_is_build_dir PATH OUT_VAR)
    string(TOLOWER "${PATH}" LOWER)
    if(LOWER MATCHES "(^|/|\\\\)build($|/|\\\\)")
        set(${OUT_VAR} TRUE PARENT_SCOPE)
    else()
        set(${OUT_VAR} FALSE PARENT_SCOPE)
    endif()
endfunction()


# --- 为单一目录生成聚合头 ---
function(_gen_single DIR)
    # 忽略 build
    _is_build_dir("${DIR}" IS_BUILD)
    if(IS_BUILD)
        return()
    endif()

    get_filename_component(NAME "${DIR}" NAME)
    get_filename_component(PARENT "${DIR}" DIRECTORY)
    set(OUT "${PARENT}/${NAME}.hpp")

    # 扫描目录下头文件
    file(GLOB HEADERS
        LIST_DIRECTORIES false
        "${DIR}/*.h"
        "${DIR}/*.hpp"
    )

    list(REMOVE_ITEM HEADERS "${OUT}")
    list(REMOVE_DUPLICATES HEADERS)
    list(SORT HEADERS)

    set(TXT "// Auto-generated aggregate for ${NAME}\n#pragma once\n\n")

    foreach(h ${HEADERS})
        file(RELATIVE_PATH REL "${CMAKE_SOURCE_DIR}/include" "${h}")
        string(APPEND TXT "#include <${REL}>\n")
    endforeach()

    # 若内容一样则不写
    if(EXISTS "${OUT}")
        file(READ "${OUT}" OLD)
        if(OLD STREQUAL TXT)
            return()
        endif()
    endif()

    file(WRITE "${OUT}" "${TXT}")
    message(STATUS "[aggregate] Generated: ${OUT}")
endfunction()



# --- 主函数：只要一个参数 ---
function(generate_recursive_aggregate_headers_to_upper_dir_for_each_level_ignore_build ROOT)
    get_filename_component(ROOT_ABS "${ROOT}" REALPATH)

    _is_build_dir("${ROOT_ABS}" IS_BUILD)
    if(IS_BUILD)
        message(FATAL_ERROR "ROOT_DIR cannot be inside a build directory!")
    endif()

    # recurse directories
    file(GLOB_RECURSE ALL_DIRS
        LIST_DIRECTORIES true
        RELATIVE "${ROOT_ABS}"
        "${ROOT_ABS}/*"
    )

    set(LEAVES "")

    foreach(rel ${ALL_DIRS})
        set(full "${ROOT_ABS}/${rel}")

        # 只处理目录
        if(NOT IS_DIRECTORY "${full}")
            continue()
        endif()

        # 忽略 build
        _is_build_dir("${full}" IS_BUILD)
        if(IS_BUILD)
            continue()
        endif()

        # 检查是否有子目录
        file(GLOB SUBS LIST_DIRECTORIES true "${full}/*")

        set(has_subdir FALSE)
        foreach(s ${SUBS})
            if(IS_DIRECTORY "${s}")
                _is_build_dir("${s}" SUB_BUILD)
                if(NOT SUB_BUILD)
                    set(has_subdir TRUE)
                    break()
                endif()
            endif()
        endforeach()

        if(NOT has_subdir)
            list(APPEND LEAVES "${full}")
        endif()
    endforeach()


    # 从每个 leaf 走回 ROOT
    foreach(leaf ${LEAVES})
        set(cur "${leaf}")

        while(TRUE)
            _gen_single("${cur}")

            if("${cur}" STREQUAL "${ROOT_ABS}")
                break()
            endif()

            # 上一级
            get_filename_component(cur "${cur}" DIRECTORY)

            _is_build_dir("${cur}" IS_BUILD2)
            if(IS_BUILD2)
                break()  # 避免掉进 build
            endif()
        endwhile()
    endforeach()
endfunction()
