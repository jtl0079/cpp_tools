#pragma once


namespace cpptools::framework::math::backend::serial {

	template<typename T , size_t N, size_t N2>
	T getDot(T (&t)[N], T (&t2)[N2]) {
		static_assert(N == N2, "[cpptools::framework::math::backend::serial::getDot()] Vectors must be same length");
		T dot{};

		for (size_t i = 0; i < N; i++) { dot += t[i] * t2[i]; }
		return dot;
	}

	template<typename T, size_t N, size_t N2>
	T getL2Norm(T (&t)[N], T (&t2)[N2]) {
		return sqrt(getDot(t, t2));
	}


}