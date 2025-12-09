#pragma once

#include <array>
#include <vector>
#include <cmath>
#include <cpptools/framework/copy.hpp>
#include <cpptools/framework/print.hpp>
#include <cpptools/framework/filler.hpp>
#include <cpptools/framework/math/backend/serial/multiply_matrix_vector.h>
#include <cpptools/framework/math/backend/serial/transpose.h>
#include <cpptools/framework/math/backend/serial/upper_triangular_back_substitution.h>
#include <cpptools/framework/filler/backend/serial.hpp>
#include <cpptools/core/traits.hpp>

namespace cpptools::framework::math::backend::serial {


	// 单向量 L2 Norm（正确）
	template<typename T, size_t N>
	double getL2Norm(const T(&vec)[N]) {
		return std::sqrt(get_dot(vec, vec));   // dot(vec, vec)
	}

	template<typename T, size_t N, typename... Vecs>
	double getL2Norm(const T(&first)[N], const Vecs(&... vecs)[N]) {
		return std::sqrt(get_dot(first, vecs...));
	}

	 

	// -----------------------------
	// Modified Gram-Schmidt QR Decomposition
	// A = QR
	// A: m x n, m >= n
	// result: Q (m x n), R (n x n)
	// -----------------------------
	template<class MatA, class MatQ, class MatR>
	void qrdecomposition_mgs(const MatA& A, MatQ& Q, MatR& R)
	{
		using cpptools::core::traits::size;

		const size_t M = size(A);
		const size_t N = size(A[0]);

		using Val = std::decay_t<decltype(A[0][0])>;
		std::vector<std::vector<Val>> U(M, std::vector<Val>(N));

		// Copy A → U
		for (size_t i = 0; i < M; i++)
			for (size_t j = 0; j < N; j++)
				U[i][j] = A[i][j];

		// Zero R
		for (size_t i = 0; i < N; i++)
			for (size_t j = 0; j < N; j++)
				R[i][j] = Val(0);

		// Zero Q
		for (size_t i = 0; i < M; i++)
			for (size_t j = 0; j < N; j++)
				Q[i][j] = Val(0);

		// MGS QR
		for (size_t k = 0; k < N; k++) {

			// Compute rkk = ||u_k||
			// 计算第 k 列的 2-范数
			// L2Norm = |U|
			double rkk = 0.0;
			for (size_t i = 0; i < M; i++)
				rkk += U[i][k] * U[i][k];
			rkk = std::sqrt(rkk);

			R[k][k] = rkk;

			if (rkk == 0.0) continue;

			// Q column k = u_k / rkk
			for (size_t i = 0; i < M; i++)
				Q[i][k] = U[i][k] / rkk;

			// Orthogonalization
			for (size_t j = k + 1; j < N; j++) {
				double rkj = 0.0;
				for (size_t i = 0; i < M; i++)
					rkj += Q[i][k] * U[i][j];

				R[k][j] = rkj;

				for (size_t i = 0; i < M; i++)
					U[i][j] -= rkj * Q[i][k];
			}
		}
	}



	// -----------------------------
	// 上三角回代求解 R x = y
	// return x
	// -----------------------------
	template<class Matrix, class Vector>
	auto get_upper_triangular_back_substitution(const Matrix& R, const Vector& y) {

		using namespace cpptools::core::traits;

		using T = std::decay_t<decltype(get(y, 0))>;

		const size_t n = size(R);
		std::vector<T> x(n, T{});

		// running time check（因为 vector/.size() 可能不是 constexpr）
		if (size(y) != n)
			throw std::runtime_error("Dimension mismatch: R is n×n but y is not length n");

		// 上三角回代
		for (int i = int(n) - 1; i >= 0; --i) {
			T s = get(y, i);

			for (size_t j = i + 1; j < n; ++j)
				s -= get(R, i)[j] * x[j];

			T diag = get(R, i)[i];

			if constexpr (std::is_floating_point_v<T>) {
				// 浮点数奇异检测
				if (std::abs(diag) < T(1e-15))
					x[i] = T{};
				else
					x[i] = s / diag;
			}
			else {
				// 非浮点数 (int等) 不能比较绝对值大小
				if (diag == T{})
					x[i] = T{};
				else
					x[i] = s / diag;
			}
		}

		return x;
	}





	// -----------------------------
	// Ax = b
	// return x
	// -----------------------------

	template<typename T, size_t M, size_t N>
	void solveByQR(const T(&A)[M][N], const T(&b)[M], T (&x_result)[N])
	{
		using namespace cpptools::framework::filler::backend::serial;
		T Q[M][N];
		T R[N][N];

		// 1. QR decomposition
		qrdecomposition_mgs(A, Q, R);

		// 2. Compute y = Qᵀ * b
		T Qt[N][M];
		T y[N];
		
		transpose(Q, Qt);
		multiply_matrix_vector(Qt, b, y);
		
		// 3. solve Rx = y
		upper_triangular_back_substitution(R, y, x_result);

	}

}


/*
     a1, a2, a3
    [  ,   ,   ]
A = [  ,   ,   ]
    [  ,   ,   ]

     q1, q2, q3
    [  ,   ,   ] q1 = a1 / |a1|
Q = [  ,   ,   ] q2 = u2 / |u2|
    [  ,   ,   ] q3 = u3 / |u3|


	[r11, r12, r13]      [|a1|, q1a2, q1a3]
R = [   , r22, r23]  R = [    , |u2|, q2a3]
	[   ,    , r33]      [    ,     , |u3|]

	 u1, u2, u3
    [  ,   ,   ]  u1 = a1
U = [  ,   ,   ]  u2 = a2 - r12q1
    [  ,   ,   ]  u3 = a3 - r13q1 - r23q2
 */