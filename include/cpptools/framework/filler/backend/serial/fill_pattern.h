#pragma once

#include <cpptools/core/traits.hpp>

namespace cpptools::framework::filler::backend::serial {

	// ============================================================
	//  fill_pattern(arr, pattern, ...)
	//  用 pattern 循环填充任意维度数组
	// ============================================================
	template<typename Arr, typename Pattern, typename... Rest>
	void fill_pattern(Arr& arr, const Pattern& pattern, Rest&... rest)
	{
		using namespace  cpptools::core::traits;

		size_t idx = 0;
		const size_t pat_size = size(pattern);

		for_each(arr, [&](auto& v) {
			v = pattern[idx % pat_size];
			idx++;
			});

		if constexpr (sizeof...(rest) > 0)
			(fill_pattern(rest, pattern), ...);
	}




}