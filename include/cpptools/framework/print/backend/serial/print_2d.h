#pragma once

#include <iomanip>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <cpptools/core/traits.hpp>

namespace cpptools::framework::print::backend::serial {

	// 获取单个元素转 string 的长度
	template<typename T>
	size_t element_width(const T& v) {
		std::ostringstream oss;
		oss << v;
		return oss.str().size();
	}


	template<typename T, size_t M, size_t N>
	void print_2d(const T(&t)[M][N], const std::string& delimiter = ", ")
	{
		using namespace std;

		for (size_t i = 0; i < M; ++i) {

			cout << t[i][0];

			for (size_t j = 1; j < N; ++j)
				cout << delimiter << t[i][j];

			if (i + 1 < M)
				cout << "\n";
		}
	}



	template<class Mat>
	void print_2d_pretty(const Mat& A)
	{
		namespace traits = cpptools::core::traits;
		using namespace std;

		const size_t M = traits::size(A);
		const size_t N = traits::size(A[0]);

		// 1) 找到最长元素长度
		size_t width = 0;
		for (size_t i = 0; i < M; ++i)
			for (size_t j = 0; j < N; ++j)
				width = std::max<size_t>(width, element_width(A[i][j]));

		// 2) 打印
		for (size_t i = 0; i < M; ++i) {

			cout << "[ ";

			for (size_t j = 0; j < N; ++j) {

				cout << setw(width) << A[i][j];

				if (j + 1 < N)
					cout << ", ";
			}
			cout << "]\n";
		}
	}


}


