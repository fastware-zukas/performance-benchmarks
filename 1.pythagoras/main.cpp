
#include "oop_triangle.h"
#include "soa_triangle.h"

#include <random>
#include <vector>

#include <benchmark/benchmark.h>

static void oop_compute_hypotenuses(benchmark::State &state) {

  const int item_count = state.range(0);

  std::random_device r;

  std::default_random_engine re(r());
  std::uniform_real_distribution<float> side_dist(1, 20);

  std::vector<oop_triangle> triangles;
  triangles.reserve(item_count);

  for (int i = 0; i < item_count; ++i) {
    triangles.emplace_back(side_dist(re), side_dist(re));
  }

  for (auto _ : state) {
    for (auto &triangle : triangles) {
      triangle.calcHypotenuse();
    }
    benchmark::DoNotOptimize(triangles.data());
  }
}

BENCHMARK(oop_compute_hypotenuses)
    ->Unit(benchmark::kMicrosecond)
    ->RangeMultiplier(2)
    ->Range(64, 64 << 12);

static void soa_compute_hypotenuses(benchmark::State &state) {

  const int item_count = state.range(0);

  std::random_device r;

  std::default_random_engine re(r());
  std::uniform_real_distribution<float> side_dist(1, 20);

  soa_triangles triangles;
  triangles.base_data.reserve(item_count);
  triangles.height_data.reserve(item_count);

  for (int i = 0; i < item_count; ++i) {
    triangles.base_data.push_back(side_dist(re));
    triangles.height_data.push_back(side_dist(re));
  }

  for (auto _ : state) {
    triangles.hypotenuse_data =
        calcHypotenuses(triangles.base_data, triangles.height_data);
    benchmark::DoNotOptimize(triangles.hypotenuse_data.data());
  }
}

BENCHMARK(soa_compute_hypotenuses)
    ->Unit(benchmark::kMicrosecond)
    ->RangeMultiplier(2)
    ->Range(64, 64 << 12);

BENCHMARK_MAIN();