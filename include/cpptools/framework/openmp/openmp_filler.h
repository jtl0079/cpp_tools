#pragma once

#include <iostream>

// update 
// openmp_filler.h

namespace cpptools::framework::openmp {

	template<typename T, size_t N>
	void fillWithIndex(T (&t)[N]) {
#pragma omp parallel for 
		for (int i = 0; i < N; i++) {
			t[i] = i;
		}
	}

	template<typename T, size_t N>
	void fillWithIndex(T(&t)[N], T startValue, T increment) {
#pragma omp parallel for  
		for (int i = 0; i < N; i++) {
			T input = (i * increment) + startValue;
			t[i] = input;
		}
	}

	template<typename T, size_t N>
	void fillWithIndex(T(&t)[N], T increment) {
#pragma omp parallel for  
		for (int i = 0; i < N; i++) {
			T input = i * increment;
			t[i] = input;
		}
	}




}

