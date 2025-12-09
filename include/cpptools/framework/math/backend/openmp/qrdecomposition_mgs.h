#pragma once

#include <omp.h>

namespace cpptools::framework::math::backend::openmp {

    template<class MatA, class MatQ, class MatR>
    void qrdecomposition_mgs(const MatA& A, MatQ& Q, MatR& R)
    {
        using cpptools::core::traits::size;

        const int M = static_cast<int>(size(A));
        const int N = static_cast<int>(size(A[0]));

        using Val = std::decay_t<decltype(A[0][0])>;
        std::vector<std::vector<Val>> U(M, std::vector<Val>(N));

        // Copy A → U（两层循环，但 MSVC 不能 collapse）
#pragma omp parallel for
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                U[i][j] = A[i][j];

        // Zero R
#pragma omp parallel for
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                R[i][j] = Val(0);

        // Zero Q
#pragma omp parallel for
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                Q[i][j] = Val(0);

        // MGS QR
        for (int k = 0; k < N; k++) {

            // -------------------------------------------
            // 1) rkk = ||u_k||
            // -------------------------------------------
            double rkk = 0.0;

#pragma omp parallel for reduction(+:rkk)
            for (int i = 0; i < M; i++)
                rkk += U[i][k] * U[i][k];

            rkk = std::sqrt(rkk);
            R[k][k] = rkk;

            if (rkk == 0.0) continue;

            // -------------------------------------------
            // 2) Q[:,k] = U[:,k] / rkk
            // -------------------------------------------
#pragma omp parallel for
            for (int i = 0; i < M; i++)
                Q[i][k] = U[i][k] / rkk;

            // -------------------------------------------
            // 3) For each j > k:
            //    rkj = dot(Q[:,k], U[:,j]);
            //    U[:,j] = U[:,j] - rkj * Q[:,k]
            // -------------------------------------------
            for (int j = k + 1; j < N; j++) {

                double rkj = 0.0;

                // compute dot(Q[:,k], U[:,j])
#pragma omp parallel for reduction(+:rkj)
                for (int i = 0; i < M; i++)
                    rkj += Q[i][k] * U[i][j];

                R[k][j] = rkj;

                // update U[:,j]
#pragma omp parallel for
                for (int i = 0; i < M; i++)
                    U[i][j] -= rkj * Q[i][k];
            }
        }
    }




}