#include <benchmark/benchmark.h>

#include <cstdio>
#include <random>
#include <vector>

constexpr const float_t cutoff{0.1f};

constexpr size_t align(size_t size, size_t alignment) {
  return (size + alignment) & ~alignment;
}

struct oop_type {
  int a;
  int b;
  int c;
  bool d;
};

static void oop_state_management_both_fn(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> int_gen(INT32_MIN, INT32_MAX);
  std::uniform_real_distribution<float> bool_gen(0, 1);

  std::vector<oop_type> vec(count);

  for (auto &t : vec) {
    t.a = int_gen(re);
    t.b = int_gen(re);
    t.c = int_gen(re);
    t.d = bool_gen(re) > cutoff;
  }
  benchmark::DoNotOptimize(vec.data());

  for (auto _ : state) {
    // function 1 do work
    for (const auto &t : vec) {
      if (t.d) {
        const int out = t.a * t.b + t.c;
        benchmark::DoNotOptimize(out);
      }
    }

    // function 2 update state
    for (auto &t : vec) {

      bool d = bool_gen(re) > cutoff;
      t.d = d;
    }

    benchmark::ClobberMemory();
  }
}

BENCHMARK(oop_state_management_both_fn)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

struct dop_type {
  int a;
  int b;
  int c;
};

static void dop_state_management_both_fn(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> int_gen(INT32_MIN, INT32_MAX);
  std::uniform_real_distribution<float> bool_gen(0, 1);

  std::vector<dop_type> vec_true[2];
  vec_true[0].resize(count);
  vec_true[1].resize(count);
  std::vector<dop_type> vec_false[2];
  vec_false[0].resize(count);
  vec_false[1].resize(count);

  int idx = 0;
  int true_count = 0;
  int false_count = 0;

  for (int i = 0; i < count; i++) {
    dop_type t;
    t.a = int_gen(re);
    t.b = int_gen(re);
    t.c = int_gen(re);
    if (bool_gen(re) > cutoff) {
      vec_true[idx][true_count++] = t;
    } else {
      vec_false[idx][false_count++] = t;
    }
  }

  benchmark::DoNotOptimize(vec_true[0].data());
  benchmark::DoNotOptimize(vec_true[1].data());
  benchmark::DoNotOptimize(vec_false[0].data());
  benchmark::DoNotOptimize(vec_false[1].data());

  for (auto _ : state) {
    // function 1 do work
    for (const auto &t : vec_true[idx]) {
      const int out = t.a * t.b + t.c;
      benchmark::DoNotOptimize(out);
    }

    // function 2 update state
    int next_idx = idx == 0 ? 1 : 0;
    int next_true_count = 0;
    int next_false_count = 0;

    for (int i = 0; i < true_count; i++) {
      bool d = bool_gen(re) > cutoff;
      if (d) {
        vec_true[next_idx][next_true_count++] = vec_true[idx][i];
      } else {
        vec_false[next_idx][next_false_count++] = vec_true[idx][i];
      }
    }
    for (int i = 0; i < false_count; i++) {
      bool d = bool_gen(re) > cutoff;
      if (d) {
        vec_true[next_idx][next_true_count++] = vec_false[idx][i];
      } else {
        vec_false[next_idx][next_false_count++] = vec_false[idx][i];
      }
    }
    idx = next_idx;
    true_count = next_true_count;
    false_count = next_false_count;

    benchmark::ClobberMemory();
  }
}

BENCHMARK(dop_state_management_both_fn)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

static void dop2_state_management_both_fn(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> int_gen(INT32_MIN, INT32_MAX);
  std::uniform_real_distribution<float> bool_gen(0, 1);

  std::vector<dop_type> vec(count);
  std::vector<bool> state_vec(count);

  int idx = 0;
  int true_count = 0;
  int false_count = 0;

  for (int i = 0; i < count; i++) {
    dop_type t;
    t.a = int_gen(re);
    t.b = int_gen(re);
    t.c = int_gen(re);

    vec[i] = t;
    state_vec[i] = (bool_gen(re) > cutoff);
  }

  benchmark::DoNotOptimize(vec.data());

  for (auto _ : state) {
    // function 1 do work
    for (const auto &t : vec) {
      const int out = t.a * t.b + t.c;
      benchmark::DoNotOptimize(out);
    }

    // function 2 update state
    for (int i = 0; i < count; i++) {
      state_vec[i] = bool_gen(re) > cutoff;
    }

    benchmark::ClobberMemory();
  }
}

