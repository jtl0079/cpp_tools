#pragma once


namespace cpptools::framework::math::backend::serial {

	template<size_t M, size_t N>
	int(*transpose(const int(&A)[M][N]))[M]
		{
			static int B[N][M];
			for (size_t i = 0; i < M; ++i)
				for (size_t j = 0; j < N; ++j)
					B[j][i] = A[i][j];
			return B;
		}


		template<typename T, size_t M, size_t N>
	void transpose(const T (&A)[M][N], T (&dest)[N][M]) {
		for (size_t i = 0; i < M; ++i)
			for (size_t j = 0; j < N; ++j)
				dest[j][i] = A[i][j];
	}


}

