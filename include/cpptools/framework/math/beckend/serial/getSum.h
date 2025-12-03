#pragma once

namespace cpptools::framework::math::backend::serial {

	template<typename T, size_t N>
	T getSum(T(&t)[N]) {
		T sum{};

		for (int i = 0; i < N; i++) {
			sum += t[i];
		}

		return sum;

	}

}