BENCHMARK(dop2_state_management_both_fn)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

static void dop3_state_management_both_fn(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> int_gen(INT32_MIN, INT32_MAX);
  std::uniform_real_distribution<float> bool_gen(0, 1);

  const size_t vec_size = align(sizeof(dop_type) * count, alignof(uint64_t));
  const size_t state_size =
      align(sizeof(uint64_t) * count / 64, alignof(uint64_t));

  union mem_space {
    void *ptr;
    uintptr_t address;

    mem_space &operator+=(size_t size) {
      address += size;
      return *this;
    }
  };

  void *ptr = aligned_alloc(alignof(dop_type), vec_size + state_size);

  mem_space space{.ptr = ptr};

  struct dop_data {
    dop_type *types;
    uint64_t *states;
  } _data{static_cast<dop_type *>(space.ptr),
          static_cast<uint64_t *>((space += vec_size).ptr)};

  struct bitset {

    void set(size_t idx, bool value) {
      const size_t core = idx / 64;
      const size_t bit = idx - core * 64;
      data[core] &= ~(1 << bit);
      data[core] |= (value << bit);
    }

    bool get(size_t idx) const {
      const size_t core = idx / 64;
      const size_t bit = idx - core * 64;

      return data[core] & (1 << bit);
    }

    uint64_t *data;
  } _bitset{_data.states};

  for (int i = 0; i < count; i++) {
    dop_type t;
    t.a = int_gen(re);
    t.b = int_gen(re);
    t.c = int_gen(re);

    _data.types[i] = t;
    _bitset.set(i, bool_gen(re) > cutoff);
  }

  benchmark::DoNotOptimize(ptr);

  for (auto _ : state) {
    // function 1 do work
    for (int i = 0; i < count; i++) {
      const auto &t = _data.types[i];
      const int out = t.a * t.b + t.c;
      benchmark::DoNotOptimize(out);
    }

    // function 2 update state
    for (int i = 0; i < count; i++) {
      _bitset.set(i, bool_gen(re) > cutoff);
    }

    benchmark::ClobberMemory();
  }

  free(ptr);
}

BENCHMARK(dop3_state_management_both_fn)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

static void oop_state_management_compute_fn(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> int_gen(INT32_MIN, INT32_MAX);
  std::uniform_real_distribution<float> bool_gen(0, 1);

  std::vector<oop_type> vec(count);

  for (auto &t : vec) {
    t.a = int_gen(re);
    t.b = int_gen(re);
    t.c = int_gen(re);
    t.d = bool_gen(re) > cutoff;
  }
  benchmark::DoNotOptimize(vec.data());

  for (auto _ : state) {
    // function 1 do work
    for (const auto &t : vec) {
      if (t.d) {
        const int out = t.a * t.b + t.c;
        benchmark::DoNotOptimize(out);
      }
    }
    benchmark::ClobberMemory();
  }
}

BENCHMARK(oop_state_management_compute_fn)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

static void dop_state_management_compute_fn(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> int_gen(INT32_MIN, INT32_MAX);
  std::uniform_real_distribution<float> bool_gen(0, 1);

  std::vector<dop_type> vec_true[2];
  vec_true[0].reserve(count);
  vec_true[1].reserve(count);
  std::vector<dop_type> vec_false[2];
  vec_false[0].reserve(count);
  vec_false[1].reserve(count);

  int idx = 0;

  for (int i = 0; i < count; i++) {
    dop_type t;
    t.a = int_gen(re);
    t.b = int_gen(re);
    t.c = int_gen(re);
    if (bool_gen(re) > cutoff) {
      vec_true[idx].push_back(t);
    } else {
      vec_false[idx].push_back(t);
    }
  }

  benchmark::DoNotOptimize(vec_true[0].data());
  benchmark::DoNotOptimize(vec_true[1].data());
  benchmark::DoNotOptimize(vec_false[0].data());
  benchmark::DoNotOptimize(vec_false[1].data());

  for (auto _ : state) {
    // function 1 do work
    for (const auto &t : vec_true[idx]) {
      const int out = t.a * t.b + t.c;
      benchmark::DoNotOptimize(out);
    }

    benchmark::ClobberMemory();
  }
}

BENCHMARK(dop_state_management_compute_fn)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

