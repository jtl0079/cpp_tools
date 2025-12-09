#pragma once


namespace cpptools::core::traits {

	// 通用遍历：接受一个 lambda 对所有元素执行操作
    // -------------------------------
    // Base case: scalar
    // -------------------------------
    template<typename T, typename F>
    void for_each(T& value, F&& f) {
        f(value);
    }

    // -------------------------------
    // C-array
    // -------------------------------
    template<typename T, size_t N, typename F>
    void for_each(T(&arr)[N], F&& f) {
        for (size_t i = 0; i < N; i++)
            for_each(arr[i], f);
    }

    // -------------------------------
    // std::array
    // -------------------------------
    template<typename T, size_t N, typename F>
    void for_each(std::array<T, N>& arr, F&& f) {
        for (auto& v : arr)
            for_each(v, f);
    }

    // -------------------------------
    // std::vector
    // -------------------------------
    template<typename T, typename F>
    void for_each(std::vector<T>& arr, F&& f) {
        for (auto& v : arr)
            for_each(v, f);
    }

}