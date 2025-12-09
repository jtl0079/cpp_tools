#pragma once

#include <omp.h>

namespace cpptools::framework::math::backend::openmp {



    template<class Matrix, class VectorY, class VectorX>
    void upper_triangular_back_substitution(
        const Matrix& R,
        const VectorY& y,
        VectorX& x_result
    ) {
        using cpptools::core::traits::size;

        using T = std::decay_t<decltype(y[0])>;
        const size_t n = size(R);

        // --- dim check  ---
        if (size(y) != n)
            throw std::runtime_error("Dimension mismatch: R is n×n but y is not length n");
        if (size(x_result) != n)
            throw std::runtime_error("Dimension mismatch: x length must be n");

        // --- 回代 ---
        for (int i = int(n) - 1; i >= 0; --i) {

            T sum = y[i];

            // ------------------------------
            // parallel part：j-loop
            // ------------------------------
#pragma omp parallel for reduction(-:sum)
            for (int j = i + 1; j < (int)n; ++j) {
                sum -= R[i][j] * x_result[j];
            }

            T diag = R[i][i];

            if constexpr (std::is_floating_point_v<T>) {
                if (std::abs(diag) < T(1e-15))
                    x_result[i] = T{};
                else
                    x_result[i] = sum / diag;
            }
            else {
                if (diag == T{})
                    x_result[i] = T{};
                else
                    x_result[i] = sum / diag;
            }
        }
    }



}