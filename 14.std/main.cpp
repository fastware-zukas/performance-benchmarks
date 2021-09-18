#include <benchmark/benchmark.h>

#include <list>
#include <map>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

static void append_vector_no_reserve(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);

  std::vector<int> data(count);
  for (auto &v : data) {
    v = gen(re);
  }

  for (auto _ : state) {
    std::vector<int> vec;
    for (auto val : data) {
      vec.push_back(val);
    }
    benchmark::DoNotOptimize(vec.data());
    benchmark::ClobberMemory();
  }
}

static void append_vector_reserve(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);

  std::vector<int> data(count);
  for (auto &v : data) {
    v = gen(re);
  }

  for (auto _ : state) {
    std::vector<int> vec;
    vec.reserve(data.size());
    for (auto val : data) {
      vec.push_back(val);
    }
    benchmark::DoNotOptimize(vec.data());
    benchmark::ClobberMemory();
  }
}

static void insert_vector(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);

  std::vector<int> data(count);
  for (auto &v : data) {
    v = gen(re);
  }

  for (auto _ : state) {
    std::vector<int> vec;
    vec.reserve(data.size());
    for (auto val : data) {
      auto it = vec.begin();
      std::advance(it, vec.size() / 2);
      vec.insert(it, val);
    }
    benchmark::DoNotOptimize(vec.data());
    benchmark::ClobberMemory();
  }
}

static void append_list(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);

  std::vector<int> data(count);
  for (auto &v : data) {
    v = gen(re);
  }

  for (auto _ : state) {
    std::list<int> l;
    for (auto val : data) {
      l.push_back(val);
    }
    benchmark::DoNotOptimize(&l.front());
    benchmark::ClobberMemory();
  }
}

static void prepend_list(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);

  std::vector<int> data(count);
  for (auto &v : data) {
    v = gen(re);
  }

  for (auto _ : state) {
    std::list<int> l;
    for (auto val : data) {
      l.push_front(val);
    }
    benchmark::DoNotOptimize(&l.front());
    benchmark::ClobberMemory();
  }
}

static void insert_list(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);

  std::vector<int> data(count);
  for (auto &v : data) {
    v = gen(re);
  }

  for (auto _ : state) {
    std::list<int> l;
    for (auto val : data) {
      auto it = l.begin();
      std::advance(it, l.size() / 2);
      l.insert(it, val);
    }
    benchmark::DoNotOptimize(&l.front());
    benchmark::ClobberMemory();
  }
}

static void append_set(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);

  std::vector<int> data(count);
  for (auto &v : data) {
    v = gen(re);
  }

  for (auto _ : state) {
    std::set<int> s;
    for (auto val : data) {
      s.insert(val);
    }
    benchmark::DoNotOptimize(&(*s.begin()));
    benchmark::ClobberMemory();
  }
}

static void insert_set(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);

  std::vector<int> data(count);
  for (auto &v : data) {
    v = gen(re);
  }

  for (auto _ : state) {
    std::set<int> s;
    for (auto val : data) {
      auto it = s.begin();
      std::advance(it, s.size() / 2);
      s.insert(it, val);
    }
    benchmark::DoNotOptimize(&(*s.begin()));
    benchmark::ClobberMemory();
  }
}

static void append_unordered_set(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);

  std::vector<int> data(count);
  for (auto &v : data) {
    v = gen(re);
  }

  for (auto _ : state) {
    std::unordered_set<int> s;
    for (auto val : data) {
      s.insert(val);
    }
    benchmark::DoNotOptimize(&(*s.begin()));
    benchmark::ClobberMemory();
  }
}

static void insert_unordered_set(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);

  std::vector<int> data(count);
  for (auto &v : data) {
    v = gen(re);
  }

  for (auto _ : state) {
    std::unordered_set<int> s;
    for (auto val : data) {
      auto it = s.begin();
      std::advance(it, s.size() / 2);
      s.insert(it, val);
    }
    benchmark::DoNotOptimize(&(*s.begin()));
    benchmark::ClobberMemory();
  }
}

