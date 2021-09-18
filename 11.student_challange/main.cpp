#include <benchmark/benchmark.h>

#include <array>
#include <cstring>
#include <string>

/* https://edabit.com/challenge/rZbXfisPPKbR6yNXn
 * Replace Letters With Position In Alphabet
 * Create a function that takes a string and replaces each letter with its
 * appropriate position in the alphabet. "a" is 1, "b" is 2, "c" is 3, etc, etc.
 *
 * If any character in the string isn't a letter, ignore it.
 */

std::array tests = {
    "Wow, does that work?",
    "We have a lot of rain in June.",
    "Check back tomorrow; I will see if the book has arrived.",
    "Wednesday is hump day, but has anyone asked the camel if "
    "he’s happy about it?",
    "Sixty-Four comes asking for bread.",
    "The#b00k$is*in^fr0nt#0f!the_taBle.",
    "Lets all be unique together until we realise we are all the same.",
    "The river stole the gods.",
    "If Purple People Eaters are real… where do they "
    "find purple people to eat?",
    "Abstraction is often one floor above you."};

std::array results = {
    "23 15 23 4 15 5 19 20 8 1 20 23 15 18 11",
    "23 5 8 1 22 5 1 12 15 20 15 6 18 1 9 14 9 14 10 21 14 5",
    "3 8 5 3 11 2 1 3 11 20 15 13 15 18 18 15 23 9 23 9 12 12 19 5 5 "
    "9 6 20 8 5 2 15 15 11 8 1 19 1 18 18 9 22 5 4",
    "23 5 4 14 5 19 4 1 25 9 19 8 21 13 16 4 1 25 2 21 20 8 "
    "1 19 1 14 25 15 14 5 1 19 11 5 4 20 8 5 3 1 13 5 12 9 6 "
    "8 5 19 8 1 16 16 25 1 2 15 21 20 9 20",
    "19 9 24 20 25 6 15 21 18 3 15 13 5 19 1 19 11 9 14 7 6 "
    "15 18 2 18 5 1 4",
    "20 8 5 2 11 9 19 9 14 6 18 14 20 6 20 8 5 20 1 2 12 5",
    "12 5 20 19 1 12 12 2 5 21 14 9 17 21 5 20 15 7 5 20 8 5 18 21 14 20 "
    "9 12 23 5 18 5 1 12 9 19 5 23 5 1 18 5 1 12 12 20 8 5 19 1 13 5",
    "20 8 5 18 9 22 5 18 19 20 15 12 5 20 8 5 7 15 4 19",
    "9 6 16 21 18 16 12 5 16 5 15 16 12 5 5 1 20 5 18 19 1 "
    "18 5 18 5 1 12 23 8 5 18 5 4 15 20 8 5 25 6 9 14 4 16 "
    "21 18 16 12 5 16 5 15 16 12 5 20 15 5 1 20",
    "1 2 19 20 18 1 3 20 9 15 14 9 19 15 6 20 5 14 15 14 5 6 "
    "12 15 15 18 1 2 15 22 5 25 15 21"};

std::string alphabetIndex_naive(std::string_view input) {

  std::string res;
  res.reserve(input.size() * 3);
  for (auto c : input) {
    switch (c) {
    case 'a':
    case 'A':
      res += "1 ";
      break;
    case 'b':
    case 'B':
      res += "2 ";
      break;
    case 'c':
    case 'C':
      res += "3 ";
      break;
    case 'd':
    case 'D':
      res += "4 ";
      break;
    case 'e':
    case 'E':
      res += "5 ";
      break;
    case 'f':
    case 'F':
      res += "6 ";
      break;
    case 'g':
    case 'G':
      res += "7 ";
      break;
    case 'h':
    case 'H':
      res += "8 ";
      break;
    case 'i':
    case 'I':
      res += "9 ";
      break;
    case 'j':
    case 'J':
      res += "10 ";
      break;
    case 'k':
    case 'K':
      res += "11 ";
      break;
    case 'l':
    case 'L':
      res += "12 ";
      break;
    case 'm':
    case 'M':
      res += "13 ";
      break;
    case 'n':
    case 'N':
      res += "14 ";
      break;
    case 'o':
    case 'O':
      res += "15 ";
      break;
    case 'p':
    case 'P':
      res += "16 ";
      break;
    case 'q':
    case 'Q':
      res += "17 ";
      break;
    case 'r':
    case 'R':
      res += "18 ";
      break;
    case 's':
    case 'S':
      res += "19 ";
      break;
    case 't':
    case 'T':
      res += "20 ";
      break;
    case 'u':
    case 'U':
      res += "21 ";
      break;
    case 'v':
    case 'V':
      res += "22 ";
      break;
    case 'w':
    case 'W':
      res += "23 ";
      break;
    case 'x':
    case 'X':
      res += "24 ";
      break;
    case 'y':
    case 'Y':
      res += "25 ";
      break;
    case 'z':
    case 'Z':
      res += "26 ";
      break;

    default:
      break;
    }
  }
  if (!res.empty()) {
    res.erase(res.end() - 1);
  }
  return res;
}

