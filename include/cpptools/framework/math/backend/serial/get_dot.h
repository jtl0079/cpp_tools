#pragma once
#include <vector>
#include <array>
#include <type_traits>
#include <cassert>

namespace cpptools::framework::math::backend::serial {

    // ---------------------------------------------
    // multiply_at：单个 index 处的乘积
    // ---------------------------------------------

    // 原生数组版本
    template<typename T, size_t N, typename... Vecs>
    T multiply_at(size_t i, const T(&first)[N], const Vecs(&... vecs)[N]) {
        return (first[i] * ... * vecs[i]);
    }

    // std::vector version
    template<typename T, typename... Vecs>
    T multiply_at(size_t i, const std::vector<T>& first, const Vecs&... vecs) {
        return (first[i] * ... * vecs[i]);
    }

    // std::array version
    template<typename T, size_t N, typename... Vecs>
    T multiply_at(size_t i, const std::array<T, N>& first, const Vecs&... vecs) {
        return (first[i] * ... * vecs[i]);
    }

    // ---------------------------------------------
    // get_dot：任意数量向量
    // ---------------------------------------------

    // --------- 原生数组 ----------
    template<typename T, size_t N, typename... Vecs>
    T get_dot(const T(&first)[N], const Vecs(&... vecs)[N]) {
        static_assert((std::is_same_v<const T(&)[N], const Vecs(&)[N]> && ...),
            "All arrays must have the same size");

        T sum{};
        for (size_t i = 0; i < N; i++) {
            sum += multiply_at(i, first, vecs...);
        }
        return sum;
    }

    // --------- std::vector ----------
    template<typename T, typename... Vecs>
    T get_dot(const std::vector<T>& first, const Vecs&... vecs) {
        size_t N = first.size();
        assert(((vecs.size() == N) && ...) && "All vectors must have the same size");

        T sum{};
        for (size_t i = 0; i < N; i++) {
            sum += multiply_at(i, first, vecs...);
        }
        return sum;
    }

    // --------- std::array ----------
    template<typename T, size_t N, typename... Vecs>
    T get_dot(const std::array<T, N>& first, const Vecs&... vecs) {
        static_assert(((std::is_same_v<std::array<T, N>, Vecs> && ...)),
            "All std::array must have the same size and type");

        T sum{};
        for (size_t i = 0; i < N; i++) {
            sum += multiply_at(i, first, vecs...);
        }
        return sum;
    }

} 
