#pragma once

#include <functional>


enum class BackendKind { Serial, OpenMP };


template<typename Func>
struct BackendDispatch;


template <typename Ret, typename... Args>
struct BackendEntry {
    const char* name;                   // backend 名称
    std::function<Ret(Args...)> fn;     // 要调用的函数
};


template<typename Ret, typename... Args>
auto get_backends_for_dot() {
    using namespace cpptools::framework::math::backend;

    return std::vector<BackendEntry<Ret, Args...>>{
        { "serial", serial::get_dot<Args...> },
#ifdef _OPENMP
        { "openmp",  openmp::get_dot<Args...> },
#endif
#ifdef ENABLE_CUDA
        { "cuda",    cuda::get_dot<Args...> },
#endif
    };
}


template<typename Ret, typename... Args>
void benchmark_all_backends(const char* op_name, const Args&... args)
{
    auto backends = get_backends_for_dot<Ret, Args...>();

    for (auto& backend : backends) {
        auto t0 = std::chrono::steady_clock::now();

        Ret r = backend.fn(args...);

        auto t1 = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration<double>(t1 - t0).count();

        std::cout << op_name << " | "
            << backend.name << " | "
            << dt << " sec" << std::endl;
    }
}
