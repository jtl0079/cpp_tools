function(cpptools_enable_sdl3)

  cmake_tools_resolve_sdl(
      VERSION   3
             
  )

  if(NOT TARGET cpptools::sdl3)
    add_library(cpptools_sdl3 INTERFACE)
    add_library(cpptools::sdl3 ALIAS cpptools_sdl3)

    target_link_libraries(cpptools_sdl3
      INTERFACE
        SDL3::SDL3
    )

    target_compile_definitions(cpptools_sdl3
      INTERFACE
        CPPTOOLS_USE_SDL3=1
    )

  endif()

  message(STATUS "[cpptools] cpptools::sdl3 ready")
endfunction()