static void dop2_state_management_compute_fn(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> int_gen(INT32_MIN, INT32_MAX);
  std::uniform_real_distribution<float> bool_gen(0, 1);

  std::vector<dop_type> vec(count);
  std::vector<bool> state_vec(count);

  int idx = 0;
  int true_count = 0;
  int false_count = 0;

  for (int i = 0; i < count; i++) {
    dop_type t;
    t.a = int_gen(re);
    t.b = int_gen(re);
    t.c = int_gen(re);

    vec[i] = t;
    state_vec[i] = (bool_gen(re) > cutoff);
  }

  benchmark::DoNotOptimize(vec.data());

  for (auto _ : state) {
    // function 1 do work
    for (const auto &t : vec) {
      const int out = t.a * t.b + t.c;
      benchmark::DoNotOptimize(out);
    }

    benchmark::ClobberMemory();
  }
}

BENCHMARK(dop2_state_management_compute_fn)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

static void dop3_state_management_compute_fn(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> int_gen(INT32_MIN, INT32_MAX);
  std::uniform_real_distribution<float> bool_gen(0, 1);

  const size_t vec_size = align(sizeof(dop_type) * count, alignof(uint64_t));
  const size_t state_size =
      align(sizeof(uint64_t) * count / 64, alignof(uint64_t));

  union mem_space {
    void *ptr;
    uintptr_t address;

    mem_space &operator+=(size_t size) {
      address += size;
      return *this;
    }
  };

  void *ptr = aligned_alloc(alignof(dop_type), vec_size + state_size);

  mem_space space{.ptr = ptr};

  struct dop_data {
    dop_type *types;
    uint64_t *states;
  } _data{static_cast<dop_type *>(space.ptr),
          static_cast<uint64_t *>((space += vec_size).ptr)};

  struct bitset {

    void set(size_t idx, bool value) {
      const size_t core = idx / 64;
      const size_t bit = idx - core * 64;
      data[core] &= ~(1 << bit);
      data[core] |= (value << bit);
    }

    bool get(size_t idx) const {
      const size_t core = idx / 64;
      const size_t bit = idx - core * 64;

      return data[core] & (1 << bit);
    }

    uint64_t *data;
  } _bitset{_data.states};

  for (int i = 0; i < count; i++) {
    dop_type t;
    t.a = int_gen(re);
    t.b = int_gen(re);
    t.c = int_gen(re);

    _data.types[i] = t;
    _bitset.set(i, bool_gen(re) > cutoff);
  }

  benchmark::DoNotOptimize(ptr);

  for (auto _ : state) {
    // function 1 do work
    for (int i = 0; i < count; i++) {
      const auto &t = _data.types[i];
      const int out = t.a * t.b + t.c;
      benchmark::DoNotOptimize(out);
    }

    benchmark::ClobberMemory();
  }

  free(ptr);
}

BENCHMARK(dop3_state_management_compute_fn)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

static void oop_state_management_update_fn(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> int_gen(INT32_MIN, INT32_MAX);
  std::uniform_real_distribution<float> bool_gen(0, 1);

  std::vector<oop_type> vec(count);

  uint64_t tc = 0;
  uint64_t fc = 0;

  for (auto &t : vec) {
    t.a = int_gen(re);
    t.b = int_gen(re);
    t.c = int_gen(re);
    t.d = bool_gen(re) > cutoff;
    if (t.d)
      tc++;
    else
      fc++;
  }
  benchmark::DoNotOptimize(vec.data());

  for (auto _ : state) {
    // function 2 update state
    for (auto &t : vec) {
      t.d = bool_gen(re) > cutoff;
    }
    benchmark::DoNotOptimize(vec.data());

    benchmark::ClobberMemory();
  }
}

BENCHMARK(oop_state_management_update_fn)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

