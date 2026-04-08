#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

namespace {

constexpr std::size_t kIterations = 30'000'000;

struct UnpaddedCounter {
    std::atomic<std::uint64_t> value{0};
};

struct alignas(64) PaddedCounter {
    std::atomic<std::uint64_t> value{0};
    std::byte pad[64 - sizeof(std::atomic<std::uint64_t>)]{};
};

template <typename Counter>
double run_benchmark(std::size_t num_threads) {
    std::vector<Counter> counters(num_threads);
    std::vector<std::thread> workers;
    workers.reserve(num_threads);

    const auto start = std::chrono::steady_clock::now();

    for (std::size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back([&, i]() {
            for (std::size_t n = 0; n < kIterations; ++n) {
                counters[i].value.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    for (auto& t : workers) {
        t.join();
    }

    const auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

} // namespace

int main() {
    const std::size_t thread_count = std::max(2u, std::thread::hardware_concurrency());

    std::cout << "False sharing demo\n";
    std::cout << "threads=" << thread_count
              << " iterations_per_thread=" << kIterations << "\n";
    std::cout << "sizeof(UnpaddedCounter)=" << sizeof(UnpaddedCounter)
              << " sizeof(PaddedCounter)=" << sizeof(PaddedCounter) << "\n\n";

    const double unpadded_ms = run_benchmark<UnpaddedCounter>(thread_count);
    const double padded_ms = run_benchmark<PaddedCounter>(thread_count);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Unpadded time (likely false sharing): " << unpadded_ms << " ms\n";
    std::cout << "Padded time (sharing avoided):       " << padded_ms << " ms\n";
    if (padded_ms > 0.0) {
        std::cout << "Speedup from padding:               " << (unpadded_ms / padded_ms) << "x\n";
    }

    return 0;
}
