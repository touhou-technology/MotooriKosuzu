#include <benchmark/benchmark.h>

static void TestBenchmark(benchmark::State &state) {
	for (auto _ : state) {
		for (int i = 0; i < 2000; ++i) {
		}
	}
}

BENCHMARK(TestBenchmark);
BENCHMARK_MAIN();