static void naive(benchmark::State &state) {

  for (auto _ : state) {
    for (int i = 0, l = tests.size(); i < l; i++) {
      const auto res = alphabetIndex_naive(tests[i]);
      benchmark::DoNotOptimize(res);
#ifndef NDEBUG
      if (strcmp(res.c_str(), results[i]) != 0) {
        printf("[naive] ERROR - Testcase: %s, Expexted: %s, Got: %s\n",
               tests[i], results[i], res.c_str());

        return;
      }
#endif
    }
  }
}

std::string alphabetIndex_pro(std::string_view input) {

  std::string res;
  res.reserve(input.size() * 3);
  for (auto c : input) {
    if (c >= 'a' && c <= 'z') {
      res += std::to_string(1 + c - 'a') + ' ';
    } else if (c >= 'A' && c <= 'Z') {
      res += std::to_string(1 + c - 'A') + ' ';
    }
  }
  if (!res.empty()) {
    res.erase(res.end() - 1);
  }
  return res;
}

static void pro(benchmark::State &state) {

  for (auto _ : state) {
    for (int i = 0, l = tests.size(); i < l; i++) {
      const auto res = alphabetIndex_pro(tests[i]);
      benchmark::DoNotOptimize(res);
#ifndef NDEBUG
      if (strcmp(res.c_str(), results[i]) != 0) {
        printf("[pro] ERROR - Testcase: %s, Expexted: %s, Got: %s\n", tests[i],
               results[i], res.c_str());

        return;
      }
#endif
    }
  }
}

uint8_t min(uint8_t x, uint8_t y) {
  return y ^ ((x ^ y) & -(x < y)); // min(x, y)
}

constexpr const char *valueMap[] = {
    "1 ",  "2 ",  "3 ",  "4 ",  "5 ",  "6 ",  "7 ",  "8 ",  "9 ",
    "10 ", "11 ", "12 ", "13 ", "14 ", "15 ", "16 ", "17 ", "18 ",
    "19 ", "20 ", "21 ", "22 ", "23 ", "24 ", "25 ", "26 "};

std::string alphabetIndex_expert(std::string_view input) {

  std::string res;
  res.reserve(input.size() * 3);
  for (int i = 0, l = input.size(); i < l; i++) {
    char c = input[i];
    uint8_t lower = uint8_t(c) - 'a';
    uint8_t upper = uint8_t(c) - 'A';
    uint8_t val = min(lower, upper);
    if (val < 27) {
      auto str = valueMap[val];
      res.insert(res.size(), str, strlen(str));
    }
  }
  if (!res.empty()) {
    res.erase(res.end() - 1);
  }

  return res;
}

static void expert(benchmark::State &state) {

  for (auto _ : state) {
    for (int i = 0, l = tests.size(); i < l; i++) {
      const auto res = alphabetIndex_expert(tests[i]);
      benchmark::DoNotOptimize(res);
#ifndef NDEBUG
      if (strcmp(res.c_str(), results[i]) != 0) {
        printf("[expert] ERROR - Testcase: %s, Expexted: %s, Got: %s\n",
               tests[i], results[i], res.c_str());

        return;
      }
#endif
    }
  }
}

BENCHMARK(naive);
BENCHMARK(pro);
BENCHMARK(expert);

BENCHMARK_MAIN();