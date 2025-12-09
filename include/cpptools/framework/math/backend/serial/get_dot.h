#pragma once

#include <vector>
#include <array>
#include <type_traits>
#include <cassert>

namespace cpptools::framework::math::backend::serial {

    template<class T1, class... Ts>
    auto get_dot(const T1& vector_a, const Ts&... rest)
    {
        using namespace cpptools::core::traits;

        const size_t n = cpptools::core::traits::size(vector_a);

        // 运行时检查（数组会编译期检查，但容器要 runtime）
        ((cpptools::core::traits::size(rest) == n ? void() : throw std::runtime_error("get_dot(): size mismatch")), ...);

        using Val = std::decay_t<decltype(get(vector_a, 0))>;

        Val sum = Val{ 0 };

        for (size_t i = 0; i < n; ++i) {
            Val product = get(vector_a, i);
            ((product *= get(rest, i)), ...);   // 依次相乘
            sum += product;
        }

        return sum;
    }


} 
