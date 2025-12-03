#pragma once



namespace cpptools::core::traits {

    // --- Detect array extent ---
    template<typename T>
    struct array_traits;

    template<typename T, size_t N>
    struct array_traits<T[N]> {
        using element_type = T;
        static constexpr size_t dim = array_traits<T>::dim + 1;
    };

    template<typename T>
    struct array_traits {
        static constexpr size_t dim = 0;
    };

} // namespace cpptools::core::traits
