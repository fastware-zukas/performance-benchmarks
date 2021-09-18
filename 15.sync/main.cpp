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

static void bench_mutex(benchmark::State &state) {

  static std::vector<int> data;
  static std::mutex mutex;
  static struct Data {
    int min;
    int max;
    int count;
  } *store = nullptr;

  if (state.thread_index == 0) {
    const int count = state.range(0);
    std::random_device r;
    std::default_random_engine re(r());
    std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);
    data.resize(count);
    for (auto &val : data) {
      val = gen(re);
    }
    store = new Data{0, 0, 0};
  }

  for (auto _ : state) {

    for (auto val : data) {
      std::lock_guard lock(mutex);
      if (val > store->max) {
        store->max = val;
      }
      if (val < store->min) {
        store->min = val;
      }
      store->count++;
    }
    benchmark::DoNotOptimize(*store);
  }

  if (state.thread_index == 0) {
    benchmark::DoNotOptimize(*store);
    delete store;
  }
}

static void bench_atomic(benchmark::State &state) {

  static std::vector<int> data;
  static struct Data {
    std::atomic_int32_t min;
    std::atomic_int32_t max;
    std::atomic_int32_t count;
  } *store = nullptr;

  if (state.thread_index == 0) {
    const int count = state.range(0);
    std::random_device r;
    std::default_random_engine re(r());
    std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);
    data.resize(count);
    for (auto &val : data) {
      val = gen(re);
    }
    store = new Data{0, 0, 0};
  }

  for (auto _ : state) {

    for (auto val : data) {
      int32_t max = store->max.load(std::memory_order_acquire);
      while (val > max && !store->max.compare_exchange_weak(
                              max, val, std::memory_order_acq_rel)) {
      }

      int32_t min = store->min.load(std::memory_order_acquire);
      while (val < min && !store->min.compare_exchange_weak(
                              min, val, std::memory_order_acq_rel)) {
      }
      store->count.fetch_add(1, std::memory_order_relaxed);
    }
    benchmark::DoNotOptimize(*store);
  }

  if (state.thread_index == 0) {
    benchmark::DoNotOptimize(*store);
    delete store;
  }
}

static void bench_no_sync(benchmark::State &state) {

  static std::vector<int> data;

  struct Data {
    int min;
    int max;
    int count;
  };
  static std::vector<Data> store;

  if (state.thread_index == 0) {
    const int count = state.range(0);
    std::random_device r;
    std::default_random_engine re(r());
    std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);
    data.resize(count);
    for (auto &val : data) {
      val = gen(re);
    }
    store.resize(state.threads);
  }

  for (auto _ : state) {
    for (auto val : data) {
      auto &s = store[state.thread_index];
      if (val > s.max) {
        s.max = val;
      }
      if (val < s.min) {
        s.min = val;
      }
      s.count++;
    }
    benchmark::DoNotOptimize(store.data());
  }

  if (state.thread_index == 0) {

    Data result = std::accumulate(store.begin(), store.end(), Data{0, 0, 0},
                                  [](const Data &res, const Data &val) {
                                    return Data{std::min(res.min, val.min),
                                                std::max(res.max, val.max),
                                                res.count + val.count};
                                  });

    benchmark::DoNotOptimize(result);
  }
}

static void bench_no_sync_with_align(benchmark::State &state) {

  static std::vector<int> data;

  struct alignas(64) Data {
    int min;
    int max;
    int count;
  };
  static std::vector<Data> store;

  if (state.thread_index == 0) {
    const int count = state.range(0);
    std::random_device r;
    std::default_random_engine re(r());
    std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);
    data.resize(count);
    for (auto &val : data) {
      val = gen(re);
    }
    store.resize(state.threads);
  }

  for (auto _ : state) {
    for (auto val : data) {
      auto &s = store[state.thread_index];
      if (val > s.max) {
        s.max = val;
      }
      if (val < s.min) {
        s.min = val;
      }
      s.count++;
    }
    benchmark::DoNotOptimize(store.data());
  }

  if (state.thread_index == 0) {

    Data result = std::accumulate(store.begin(), store.end(), Data{0, 0, 0},
                                  [](const Data &res, const Data &val) {
                                    return Data{std::min(res.min, val.min),
                                                std::max(res.max, val.max),
                                                res.count + val.count};
                                  });

    benchmark::DoNotOptimize(result);
  }
}

#define BENCH(N)                                                               \
  BENCHMARK(bench_mutex)->Arg(100)->Threads(N);                                \
  BENCHMARK(bench_mutex)->Arg(1000)->Threads(N);                               \
  BENCHMARK(bench_mutex)->Arg(10000)->Threads(N);                              \
  BENCHMARK(bench_mutex)->Arg(100000)->Threads(N);                             \
                                                                               \
  BENCHMARK(bench_atomic)->Arg(100)->Threads(N);                               \
  BENCHMARK(bench_atomic)->Arg(1000)->Threads(N);                              \
  BENCHMARK(bench_atomic)->Arg(10000)->Threads(N);                             \
  BENCHMARK(bench_atomic)->Arg(100000)->Threads(N);                            \
                                                                               \
  BENCHMARK(bench_no_sync)->Arg(100)->Threads(N);                              \
  BENCHMARK(bench_no_sync)->Arg(1000)->Threads(N);                             \
  BENCHMARK(bench_no_sync)->Arg(10000)->Threads(N);                            \
  BENCHMARK(bench_no_sync)->Arg(100000)->Threads(N);                           \
                                                                               \
  BENCHMARK(bench_no_sync_with_align)->Arg(100)->Threads(N);                   \
  BENCHMARK(bench_no_sync_with_align)->Arg(1000)->Threads(N);                  \
  BENCHMARK(bench_no_sync_with_align)->Arg(10000)->Threads(N);                 \
  BENCHMARK(bench_no_sync_with_align)->Arg(100000)->Threads(N)

BENCH(1);
BENCH(2);
BENCH(4);
BENCH(8);

BENCHMARK_MAIN();