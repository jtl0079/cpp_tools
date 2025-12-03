#pragma once

#include <iostream>
#include <string>

namespace cpptools::framework::print {

	template<typename T, size_t N>
	void print(T(&t)[N], std::string delimiter = ", ") {
		using namespace std;
		int limiter = N - 1;
		int i = 0;
		for (i = 0; i < limiter; i++) {
			cout << t[i] << delimiter;
		}
		cout << t[i];
	}


	template<typename T, size_t N, size_t N2>
	void print(T(&t)[N][N2], std::string delimiter = ", ") {
		using namespace std;
		int n = N - 1;
		int n2 = N2 - 1;
		int i = 0;
		int i2 = 0;
		for (i; i < n; i++) {
			for (i2 = 0; i2 < n2; i2++) {
				cout << t[i][i2] << delimiter;
			}
			cout << t[i][i2] << "\n";
		}
		for (i2 = 0; i2 < n2; i2++) {
			cout << t[i][i2] << delimiter;
		}
		cout << t[i][i2];

	}

}

