#pragma once

#include <omp.h>

namespace cpptools::framework::math::backend::openmp {





	template<typename T, size_t N, size_t N2>
	T getDot(const T(&a)[N], const T(&b)[N2]) {
		static_assert(N == N2, "[cpptools::framework::math::backend::openmp::getDot()] Vectors must be same length");

		T dot = 0;

		// 计算可以整除4的部分
		int limit = static_cast<int>(N / 4 * 4);

		// 并行循环，处理整块元素
#pragma omp parallel for reduction(+:dot)
		for (int i = 0; i < limit; i += 4) {
			dot += a[i] * b[i]
				+ a[i + 1] * b[i + 1]
				+ a[i + 2] * b[i + 2]
				+ a[i + 3] * b[i + 3];
		}

		// 处理剩余 0~3 个元素
		for (int i = limit; i < static_cast<int>(N); ++i) {
			dot += a[i] * b[i];
		}

		return dot;
	}

}