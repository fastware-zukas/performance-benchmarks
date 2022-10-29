#include <benchmark/benchmark.h>

#include <bitset>
#include <cmath>
#include <cstdio>
#include <vector>

struct Inputs {
  uint32_t N;
  uint32_t S : 31;
  uint32_t P : 31;
  uint32_t Q : 31;
};

struct TestCase {
  Inputs inputs;
  uint32_t output;
};

constexpr TestCase testCases[]{
    {{10, 1972857628, 859433109, 437173653}, 10},
    {{10000, 573272257, 859433109, 1894136138}, 10000},
    {{10000000, 658061970, 695098531, 1430548937}, 10000000},
    {{100000000, 923092883, 976061291, 1205350525}, 100000000},
    {{100000000, 1506922183, 1236189611, 306853238}, 100000000},
    {{100000000, 569099406, 1607140150, 823906344}, 31},
    {{100000000, 831602480, 704408287, 1134515747}, 100000000},
    {{100000000, 178349423, 584642695, 2037647927}, 100000000},
    {{100000000, 1232077670, 126810854, 1536183938}, 26},
    {{100000000, 2065188356, 657733125, 1636844041}, 100000000},
    {{3, 1, 1, 1}, 3}

};

static void bitset_1(benchmark::State &state) {

  constexpr auto fn = [](uint32_t N, uint32_t S, uint32_t P, uint32_t Q) {
    constexpr uint32_t POW{2147483648ul}; // 2 ^ 31

    uint32_t first = S;
    uint32_t a = (S * P + Q) % POW;
    uint32_t prev = a;

    for (int i = 1; i < N; i++) {
      a = (a * P + Q) % POW;
      if (a == first || a == prev) {
        return uint32_t(i + 1);
      }
      prev = a;
    }
    return N;
  };

  for (auto _ : state) {
    for (auto tc : testCases) {
      auto c = fn(tc.inputs.N, tc.inputs.S, tc.inputs.P, tc.inputs.Q);
      if (c != tc.output) {
        printf("N:%u S:%u P:%u Q:%u\n", tc.inputs.N, tc.inputs.S, tc.inputs.P,
               tc.inputs.Q);
        printf("Expected: %u, Got: %u\n", tc.output, c);
        return;
      }
    }
  }
}

BENCHMARK(bitset_1);

static void bitset_2(benchmark::State &state) {

  constexpr auto fn = [](uint32_t N, uint32_t S, uint32_t P, uint32_t Q) {
    uint32_t first = S;
    uint32_t a = std::bitset<31>(S * P + Q).to_ulong();
    uint32_t prev = a;

    for (int i = 1; i < N; i++) {
      a = std::bitset<31>(a * P + Q).to_ulong();
      if (a == first || a == prev) {
        return uint32_t(i + 1);
      }
      prev = a;
    }
    return N;
  };

  for (auto _ : state) {
    for (auto tc : testCases) {
      auto c = fn(tc.inputs.N, tc.inputs.S, tc.inputs.P, tc.inputs.Q);
      if (c != tc.output) {
        printf("N:%u S:%u P:%u Q:%u\n", tc.inputs.N, tc.inputs.S, tc.inputs.P,
               tc.inputs.Q);
        printf("Expected: %u, Got: %u\n", tc.output, c);
        return;
      }
    }
  }
}

BENCHMARK(bitset_2);

static void bitset_3(benchmark::State &state) {

  constexpr auto fn = [](uint32_t N, uint32_t S, uint32_t P, uint32_t Q) {
    constexpr uint32_t POW{2147483647ul}; // 2 ^ 31 - 1
    uint32_t first = S;
    uint32_t a = (S * P + Q) & POW;
    uint32_t prev = a;

    for (int i = 1; __glibc_likely(i < N); i++) {
      a = (a * P + Q) & POW;
      if (__glibc_unlikely(a == first | a == prev)) {
        return uint32_t(i + 1);
      }
      prev = a;
    }
    return N;
  };

  for (auto _ : state) {
    for (auto tc : testCases) {
      auto c = fn(tc.inputs.N, tc.inputs.S, tc.inputs.P, tc.inputs.Q);
      if (c != tc.output) {
        printf("N:%u S:%u P:%u Q:%u\n", tc.inputs.N, tc.inputs.S, tc.inputs.P,
               tc.inputs.Q);
        printf("Expected: %u, Got: %u\n", tc.output, c);
        return;
      }
    }
  }
}

BENCHMARK(bitset_3);

BENCHMARK_MAIN();