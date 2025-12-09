#pragma once

struct BenchmarkResult {
public:
    // Basic info
    std::string algorithm;     // "dot", "transpose", "matmul"
    std::string backend;       // "serial", "openmp", "cuda"
    int threads = 1;           // OMP threads
    size_t problem_size = 0;   // N / M,N,K 等

    // Data size
    size_t bytes_read = 0;
    size_t bytes_written = 0;

    // FLOPs
    double flops = 0;          // total flops

    // Timings
    std::vector<double> times; // 每次迭代的耗时 (seconds)

    // Derived metrics (auto computed)
    double time_min = 0.0;
    double time_avg = 0.0;
    double time_max = 0.0;

    double gflops = 0.0;       // flops / time_min
    double bandwidth = 0.0;    // bytes / time_min
};
