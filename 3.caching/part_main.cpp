#include <algorithm>
#include <cmath>
#include <limits>
#include <random>
#include <utility>
#include <vector>

#include <benchmark/benchmark.h>

static void L1_cache(benchmark::State &state) {
  std::vector<int> vec;
  int res = std::accumulate(
      vec.begin(), vec.end(), 0,
      [](auto inital, const auto &val) { return inital + val; });
}

static void L2_cache(benchmark::State &state) {
  struct SmallObject {
    int vals[10];
  };
  std::vector<SmallObject> vec;
  int res = std::accumulate(
      vec.begin(), vec.end(), 0,
      [](auto inital, const auto &val) { return inital + val.vals[0]; });
}

static void L3_cache(benchmark::State &state) {
  struct MediumObject {
    int vals[48];
  };
  std::vector<MediumObject> vec;
  int res = std::accumulate(
      vec.begin(), vec.end(), 0,
      [](auto inital, const auto &val) { return inital + val.vals[0]; });
}

static void RAM(benchmark::State &state) {
  struct LargeObject {
    int vals[128];
  };
  std::vector<LargeObject> vec;
  int res = std::accumulate(
      vec.begin(), vec.end(), 0,
      [](auto inital, const auto &val) { return inital + val.vals[0]; });
}