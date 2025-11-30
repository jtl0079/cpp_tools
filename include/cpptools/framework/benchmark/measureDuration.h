#pragma once

#include <chrono>

namespace cpptools::framework::benchmark {

	//------------------------------------------------------------------------------
	// measureDuration（适用于返回 void 的函数）
	// 用于测量某个函数执行所耗费的时间。
	// 调用 func()，并返回其耗时（以 Duration 指定的时间单位表示）。
	// 
	//------------------------------------------------------------------------------
	// measureDuration (void-returning function)
	//
	// Measures the execution time of a callable that returns void.
	// Returns the elapsed time using the specified Duration type.
	//------------------------------------------------------------------------------
	template<
		typename Duration = std::chrono::microseconds,
		typename F,
		typename R = std::invoke_result_t<F>,
		std::enable_if_t<std::is_void_v<R>, int> = 0
	>
	Duration measureDuration(F&& func)
	{
		using clock = std::chrono::steady_clock;

		auto start = clock::now();
		func();
		auto end = clock::now();

		return std::chrono::duration_cast<Duration>(end - start);
	}



	//------------------------------------------------------------------------------
	// measureDuration（适用于有返回值的函数）
	// 与上方版本类似，但额外返回函数执行的返回值。
	// 返回 std::pair：
	//   first  = 函数执行耗时（Duration）
	//   second = 函数返回值（R）
	// 
	//------------------------------------------------------------------------------
	// measureDuration (non-void-returning function)
	//
	// Measures the execution time of a callable that returns a value.
	// Returns a pair <Duration, R> where:
	//   - Duration: elapsed time of the function
	//   - R: the return value from the function being measured
	//------------------------------------------------------------------------------

	template<
		typename Duration = std::chrono::microseconds,
		typename F,
		typename R = std::invoke_result_t<F>,
		std::enable_if_t<!std::is_void_v<R>, int> = 0
	>
	std::pair<Duration, R> measureDuration(F&& func)
	{
		using clock = std::chrono::steady_clock;

		auto start = clock::now();
		R result = func();
		auto end = clock::now();

		return {
			std::chrono::duration_cast<Duration>(end - start),
			std::move(result)
		};
	}
}
