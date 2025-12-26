# cmake/dependencies/sdl/sdl3/sdl3.cmake
include_guard(GLOBAL)


set(SDL3_SOURCE_GIT_REPOSITORY
    "https://github.com/libsdl-org/SDL.git"
    CACHE STRING "SDL3 source git repository"
)  

set(SDL3_MIRROR_GIT_REPOSITORY
    "https://github.com/org-foundation/mirrors_sdl.git"
    CACHE STRING "SDL3 mirror git repository"
)

set(SDL3_GIT_TAG
    "release-3.2.28"
    CACHE STRING "SDL3 git tag"
)

include(${CMAKE_CURRENT_LIST_DIR}/cpptools_resolve_sdl3.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cpptools_enable_sdl3.cmake)