static void dop_state_management_update_fn(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> int_gen(INT32_MIN, INT32_MAX);
  std::uniform_real_distribution<float> bool_gen(0, 1);

  std::vector<dop_type> vec_true[2];
  vec_true[0].resize(count);
  vec_true[1].resize(count);
  std::vector<dop_type> vec_false[2];
  vec_false[0].resize(count);
  vec_false[1].resize(count);

  int idx = 0;
  int true_count = 0;
  int false_count = 0;

  for (int i = 0; i < count; i++) {
    dop_type t;
    t.a = int_gen(re);
    t.b = int_gen(re);
    t.c = int_gen(re);
    if (bool_gen(re) > cutoff) {
      vec_true[idx][true_count++] = t;
    } else {
      vec_false[idx][false_count++] = t;
    }
  }

  benchmark::DoNotOptimize(vec_true[0].data());
  benchmark::DoNotOptimize(vec_true[1].data());
  benchmark::DoNotOptimize(vec_false[0].data());
  benchmark::DoNotOptimize(vec_false[1].data());

  for (auto _ : state) {

    // function 2 update state
    int next_idx = idx == 0 ? 1 : 0;
    int next_true_count = 0;
    int next_false_count = 0;

    for (int i = 0; i < true_count; i++) {
      bool d = bool_gen(re) > cutoff;
      if (d) {
        vec_true[next_idx][next_true_count++] = vec_true[idx][i];
      } else {
        vec_false[next_idx][next_false_count++] = vec_true[idx][i];
      }
    }
    for (int i = 0; i < false_count; i++) {
      bool d = bool_gen(re) > cutoff;
      if (d) {
        vec_true[next_idx][next_true_count++] = vec_false[idx][i];
      } else {
        vec_false[next_idx][next_false_count++] = vec_false[idx][i];
      }
    }
    idx = next_idx;
    true_count = next_true_count;
    false_count = next_false_count;
    benchmark::DoNotOptimize(vec_true[idx].data());
    benchmark::DoNotOptimize(vec_false[idx].data());

    benchmark::ClobberMemory();
  }
}

BENCHMARK(dop_state_management_update_fn)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

static void dop2_state_management_update_fn(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> int_gen(INT32_MIN, INT32_MAX);
  std::uniform_real_distribution<float> bool_gen(0, 1);

  std::vector<dop_type> vec(count);
  std::vector<bool> state_vec(count);

  int idx = 0;
  int true_count = 0;
  int false_count = 0;

  for (int i = 0; i < count; i++) {
    dop_type t;
    t.a = int_gen(re);
    t.b = int_gen(re);
    t.c = int_gen(re);

    vec[i] = t;
    state_vec[i] = (bool_gen(re) > cutoff);
  }

  benchmark::DoNotOptimize(vec.data());

  for (auto _ : state) {

    // function 2 update state
    for (int i = 0; i < count; i++) {
      state_vec[i] = bool_gen(re) > cutoff;
    }

    benchmark::ClobberMemory();
  }
}

BENCHMARK(dop2_state_management_update_fn)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

static void dop3_state_management_update_fn(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> int_gen(INT32_MIN, INT32_MAX);
  std::uniform_real_distribution<float> bool_gen(0, 1);

  const size_t vec_size = align(sizeof(dop_type) * count, alignof(uint64_t));
  const size_t state_size =
      align(sizeof(uint64_t) * count / 64, alignof(uint64_t));

  union mem_space {
    void *ptr;
    uintptr_t address;

    mem_space &operator+=(size_t size) {
      address += size;
      return *this;
    }
  };

  void *ptr = aligned_alloc(alignof(dop_type), vec_size + state_size);

  mem_space space{.ptr = ptr};

  struct dop_data {
    dop_type *types;
    uint64_t *states;
  } _data{static_cast<dop_type *>(space.ptr),
          static_cast<uint64_t *>((space += vec_size).ptr)};

  struct bitset {

    void set(size_t idx, bool value) {
      const size_t core = idx / 64;
      const size_t bit = idx - core * 64;
      data[core] &= ~(1 << bit);
      data[core] |= (value << bit);
    }

    bool get(size_t idx) const {
      const size_t core = idx / 64;
      const size_t bit = idx - core * 64;

      return data[core] & (1 << bit);
    }

    uint64_t *data;
  } _bitset{_data.states};

  for (int i = 0; i < count; i++) {
    dop_type t;
    t.a = int_gen(re);
    t.b = int_gen(re);
    t.c = int_gen(re);

    _data.types[i] = t;
    _bitset.set(i, bool_gen(re) > cutoff);
  }

  benchmark::DoNotOptimize(ptr);

  for (auto _ : state) {

    // function 2 update state
    for (int i = 0; i < count; i++) {
      _bitset.set(i, bool_gen(re) > cutoff);
    }

    benchmark::DoNotOptimize(_bitset.data);

    benchmark::ClobberMemory();
  }

  free(ptr);
}

BENCHMARK(dop3_state_management_update_fn)
    ->Arg(10)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000)
    ->Arg(10000000);

BENCHMARK_MAIN();