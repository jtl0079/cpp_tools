project(cpptools_framework_openmp LANGUAGES CXX)
find_package(OpenMP REQUIRED)

add_library(cpptools_framework_openmp STATIC
    openmp.cpp
)

target_include_directories(cpptools_framework_openmp
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include                   # hand-written public headers
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/generated_include>  # generated headers during build
)

target_link_libraries(cpptools_framework_openmp
    PUBLIC OpenMP::OpenMP_CXX
)

target_compile_features(cpptools_framework_openmp PUBLIC cxx_std_17)
