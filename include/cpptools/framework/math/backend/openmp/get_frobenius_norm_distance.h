#pragma once

#include <omp.h>

namespace cpptools::framework::math::backend::openmp {


    template<class MatA, class MatB>
    double get_frobenius_norm_distance(const MatA& A, const MatB& B)
    {
        namespace traits = cpptools::core::traits;

        const int M = static_cast<int>(traits::size(A));                // rows
        const int N = static_cast<int>(traits::size(traits::get(A, 0)));     // cols

        // check B size
        if (traits::size(B) != M || traits::size(traits::get(B, 0)) != N) {
            throw std::runtime_error("get_frobenius_norm_distance(): size mismatch");
        }


        // OpenMP parallel + reduction
        double distance = 0.0;

#pragma omp parallel for reduction(+:distance)
        for (int i = 0; i < M; ++i) {
            const auto& rowA = traits::get(A, i);
            const auto& rowB = traits::get(B, i);
            for (int j = 0; j < N; ++j) {

                double d =
                    double(traits::get(rowA, j)) -
                    double(traits::get(rowB, j));

                distance += d * d;
            }
        }

        return std::sqrt(distance);
    }


    template<class MatA, class MatB>
    double get_frobenius_norm_distance_flatten(const MatA& A, const MatB& B)
    {
        namespace traits = cpptools::core::traits;

        const int M = static_cast<int>(traits::size(A));                // rows
        const int N = static_cast<int>(traits::size(traits::get(A, 0)));     // cols

        // check B size
        if (traits::size(B) != M || traits::size(traits::get(B, 0)) != N) {
            throw std::runtime_error("get_frobenius_norm_distance(): size mismatch");
        }

        // ✔ OpenMP parallel + reduction
        double distance = 0.0;

#pragma omp parallel for reduction(+:distance)
        for (int idx = 0; idx < M * N; ++idx) {
            const int i = idx / N;
            const int j = idx % N;

            double d =
                double(traits::get(traits::get(A, i), j)) -
                double(traits::get(traits::get(B, i), j));

            distance += d * d;
        }

        return std::sqrt(distance);
    }


}