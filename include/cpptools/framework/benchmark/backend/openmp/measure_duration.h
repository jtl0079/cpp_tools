#pragma once



#include <omp.h>
#include <type_traits>
#include <utility>

namespace cpptools::framework::benchmark::backend::openmp {

    //--------------------------------------------------------------------------
    // measure_duration_omp（适用于返回 void 的函数）
    //--------------------------------------------------------------------------
    template<
        typename Duration = std::chrono::microseconds,
        typename F,
        typename R = std::invoke_result_t<F>,
        std::enable_if_t<std::is_void_v<R>, int> = 0
    >
    Duration measure_duration(F&& func)
    {
        double start = omp_get_wtime();
        func();                      // 执行你的 OpenMP 并行代码
        double end = omp_get_wtime();

        return std::chrono::duration_cast<Duration>(
            std::chrono::duration<double>(end - start)
        );
    }

    //--------------------------------------------------------------------------
    // measure_duration_omp（适用于有返回值的函数）
    //--------------------------------------------------------------------------
    template<
        typename Duration = std::chrono::microseconds,
        typename F,
        typename R = std::invoke_result_t<F>,
        std::enable_if_t<!std::is_void_v<R>, int> = 0
    >
    std::pair<Duration, R> measure_duration(F&& func)
    {
        double start = omp_get_wtime();
        R result = func();            // 执行并行函数
        double end = omp_get_wtime();

        return {
            std::chrono::duration_cast<Duration>(
                std::chrono::duration<double>(end - start)
            ),
            std::move(result)
        };
    }
}