static void append_map(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);

  std::vector<int> data(count);
  for (auto &v : data) {
    v = gen(re);
  }

  for (auto _ : state) {
    std::map<int, int> m;
    for (auto val : data) {
      m[val] = val;
    }
    benchmark::DoNotOptimize(&(*m.begin()));
    benchmark::ClobberMemory();
  }
}

static void insert_map(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);

  std::vector<int> data(count);
  for (auto &v : data) {
    v = gen(re);
  }

  for (auto _ : state) {
    std::map<int, int> m;
    for (auto val : data) {
      auto it = m.begin();
      std::advance(it, m.size() / 2);
      m.insert(it, {val, val});
    }
    benchmark::DoNotOptimize(&(*m.begin()));
    benchmark::ClobberMemory();
  }
}

static void append_unordered_map(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);

  std::vector<int> data(count);
  for (auto &v : data) {
    v = gen(re);
  }

  for (auto _ : state) {
    std::unordered_map<int, int> m;
    for (auto val : data) {
      m[val] = val;
    }
    benchmark::DoNotOptimize(&(*m.begin()));
    benchmark::ClobberMemory();
  }
}

static void insert_unordered_map(benchmark::State &state) {

  const int count = state.range(0);
  std::random_device r;
  std::default_random_engine re(r());
  std::uniform_int_distribution<int> gen(INT32_MIN, INT32_MAX);

  std::vector<int> data(count);
  for (auto &v : data) {
    v = gen(re);
  }

  for (auto _ : state) {
    std::unordered_map<int, int> m;
    for (auto val : data) {
      auto it = m.begin();
      std::advance(it, m.size() / 2);
      m.insert(it, {val, val});
    }
    benchmark::DoNotOptimize(&(*m.begin()));
    benchmark::ClobberMemory();
  }
}

BENCHMARK(append_vector_no_reserve)->Arg(100);
BENCHMARK(append_vector_reserve)->Arg(100);
BENCHMARK(insert_vector)->Arg(100);
BENCHMARK(append_list)->Arg(100);
BENCHMARK(prepend_list)->Arg(100);
BENCHMARK(insert_list)->Arg(100);
BENCHMARK(append_set)->Arg(100);
BENCHMARK(insert_set)->Arg(100);
BENCHMARK(append_unordered_set)->Arg(100);
BENCHMARK(insert_unordered_set)->Arg(100);
BENCHMARK(append_map)->Arg(100);
BENCHMARK(insert_map)->Arg(100);
BENCHMARK(append_unordered_map)->Arg(100);
BENCHMARK(insert_unordered_map)->Arg(100);

BENCHMARK(append_vector_no_reserve)->Arg(1000);
BENCHMARK(append_vector_reserve)->Arg(1000);
BENCHMARK(insert_vector)->Arg(1000);
BENCHMARK(append_list)->Arg(1000);
BENCHMARK(prepend_list)->Arg(1000);
BENCHMARK(insert_list)->Arg(1000);
BENCHMARK(append_set)->Arg(1000);
BENCHMARK(insert_set)->Arg(1000);
BENCHMARK(append_unordered_set)->Arg(1000);
BENCHMARK(insert_unordered_set)->Arg(1000);
BENCHMARK(append_map)->Arg(1000);
BENCHMARK(insert_map)->Arg(1000);
BENCHMARK(append_unordered_map)->Arg(1000);
BENCHMARK(insert_unordered_map)->Arg(1000);

BENCHMARK(append_vector_no_reserve)->Arg(10000);
BENCHMARK(append_vector_reserve)->Arg(10000);
BENCHMARK(insert_vector)->Arg(10000);
BENCHMARK(append_list)->Arg(10000);
BENCHMARK(prepend_list)->Arg(10000);
BENCHMARK(insert_list)->Arg(10000);
BENCHMARK(append_set)->Arg(10000);
BENCHMARK(insert_set)->Arg(10000);
BENCHMARK(append_unordered_set)->Arg(10000);
BENCHMARK(insert_unordered_set)->Arg(10000);
BENCHMARK(append_map)->Arg(10000);
BENCHMARK(insert_map)->Arg(10000);
BENCHMARK(append_unordered_map)->Arg(10000);
BENCHMARK(insert_unordered_map)->Arg(10000);

BENCHMARK_MAIN();