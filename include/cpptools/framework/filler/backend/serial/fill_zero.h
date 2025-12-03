#pragma once

#include <random>
#include <cpptools/core/traits.hpp>


namespace cpptools::framework::filler::backend::serial {

    template<typename T, typename Arr, typename... Rest>
    void fill(const T& value, Arr& arr, Rest&... rest) {
        namespace core_traits = cpptools::core::traits;

        core_traits::for_each(arr, [&](auto& v) {
            v = value;
            });

        if constexpr (sizeof...(rest) > 0)
            (fill_zero(rest), ...);
    }


    template<typename Arr, typename... Rest>
    void fill_zero(Arr& arr, Rest&... rest) {
        namespace core_traits = cpptools::core::traits;
        
        core_traits::for_each(arr, [](auto& v) {
            v = 0;
            });

        if constexpr (sizeof...(rest) > 0)
            (fill_zero(rest), ...);
    }


}