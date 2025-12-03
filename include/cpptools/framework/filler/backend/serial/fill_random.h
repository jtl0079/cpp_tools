#pragma once

#include <random>
#include <type_traits>
#include <cpptools/core/traits.hpp>

namespace cpptools::framework::filler::backend::serial {

    // ============================================================
    // 1. 通用 for_each 填充（所有最终逻辑都回到这里）
    // ============================================================
    template<typename Arr, typename RNG, typename Dist>
    void fill_random_impl(Arr& arr, RNG& rng, Dist& dist)
    {
        cpptools::core::traits::for_each(arr, [&](auto& v) {
            using V = std::remove_reference_t<decltype(v)>;
            v = static_cast<V>(dist(rng));
            });
    }


    // ============================================================
    // 2. 默认 dist 类型（整数 → uniform_int，浮点 → uniform_real）
    // ============================================================
    template<typename T>
    struct default_dist {
        using type = std::conditional_t<
            std::is_integral_v<T>,
            std::uniform_int_distribution<T>,
            std::uniform_real_distribution<T>
        >;
    };


    // ============================================================
    // 3. 默认版 fill_random（支持 任意维度 + rest...）
    //
    // 用法：
    //   fill_random(arr, min, max);
    //   fill_random(arr1, min1,max1, arr2, min2,max2, ...)
    // ============================================================
    template<typename Arr, typename T = std::remove_all_extents_t<Arr>, typename... Rest>
    void fill_random(
        Arr& arr,
        T min_val = T{ 0 },
        T max_val = std::is_integral_v<T> ? T{ 100 } : T{ 1 },
        Rest&&... rest
    )
    {
        using Dist = typename default_dist<T>::type;

        std::mt19937 rng{ std::random_device{}() };
        Dist dist(min_val, max_val);

        // 本数组（任意维）
        fill_random_impl(arr, rng, dist);

        // 后续数组（rest）
        if constexpr (sizeof...(rest) > 0)
            fill_random(std::forward<Rest>(rest)...);
    }

}
