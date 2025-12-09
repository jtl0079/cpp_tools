#pragma once

#include <omp.h>


namespace cpptools::framework::math::backend::openmp {

    template<class MatA, class MatB>
    void transpose(const MatA& A, MatB& B)
    {
        namespace traits = cpptools::core::traits;

        const size_t M = traits::size(A);     // rows
        const int N = static_cast<int>(traits::size(A[0]));  // cols

        // OpenMP：outer j 以减少 cache 冲突
#pragma omp parallel for schedule(static)
        for (int j = 0; j < N; ++j)
        {
            for (size_t i = 0; i < M; ++i)
            {
                B[j][i] = A[i][j];
            }
        }
    }

}