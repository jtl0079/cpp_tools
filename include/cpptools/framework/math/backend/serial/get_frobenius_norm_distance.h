#pragma once

#include <array>
#include <vector>
#include <cmath>

namespace cpptools::framework::math::backend::serial {

	// Frobenius norm 范数 ||A-B||_F
    template<class MatA, class MatB>
    double get_frobenius_norm_distance(const MatA& A, const MatB& B)
    {
        namespace traits = cpptools::core::traits;

        const size_t M = traits::size(A);            // rows
        const size_t N = traits::size(traits::get(A, 0));    // cols

        // check B size
        if (traits::size(B) != M || traits::size(traits::get(B, 0)) != N) {
            throw std::runtime_error("get_frobenius_norm_distance(): size mismatch");
        }

        double distance = 0.0;

        for (size_t i = 0; i < M; ++i) {
            for (size_t j = 0; j < N; ++j) {

                // traits get
                double d = double(traits::get(traits::get(A, i), j)) -
                    double(traits::get(traits::get(B, i), j));

                distance += d * d;
            }
        }

        return std::sqrt(distance);
    }

}