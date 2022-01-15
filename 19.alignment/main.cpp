#include <benchmark/benchmark.h>

#include <cstdio>
#include <vector>

struct my_type {
  int a;  // offset 0, 4 bytes
  int b;  // offset 4, 4 bytes
  bool c; // offset 8, 1 byte
  // padding 7 bytes for double alignment
  double d; // offset 16, 8 bytes
  char *e;  // offset 24, 8 bytes
};

// sizeof(my_type) = 32 bytes

struct __attribute__((packed)) my_type2 {
  int a;    // offset 0, 4 bytes
  int b;    // offset 4, 4 bytes
  double d; // offset 8, 8 bytes
  char *e;  // offset 16, 8 bytes
  bool c;   // offset 24, 1 byte
};

// sizeof(my_type2) = 32 bytes, 25 bytes with 8 byte alignment = 32 bytes

struct soa_type {
  int *a;    // array
  int *b;    // array
  bool *c;   // array
  double *d; // array
  char **e;  // array
};

constexpr size_t align(size_t size, size_t alignment) {
  return (size + alignment) & ~alignment;
}

soa_type create_soa_type(size_t count) {

  const size_t e_size = sizeof(char *) * count;
  const size_t d_size = align(sizeof(double) * count, alignof(char *));
  const size_t c_size = align(sizeof(bool) * count, alignof(double));
  const size_t b_size = align(sizeof(int) * count, alignof(bool));
  const size_t a_size = align(sizeof(int) * count, alignof(int));

  const size_t alignment = alignof(double);
  const size_t size = a_size + b_size + c_size + d_size + e_size;

  union mem_space {
    void *ptr;
    uintptr_t address;

    mem_space &operator+=(size_t size) {
      address += size;
      return *this;
    }
  };

  mem_space space{.ptr = aligned_alloc(alignment, size)};

  soa_type soa{
      static_cast<int *>(space.ptr),                // a
      static_cast<int *>((space += a_size).ptr),    // b
      static_cast<bool *>((space += b_size).ptr),   // c
      static_cast<double *>((space += c_size).ptr), // d
      static_cast<char **>((space += d_size).ptr),  // e
  };

  return soa;
}

void destroy_soa_type(soa_type soa) { free(soa.a); }

static void alignment_effects_1(benchmark::State &state) {

  std::vector<my_type> a(state.range(0));
  benchmark::DoNotOptimize(a.data());

  for (auto _ : state) {
    for (const auto &v : a) {

      if (v.c) {
        auto res = v.a * v.d;
        benchmark::DoNotOptimize(res);
        if (res < 0) {
          // error
          auto err = v.e;
          benchmark::DoNotOptimize(err);
        }
      } else {
        auto res = v.b / v.d;
        benchmark::DoNotOptimize(res);
        if (res < 0) {
          // error
          auto err = v.e;
          benchmark::DoNotOptimize(err);
        }
      }
      benchmark::ClobberMemory();
    }
  }
}

BENCHMARK(alignment_effects_1)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

static void alignment_effects_2(benchmark::State &state) {

  std::vector<my_type2> a(state.range(0));
  benchmark::DoNotOptimize(a.data());

  for (auto _ : state) {
    for (const auto &v : a) {

      if (v.c) {
        auto res = v.a * v.d;
        benchmark::DoNotOptimize(res);
        if (res < 0) {
          // error
          auto err = v.e;
          benchmark::DoNotOptimize(err);
        }
      } else {
        auto res = v.b / v.d;
        benchmark::DoNotOptimize(res);
        if (res < 0) {
          // error
          auto err = v.e;
          benchmark::DoNotOptimize(err);
        }
      }
      benchmark::ClobberMemory();
    }
  }
}

BENCHMARK(alignment_effects_2)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

static void alignment_effects_3(benchmark::State &state) {

  const auto size = state.range(0);

  soa_type a = create_soa_type(size);

  benchmark::DoNotOptimize(a.a);

  for (auto _ : state) {
    for (size_t i = 0; i < size; i++) {

      if (a.c[i]) {
        auto res = a.a[i] * a.d[i];
        benchmark::DoNotOptimize(res);
        if (res < 0) {
          // error
          auto err = a.e[i];
          benchmark::DoNotOptimize(err);
        }
      } else {
        auto res = a.b[i] / a.d[i];
        benchmark::DoNotOptimize(res);
        if (res < 0) {
          // error
          auto err = a.e[i];
          benchmark::DoNotOptimize(err);
        }
      }
      benchmark::ClobberMemory();
    }
  }

  destroy_soa_type(a);
}

BENCHMARK(alignment_effects_3)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

BENCHMARK_MAIN();