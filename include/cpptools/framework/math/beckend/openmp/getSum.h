#pragma once

#include<iostream>
#include<omp.h>
#include<vector>

namespace cpptools::framework::math::backend::openmp {

	// 泛型函数：对 vector 中的所有元素进行求和
	// T 必须支持默认构造 T{} 以及运算符 +=
	template<typename T>
	T getSum(const std::vector<T>& v)
	{
		using namespace std;

		const size_t n = v.size();
		const T* ptr = v.data();
		T result{};

#pragma omp parallel for reduction(+:result) schedule(static)
		for (int i = 0; i < n; i++) {
			result += ptr[i];
		}
		return result;
	}



	template<typename T, size_t N>
	T getSum(T(&t)[N]) {
		T sum{};

#pragma omp parallel for reduction(+:sum) schedule(static)
		for (int i = 0; i < N; i++) {
			sum += t[i];
		}

		return sum;

	}



}
