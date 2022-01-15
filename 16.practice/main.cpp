#include <benchmark/benchmark.h>

#include <algorithm>
#include <atomic>
#include <cstring>
#include <mutex>
#include <numeric>
#include <random>
#include <string>
#include <thread>
#include <vector>

struct input_t {
  std::vector<std::vector<int64_t>> data;
  bool expected;
};

input_t tests[] = {
    input_t{{{8, 1, 6}, {3, 5, 7}, {4, 9, 2}}, true},
    input_t{{{16, 3, 2, 13}, {5, 10, 11, 8}, {9, 6, 7, 12}, {4, 15, 14, 1}},
            true},
    input_t{{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, false}};

static void magic_square_attempt_1(benchmark::State &state) {

  const auto isMagicSquare = [](const input_t &input) {
    // 3x3 = 3 down + 3 accorss + 2 diagnal??

    std::vector<int64_t> sums;
    sums.reserve(input.data.size() + input.data.size() + 2);

    // accross rows
    for (const auto &row : input.data) {
      sums.push_back(std::accumulate(row.begin(), row.end(), 0));
    }

    // accross columns
    for (auto l = input.data.size(), i = 0ul; i < l; i++) {
      int64_t sum = 0;
      for (decltype(l) j = 0; j < l; j++) {
        sum += input.data[j][i];
      }
      sums.push_back(sum);
    }

    // diagnal 1
    {
      int sum = 0;
      for (auto l = input.data.size(), i = 0ul; i < l; i++) {
        sum += input.data[i][i];
      }
      sums.push_back(sum);
    }

    // diagnal 2
    {
      int sum = 0;
      for (auto l = input.data.size(), i = 0ul; i < l; i++) {
        sum += input.data[i][l - (i + 1)];
      }
      sums.push_back(sum);
    }

    return std::all_of(sums.begin(), sums.end(),
                       [val = sums[0]](auto sum) { return val == sum; });
  };

  for (auto _ : state) {
    for (const auto &input : tests) {
      const auto is = isMagicSquare(input);
      // printf("Expected: %d, Got: %d\n", input.expected, is);
      benchmark::DoNotOptimize(is);
    }
  }
}

BENCHMARK(magic_square_attempt_1);

struct problem_t {
  std::vector<int64_t> data;
  int32_t dimention;
  bool expected;
};

problem_t prob[]{
    problem_t{{8, 1, 6, 3, 5, 7, 4, 9, 2}, 3, true},
    problem_t{{16, 3, 2, 13, 5, 10, 11, 8, 9, 6, 7, 12, 4, 15, 14, 1}, 4, true},
    problem_t{{1, 2, 3, 4, 5, 6, 7, 8, 9}, 3, false}};

static void magic_square_attempt_2(benchmark::State &state) {

  const auto isMagicSquare = [](const int64_t *input, int32_t dimention) {
    switch (dimention) {
    case 0:
    case 1:
      return true;

    case 2: {

      const auto row1 = input[0] + input[1];
      const auto row2 = input[2] + input[3];

      const auto col1 = input[0] + input[2];
      const auto col2 = input[1] + input[3];

      const auto diag1 = input[0] + input[3];
      const auto diag2 = input[1] + input[2];

      return (row2 & col1 & col2 & diag1 & diag2) == row1;
    }

    case 3: {

      const auto row1 = input[0] + input[1] + input[2];
      const auto row2 = input[3] + input[4] + input[5];
      const auto row3 = input[6] + input[7] + input[8];

      const auto col1 = input[0] + input[3] + input[6];
      const auto col2 = input[1] + input[4] + input[7];
      const auto col3 = input[2] + input[5] + input[8];

      const auto diag1 = input[0] + input[4] + input[8];
      const auto diag2 = input[2] + input[4] + input[6];

      return (row2 & row3 & col1 & col2 & col3 & diag1 & diag2) == row1;
    }

    case 4: {

      const auto row1 = input[0] + input[1] + input[2] + input[3];
      const auto row2 = input[4] + input[5] + input[6] + input[7];
      const auto row3 = input[8] + input[9] + input[10] + input[11];
      const auto row4 = input[12] + input[13] + input[14] + input[15];

      const auto col1 = input[0] + input[4] + input[8] + input[12];
      const auto col2 = input[1] + input[5] + input[9] + input[13];
      const auto col3 = input[2] + input[6] + input[10] + input[14];
      const auto col4 = input[3] + input[7] + input[11] + input[15];

      const auto diag1 = input[0] + input[5] + input[10] + input[15];
      const auto diag2 = input[3] + input[6] + input[9] + input[12];

      return (row2 & row3 & row4 & col1 & col2 & col3 & col4 & diag1 & diag2) ==
             row1;
    }
    }

    int64_t res = 0;

    int64_t row1 = 0;
    int64_t col1 = 0;
    int64_t diag1 = 0;
    for (int32_t i = 0; i < dimention; i++) {
      row1 += input[i];
      col1 += input[i * dimention];
      diag1 += input[i * dimention + i];
    }

    res = row1 & col1 & diag1;

    for (int32_t i = 1; i < dimention; i++) {
      int64_t row = 0;
      int64_t col = 0;
      for (int32_t j = 0; j < dimention; j++) {
        row += input[i * dimention + j];
        col += input[i + j * dimention];
      }
      res = res & row & col;
    }

    int64_t diag2 = 0;
    for (int32_t i = 0, j = dimention - 1; j >= 0; i++, j--) {
      diag2 += input[i * dimention + j];
    }

    return (res & diag2) == row1;

    return false;
  };

  for (auto _ : state) {
    for (const auto &input : prob) {
      const auto is = isMagicSquare(input.data.data(), input.dimention);
      // printf("Expected: %d, Got: %d\n", input.expected, is);
      benchmark::DoNotOptimize(is);
    }
  }
}

BENCHMARK(magic_square_attempt_2);

BENCHMARK_MAIN();