#include <algorithm>
#include <cmath>
#include <limits>
#include <random>
#include <utility>
#include <vector>

#include <benchmark/benchmark.h>

static void L1_cache(benchmark::State &state) {

  std::random_device r;

  std::default_random_engine re(r());
  std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(),
                                          std::numeric_limits<int>::max());

  std::vector<int> vec(state.range(0));
  for (auto &v : vec) {
    v = dist(re);
  }

  for (auto _ : state) {
    int res = std::accumulate(
        vec.begin(), vec.end(), 0,
        [](auto inital, const auto &val) { return inital + val; });

    benchmark::DoNotOptimize(res);
  }
}

BENCHMARK(L1_cache)->Unit(benchmark::kMicrosecond)->Range(64, 64 << 10);

static void L2_cache(benchmark::State &state) {

  std::random_device r;

  std::default_random_engine re(r());
  std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(),
                                          std::numeric_limits<int>::max());

  struct SmallObject {
    int vals[10];
  };

  std::vector<SmallObject> vec(state.range(0));
  for (auto &v : vec) {
    v.vals[0] = dist(re);
  }

  for (auto _ : state) {
    int res = std::accumulate(
        vec.begin(), vec.end(), 0,
        [](auto inital, const auto &val) { return inital + val.vals[0]; });

    benchmark::DoNotOptimize(res);
  }
}

BENCHMARK(L2_cache)->Unit(benchmark::kMicrosecond)->Range(64, 64 << 10);

static void L3_cache(benchmark::State &state) {

  std::random_device r;

  std::default_random_engine re(r());
  std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(),
                                          std::numeric_limits<int>::max());

  struct MediumObject {
    int vals[48];
  };

  std::vector<MediumObject> vec(state.range(0));
  for (auto &v : vec) {
    v.vals[0] = dist(re);
  }

  for (auto _ : state) {
    int res = std::accumulate(
        vec.begin(), vec.end(), 0,
        [](auto inital, const auto &val) { return inital + val.vals[0]; });

    benchmark::DoNotOptimize(res);
  }
}

BENCHMARK(L3_cache)->Unit(benchmark::kMicrosecond)->Range(64, 64 << 10);

static void RAM(benchmark::State &state) {

  std::random_device r;

  std::default_random_engine re(r());
  std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(),
                                          std::numeric_limits<int>::max());

  struct LargeObject {
    int vals[128];
  };

  std::vector<LargeObject> vec(state.range(0));
  for (auto &v : vec) {
    v.vals[0] = dist(re);
  }

  for (auto _ : state) {
    int res = std::accumulate(
        vec.begin(), vec.end(), 0,
        [](auto inital, const auto &val) { return inital + val.vals[0]; });

    benchmark::DoNotOptimize(res);
  }
}

BENCHMARK(RAM)->Unit(benchmark::kMicrosecond)->Range(64, 64 << 10);

BENCHMARK_MAIN();