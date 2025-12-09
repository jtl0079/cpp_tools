#pragma once

#include <cpptools/core/traits.hpp>

namespace cpptools::framework::math::backend::serial {



    template<class MatA, class MatB>
    void transpose(const MatA& A, MatB& B)
    {
        namespace traits = cpptools::core::traits;

        const size_t M = traits::size(A);     // rows
        const size_t N = traits::size(A[0]);  // cols

        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < N; ++j)
                B[j][i] = A[i][j];
    }



    template<class MatA, class MatB>
    void transpose_cache_block(const MatA& A, MatB& B_result)
    {
        namespace traits = cpptools::core::traits;


        const size_t M = traits::size(A);
        const size_t N = traits::size(traits::get(A, 0));

        // Block size：32 是通用最佳点
        constexpr size_t BS = 32;

        for (size_t ii = 0; ii < M; ii += BS) {
            for (size_t jj = 0; jj < N; jj += BS) {

                const size_t i_end = std::min(ii + BS, M);
                const size_t j_end = std::min(jj + BS, N);

                for (size_t i = ii; i < i_end; ++i) {
                    for (size_t j = jj; j < j_end; ++j) {
                        traits::get(traits::get(B_result, j), i) = traits::get(traits::get(A, i), j);
                    }
                }
            }
        }
    }


}