#include <benchmark/benchmark.h>

#include <array>
#include <cstring>
#include <random>
#include <string>
#include <vector>

enum Event {
  ADD, // Add operation
  SUB, // Sub operation
  MUL, // Mul operation
  DIV, // Div operation
  // MOD, // Mod operation
  END // End tag
};

static void bench_switch(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(ADD, END - 1);

  std::vector<Event> data(count + 1);
  for (auto &e : data) {
    e = static_cast<Event>(gen(re));
  }
  data[count] = END;

  for (auto _ : state) {
    int64_t value = 0;
    for (auto e : data) {
      switch (e) {
      case ADD: {
        value += 1;
        continue;
      }
      case SUB: {
        value -= 1;
        continue;
      }
      case MUL: {
        value *= 2;
        continue;
      }
      case DIV: {
        value /= 2;
        continue;
      }
        // case MOD: {
        //   value %= 2;
        //   continue;
        // }
      }
    }
    benchmark::DoNotOptimize(value);
  }
}

static void bench_goto(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(ADD, END - 1);

  std::vector<Event> data(count + 1);
  for (auto &e : data) {
    e = static_cast<Event>(gen(re));
  }
  data[count] = END;

  constexpr void *jump_targets[]{
      [ADD] = &&ADD_LBL, // Add jump target
      [SUB] = &&SUB_LBL, // Sub jump target
      [MUL] = &&MUL_LBL, // Mul jump target
      [DIV] = &&DIV_LBL, // Div jump target
      // [MOD] = &&MOD_LBL, // Mod jump target
      [END] = &&END_LBL // End jump target
  };

  for (auto _ : state) {
    int64_t value = 0;
    Event *eve = data.data();
    const auto next = [&]() { return jump_targets[*eve++]; };
    goto *next();

  ADD_LBL:
    value += 1;
    goto *next();

  SUB_LBL:
    value -= 1;
    goto *next();

  MUL_LBL:
    value *= 2;
    goto *next();

  DIV_LBL:
    value /= 2;
    goto *next();

    // MOD_LBL:
    //   value %= 2;
    //   goto *next();

  END_LBL:
    benchmark::DoNotOptimize(value);
  }
}

BENCHMARK(bench_switch)->Arg(100);
BENCHMARK(bench_switch)->Arg(1000);
BENCHMARK(bench_switch)->Arg(10000);
BENCHMARK(bench_switch)->Arg(100000);

BENCHMARK(bench_goto)->Arg(100);
BENCHMARK(bench_goto)->Arg(1000);
BENCHMARK(bench_goto)->Arg(10000);
BENCHMARK(bench_goto)->Arg(100000);

BENCHMARK_MAIN();