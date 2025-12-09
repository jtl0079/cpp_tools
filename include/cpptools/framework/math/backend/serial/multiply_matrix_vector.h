#pragma once

#include <cpptools/core/traits/traits.hpp>


namespace cpptools::framework::math::backend::serial {


    template<class Matrix, class Vector, class Out>
    void multiply_matrix_vector(const Matrix& matrix, const Vector& vec, Out& out) {
        using namespace cpptools::core::traits;

        const size_t M = size(matrix);          // col
        const size_t N = size(get(matrix, 0));  // row

        // 用户必须保证 vec.size() >= N，out.size() >= M
        for (size_t i = 0; i < M; ++i) {
            typename std::remove_reference<decltype(get(out, 0))>::type sum{};
            for (size_t j = 0; j < N; ++j) {
                sum += get(matrix, i)[j] * get(vec, j);
            }
            get(out, i) = sum;
        }
    }

}