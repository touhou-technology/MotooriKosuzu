#include <benchmark/benchmark.h>

static void TestBenchmark(benchmark::State &state) {
	for (auto _ : state) {
		for (int i = 0; i < 100000000; ++i) {
		}
	}
}

BENCHMARK(TestBenchmark);
BENCHMARK(TestBenchmark);
BENCHMARK_MAIN();
