# ==============================================================================
#  write_if_changed(filepath content)
#   - only rewrite when the content actually changed
# ==============================================================================

function(write_if_changed filepath content)
    if(EXISTS "${filepath}")
        file(READ "${filepath}" old_content)
        if(old_content STREQUAL content)
            return()
        endif()
    endif()

    file(WRITE "${filepath}" "${content}")
    message(STATUS "[AutoGen] Re-Wrote: ${filepath}")
endfunction()
