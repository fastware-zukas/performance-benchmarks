#include <benchmark/benchmark.h>

#include <algorithm>
#include <atomic>
#include <cassert>
#include <cstring>
#include <mutex>
#include <numeric>
#include <random>
#include <string>
#include <thread>
#include <vector>

// https://edabit.com/challenge/mGnmpYKnozcjP8Yja

/*
  This challenge deals with finding and counting the number of paths between
  points on a rectilinear grid. A starting point (x, y) with non-negative
  integer coordinates is provided. You are only allowed to move horizontally and
  vertically along the grid. Hence, from (x, y) you may move to (x+1, y), (x-1,
  y), (x, y+1), or (x, y-1). Your goal is to return to the origin (0, 0) in such
  a way that you never increase the distance to the origin. The distance is
  counted as the minimum number of total vertical and horizontal steps to reach
  the origin.

  Create a function that reads a starting location, (x, y) and returns the total
  number of different paths back to the origin. Two paths are different if there
  is at least one step on the path that is different even if most of the steps
  are the same.

  Examples:
    paths(0, 0) ➞ 1
    paths(2, 1) ➞ 3
    paths(2, 2) ➞ 6

  Notes:
    This function can be easily written using recursion. It is STRONGLY
    recommended (though not necessary) that you use some form of recursion in
  your solution. The return type for this function is a positive integer. x and
  y will always be integers greater than or equal to 0.
*/

struct Problem {
  uint32_t x;
  uint32_t y;
  uint64_t out;
};

constexpr Problem problems[]{{0, 0, 1ul},
                             {2, 1, 3ul},
                             {2, 2, 6ul},
                             {6, 9, 5005ul},
                             {0, 8, 1ul},
                             {7, 0, 1ul},
                             {1, 99, 100ul},
                             {31, 31, 465428353255261088ul},
                             {160, 12, 945177440546658501ul},
                             {12, 150, 449331386962827096ul}};

static uint64_t paths(uint32_t x, uint32_t y) {
  constexpr __uint128_t max_value = std::numeric_limits<uint64_t>::max();
  const uint32_t index = std::min(x, y);
  const uint32_t row = x + y;
  assert(index < 36);

  uint64_t row_data[36];
  row_data[0] = 1;

  for (int32_t i = 1; i <= index; i++) {
    const __uint128_t prev_element = row_data[i - 1];
    const __uint128_t scaler = row - i + 1;
    const __uint128_t scaled_element = prev_element * scaler;
    if (max_value < scaled_element) {
      // overflow
      return 0;
    }
    const __uint128_t value = scaled_element / i;
    row_data[i] = value;
  }

  return row_data[index];
}

void test_limits_1() {
  for (uint32_t i = 0; i < 200; i++) {
    for (uint32_t j = 0; j < 200; j++) {
      auto res1 = paths(j, i);
      auto res2 = paths(i, j);
      printf("paths(%u, %u) = %lu\n", j, i, res1);
      printf("paths(%u, %u) = %lu\n", i, j, res2);
      benchmark::DoNotOptimize(res1);
      benchmark::DoNotOptimize(res2);
      if (res1 == 0 || res2 == 0) {
        printf("max reached\n");
        return;
      }
    }
  }
}

void test_limits_2() {
  for (uint32_t i = 0; i < 200; i++) {
    auto res = paths(i, i);
    printf("paths(%u, %u) = %lu\n", i, i, res);
    benchmark::DoNotOptimize(res);
    if (res == 0) {
      printf("max reached\n");
      for (uint32_t j = i; j < 200; j++) {
        auto res1 = paths(j, i - 1);
        auto res2 = paths(i - 1, j);
        printf("paths(%u, %u) = %lu\n", j, i - 1, res1);
        printf("paths(%u, %u) = %lu\n", i - 1, j, res2);
        benchmark::DoNotOptimize(res1);
        benchmark::DoNotOptimize(res2);
        if (res1 == 0 || res2 == 0) {
          printf("max reached\n");
          return;
        }
      }
    }
  }
}

static void finding_paths(benchmark::State &state) {

  for (auto _ : state) {
    for (const auto &p : problems) {
      auto res = paths(p.x, p.y);
      assert(res == p.out);
      benchmark::DoNotOptimize(res);
    }
  }
}

BENCHMARK(finding_paths);

static void shot_paths(benchmark::State &state) {

  for (auto _ : state) {
    auto res = paths(problems[0].x, problems[0].y);
    assert(res == problems[0].out);
    benchmark::DoNotOptimize(res);
  }
}

BENCHMARK(shot_paths);

static void long_paths(benchmark::State &state) {

  for (auto _ : state) {
    auto res = paths(problems[8].x, problems[8].y);
    assert(res == problems[8].out);
    benchmark::DoNotOptimize(res);
  }
}

BENCHMARK(long_paths);

static void square_paths(benchmark::State &state) {

  for (auto _ : state) {
    auto res = paths(problems[7].x, problems[7].y);
    assert(res == problems[7].out);
    benchmark::DoNotOptimize(res);
  }
}

BENCHMARK(square_paths);

BENCHMARK_MAIN();

// paths(x, y) = (x+y)!/x!*y!
// https://mathigon.org/task/paths-on-a-grid
// https://www.mathsisfun.com/pascals-triangle.html
// https://towardsdatascience.com/understanding-combinatorics-number-of-paths-on-a-grid-bddf08e28384
// https://medium.com/@prafullcpp/nth-row-of-pascals-triangle-3f2a81abe49f
