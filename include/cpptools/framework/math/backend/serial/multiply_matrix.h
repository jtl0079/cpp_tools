#pragma once

#include <cpptools/core/traits/traits.hpp>

namespace cpptools::framework::math::backend::serial {




    template<class MatA, class MatB, class MatC>
    void multiply_matrix(const MatA& A, const MatB& B, MatC& C_result) {

        using namespace cpptools::core::traits;
        
        const size_t M = size(A);
        const size_t N = size(get(A, 0));
        const size_t N2 = size(B);
        const size_t P = size(get(B, 0));

        // --- check ---
        static_assert(N == N2, "Matrix dimension mismatch: A(MxN) * B(NxP): N != rows(B)");
        static_assert(size(C_result) == M, "Result matrix C must have M rows");
        static_assert(size(get(C_result, 0)) == P, "Result matrix C must have P columns");

        // --- clean ---
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < P; ++j)
                get(C_result, i)[j] = {};

        // --- count ---
        for (size_t i = 0; i < M; ++i) {
            for (size_t k = 0; k < N; ++k) {
                auto a = get(A, i)[k];
                for (size_t j = 0; j < P; ++j) {
                    get(C_result, i)[j] += a * get(B, k)[j];
                }
            }
        }
    }

}
