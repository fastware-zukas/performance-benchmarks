#include <benchmark/benchmark.h>
#include <charconv>
#include <cstdlib>
#include <cstring>
#include <random>
#include <utility>

class string_data_fixture : public ::benchmark::Fixture {
public:
  void SetUp(const ::benchmark::State &st) {
    int digits = st.range(0);
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<> dis{0, 9};

    values.reserve(1000);
    for (int n = 0; n < 1000; ++n) {
      int res = 0;
      for (int i = 0; i < digits; i++) {
        res = res * 10 + dis(gen);
      }
      if (dis(gen) >= 5) {
        res *= -1;
      }
      values.emplace_back(std::to_string(res), res);
    }
  }

  void TearDown(const ::benchmark::State &) { values.clear(); }

  std::vector<std::pair<std::string, int>> values;
};

#define BENCH(test)                                                            \
  BENCHMARK_REGISTER_F(string_data_fixture, test)                              \
      ->Arg(1)                                                                 \
      ->Arg(2)                                                                 \
      ->Arg(3)                                                                 \
      ->Arg(4)                                                                 \
      ->Arg(5)                                                                 \
      ->Arg(6)                                                                 \
      ->Arg(7)                                                                 \
      ->Arg(8)                                                                 \
      ->Arg(9)                                                                 \
      ->Arg(10);

/**
 * http://www.geeksforgeeks.org/write-your-own-atoi/
 **/
// A utility function to check whether x is numeric
bool is_num(char x) { return (x >= '0' && x <= '9') ? true : false; }

// A simple atoi() function. If the given string contains
// any invalid character, then this function returns 0
int geeksforgeeks_atoi(const char *str) {
  if (str == nullptr)
    return 0;

  int res = 0;  // Initialize result
  int sign = 1; // Initialize sign as positive
  int i = 0;    // Initialize index of first digit

  // If number is negative, then update sign
  if (str[0] == '-') {
    sign = -1;
    i++; // Also update index of first digit
  }

  // Iterate through all digits of input string and update result
  for (; str[i] != '\0'; ++i) {
    if (is_num(str[i]) == false)
      return 0; // You may add some lines to write error message
                // to error stream
    res = res * 10 + str[i] - '0';
  }

  // Return result with sign
  return sign * res;
}

BENCHMARK_DEFINE_F(string_data_fixture, geeksforgeeks_atoi)
(benchmark::State &state) {

  for (auto _ : state) {
    for (const auto &[str, integer] : values) {
      const auto val = geeksforgeeks_atoi(str.c_str());
      assert(val == integer);
      benchmark::DoNotOptimize(&val);
    }
  }
}

BENCH(geeksforgeeks_atoi);

/*


























 */

// Baseline 1
BENCHMARK_DEFINE_F(string_data_fixture, std_atoi)(benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &[str, integer] : values) {
      const auto val = atoi(str.c_str());
      assert(val == integer);
      benchmark::DoNotOptimize(&val);
    }
  }
}

BENCH(std_atoi);

/*


























 */

// Baseline 2
BENCHMARK_DEFINE_F(string_data_fixture, std_from_chars)
(benchmark::State &state) {

  for (auto _ : state) {
    for (const auto &[str, integer] : values) {
      int val = 0;
      std::from_chars(str.c_str(), str.c_str() + str.length(), val);
      assert(val == integer);
      benchmark::DoNotOptimize(&val);
    }
  }
}

BENCH(std_from_chars);

/*


























 */

int my_atoi(const char *str) {
  if (str == nullptr)
    return 0;

  int res = 0;  // Initialize result
  int sign = 1; // Initialize sign as positive

  // If number is negative, then update sign
  if (*str == '-') {
    sign = -1;
    str++; // Also update index of first digit
  }

  // Iterate through all digits of input string and update result
  for (uint8_t c = uint8_t(*str) - '0'; c <= 9; c = uint8_t(*++str) - '0') {
    res = res * 10 + int(c);
  }

  // Return result with sign
  return (*str == '\0' ? res * sign : 0);
}

BENCHMARK_DEFINE_F(string_data_fixture, my_atoi)(benchmark::State &state) {

  for (auto _ : state) {
    for (const auto &[str, integer] : values) {
      const auto val = my_atoi(str.c_str());
      assert(val == integer);
      benchmark::DoNotOptimize(&val);
    }
  }
}

BENCH(my_atoi);

/*


























 */

int my_atoi2(const char *begin, const char *end) {
  if ((begin == nullptr) | (end == nullptr))
    return 0;

  int res = 0;  // Initialize result
  int sign = 1; // Initialize sign as positive

  // If number is negative, then update sign
  if (*begin == '-') {
    sign = -1;
    begin++; // Also update index of first digit
  }

  constexpr int powers10[]{1000000000, 100000000, 10000000, 1000000, 100000,
                           10000,      1000,      100,      10,      1};

  constexpr int powers10_array_size = sizeof(powers10) / sizeof(int);

  const int char_count = end - begin;
  int pos = powers10_array_size - char_count;
  int i;
  for (i = 0;; i++) {
    uint8_t c = uint8_t(begin[i]) - '0';
    if (c > 9)
      break;
    res += int(c) * powers10[pos + i];
  }

  // Return result with sign
  return (begin[i] == '\0' ? res * sign : 0);
}

BENCHMARK_DEFINE_F(string_data_fixture, my_atoi2)(benchmark::State &state) {

  for (auto _ : state) {
    for (const auto &[str, integer] : values) {
      const auto val = my_atoi2(str.c_str(), str.c_str() + str.length());
      assert(val == integer);
      benchmark::DoNotOptimize(&val);
    }
  }
}

BENCH(my_atoi2);

int my_atoi3(const char *begin, const char *end) {
  if ((begin == nullptr) | (end == nullptr))
    return 0;

  int res = 0;  // Initialize result
  int sign = 1; // Initialize sign as positive

  // If number is negative, then update sign
  if (*begin == '-') {
    sign = -1;
    begin++; // Also update index of first digit
  }

  constexpr int powers10[]{1,      10,      100,      1000,      10000,
                           100000, 1000000, 10000000, 100000000, 1000000000};

  const int *pow = nullptr;
  uint8_t c = 0;
  for (c = uint8_t(*end) - '0', pow = powers10; c <= 9;
       c = uint8_t(*--end) - '0', pow++) {
    res += int(c) * *pow;
  }

  // Return result with sign
  return res * sign;
}

BENCHMARK_DEFINE_F(string_data_fixture, my_atoi3)(benchmark::State &state) {

  for (auto _ : state) {
    for (const auto &[str, integer] : values) {
      const auto val = my_atoi3(str.c_str(), str.c_str() + str.length());
      assert(val == integer);
      benchmark::DoNotOptimize(&val);
    }
  }
}

BENCH(my_atoi2);

BENCHMARK_MAIN();
