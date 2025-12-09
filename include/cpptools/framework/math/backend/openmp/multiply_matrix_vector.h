#pragma once

#include <omp.h>


namespace cpptools::framework::math::backend::openmp {

    /*
    * A = M x N
    * x = N
    * y = N
    */
    template<class Matrix, class Vector, class Out>
    void multiply_matrix_vector(const Matrix& A, const Vector& x, Out& y)
    {
        using cpptools::core::traits::size;

        const int M = static_cast<int>(size(A));     // rows
        const size_t N = size(A[0]);  // cols

        // y must have at least M elements
        // x must have at least N elements

#pragma omp parallel for schedule(static)
        for (int i = 0; i < M; ++i)
        {
            // 推导元素类型 T
            using T = std::decay_t<decltype(A[0][0])>;

            T sum{};  // init 0

            for (size_t j = 0; j < N; ++j)
            {
                sum += A[i][j] * x[j];
            }

            y[i] = sum;
        }
    }


}