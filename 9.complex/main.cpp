#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <random>
#include <utility>
#include <vector>

#include <benchmark/benchmark.h>

#include "complex.h"

static void constructors_destructors_small(benchmark::State &state) {

  int size = state.range(0);
  for (auto _ : state) {
    for (int i = 0; i < size; ++i) {
      ComplexSmall obj;
      benchmark::DoNotOptimize(obj.value);
    }
    benchmark::ClobberMemory();
  }
}

static void simple_init_small(benchmark::State &state) {

  int size = state.range(0);
  for (auto _ : state) {
    for (int i = 0; i < size; ++i) {
      int value = 1;
      benchmark::DoNotOptimize(value);
      value = 0;
    }
    benchmark::ClobberMemory();
  }
}

static void constructors_destructors_medium(benchmark::State &state) {

  int size = state.range(0);
  for (auto _ : state) {
    for (int i = 0; i < size; ++i) {
      ComplexMedium obj;
      benchmark::DoNotOptimize(obj.value);
    }
    benchmark::ClobberMemory();
  }
}

static void simple_init_medium(benchmark::State &state) {

  int size = state.range(0);
  for (auto _ : state) {
    for (int i = 0; i < size; ++i) {
      std::array<int, 16> values;
      std::fill(values.begin(), values.end(), 1);
      benchmark::DoNotOptimize(values);
      std::fill(values.begin(), values.end(), 0);
    }
    benchmark::ClobberMemory();
  }
}

static void constructors_destructors_large(benchmark::State &state) {

  int size = state.range(0);
  for (auto _ : state) {
    for (int i = 0; i < size; ++i) {
      ComplexLarge obj;
      benchmark::DoNotOptimize(obj.value);
    }
    benchmark::ClobberMemory();
  }
}

static void simple_init_large(benchmark::State &state) {

  int size = state.range(0);
  for (auto _ : state) {
    for (int i = 0; i < size; ++i) {
      std::array<int, 32> values;
      std::fill(values.begin(), values.end(), 1);
      benchmark::DoNotOptimize(values);
      std::fill(values.begin(), values.end(), 0);
    }
    benchmark::ClobberMemory();
  }
}

static void constructors_destructors_largeX(benchmark::State &state) {

  int size = state.range(0);
  for (auto _ : state) {
    for (int i = 0; i < size; ++i) {
      ComplexXLarge obj;
      benchmark::DoNotOptimize(obj.value);
    }
    benchmark::ClobberMemory();
  }
}

static void simple_init_largeX(benchmark::State &state) {

  int size = state.range(0);
  for (auto _ : state) {
    for (int i = 0; i < size; ++i) {
      std::array<int, 64> values;
      std::fill(values.begin(), values.end(), 1);
      benchmark::DoNotOptimize(values);
      std::fill(values.begin(), values.end(), 0);
    }
    benchmark::ClobberMemory();
  }
}

BENCHMARK(constructors_destructors_small)->Arg(100);
BENCHMARK(constructors_destructors_small)->Arg(1000);
BENCHMARK(constructors_destructors_small)->Arg(10000);
BENCHMARK(simple_init_small)->Arg(100);
BENCHMARK(simple_init_small)->Arg(1000);
BENCHMARK(simple_init_small)->Arg(10000);

BENCHMARK(constructors_destructors_medium)->Arg(100);
BENCHMARK(constructors_destructors_medium)->Arg(1000);
BENCHMARK(constructors_destructors_medium)->Arg(10000);
BENCHMARK(simple_init_medium)->Arg(100);
BENCHMARK(simple_init_medium)->Arg(1000);
BENCHMARK(simple_init_medium)->Arg(10000);

BENCHMARK(constructors_destructors_large)->Arg(100);
BENCHMARK(constructors_destructors_large)->Arg(1000);
BENCHMARK(constructors_destructors_large)->Arg(10000);
BENCHMARK(simple_init_large)->Arg(100);
BENCHMARK(simple_init_large)->Arg(1000);
BENCHMARK(simple_init_large)->Arg(10000);

BENCHMARK(constructors_destructors_largeX)->Arg(100);
BENCHMARK(constructors_destructors_largeX)->Arg(1000);
BENCHMARK(constructors_destructors_largeX)->Arg(10000);
BENCHMARK(simple_init_largeX)->Arg(100);
BENCHMARK(simple_init_largeX)->Arg(1000);
BENCHMARK(simple_init_largeX)->Arg(10000);

BENCHMARK_MAIN();