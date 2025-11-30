#pragma once

#include <iostream>
#include <string>

namespace cpptools::framework::print {

	template<typename T, size_t N>
	void print(T(&t)[N], std::string delimiter = "\n") {
		using namespace std;
		int limiter = N - 1;
		int i = 0;
		for (i = 0; i < limiter; i++) {
			cout << t[i] << delimiter;
		}
		cout << t[i];
	}


}

