#include <benchmark/benchmark.h>

#include <bitset>
#include <cmath>
#include <cstdio>
#include <random>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>

template <typename T>
class MutexQueue
{

public:
  MutexQueue(int32_t size) : d_size(size)
  {
  }

  bool push(T t)
  {
    std::unique_lock<std::mutex> lock(d_mutex);
    if (d_queue.size() == d_size)
    {
      return false;
    }
    d_queue.push(std::move(t));
    return true;
  }

  bool pop(T &res)
  {
    std::unique_lock<std::mutex> lock(d_mutex);
    if (d_queue.size() == 0)
    {
      return false;
    }
    res = std::move(d_queue.front());
    d_queue.pop();
    return true;
  }

private:
  std::mutex d_mutex;
  std::queue<T> d_queue;
  int32_t d_size;
};

template <typename T>
class AtomicRingBuffer
{
public:
  AtomicRingBuffer(int32_t size) : d_readIdx(0), d_writeIdx(0)
  {
    d_queue.resize(size);
  }

  bool push(T t)
  {
    int32_t readIdx = d_readIdx.load(std::memory_order_acquire);
    int32_t writeIdx = d_writeIdx.load(std::memory_order_consume);
    const int32_t size = d_queue.size();

    int32_t newWriteIdx = (writeIdx + 1) == size ? 0 : (writeIdx + 1);
    while (newWriteIdx == readIdx)
    {
      return false;
    }

    while (!d_writeIdx.compare_exchange_weak(writeIdx, newWriteIdx, std::memory_order_acquire, std::memory_order_acq_rel))
    {
      newWriteIdx = (writeIdx + 1) == size ? 0 : (writeIdx + 1);
    }
    d_queue[writeIdx].data = std::move(t);
    return true;
  }

  bool pop(T &res)
  {

    int32_t readIdx = d_readIdx.load(std::memory_order_consume);
    int32_t writeIdx = d_writeIdx.load(std::memory_order_acquire);
    int32_t size = d_queue.size();
    if (readIdx == writeIdx)
    {
      return false;
    }

    int32_t newReadIdx = readIdx + 1 == size ? 0 : readIdx + 1;
    while (!d_readIdx.compare_exchange_weak(readIdx, newReadIdx, std::memory_order_acquire, std::memory_order_acq_rel))
    {
      newReadIdx = readIdx + 1 == size ? 0 : readIdx + 1;
    }
    res = std::move(d_queue[readIdx].data);

    return true;
  }

private:
  struct alignas(64) AlignedT
  {
    T data;
  };
  std::vector<AlignedT> d_queue;
  std::atomic_int32_t d_readIdx;
  std::atomic_int32_t d_writeIdx;
};

struct TestData
{
  char buffer[70];
};

class BasicTest : public benchmark::Fixture
{

public:
  void SetUp(const ::benchmark::State &state)
  {
  }

  void TearDown(const ::benchmark::State &state)
  {
  }

  MutexQueue<TestData> &queue()
  {
    return d_queue;
  }

private:
  std::random_device r;
  std::default_random_engine re{r()};
  MutexQueue<TestData> d_queue{1024};
};

class AdvancedTest : public benchmark::Fixture
{

public:
  void SetUp(const ::benchmark::State &state)
  {
  }

  void TearDown(const ::benchmark::State &state)
  {
  }

  AtomicRingBuffer<TestData> &queue()
  {
    return d_queue;
  }

private:
  std::random_device r;
  std::default_random_engine re{r()};
  AtomicRingBuffer<TestData> d_queue{1024};
};

BENCHMARK_DEFINE_F(BasicTest, push_pop)
(benchmark::State &state)
{
  for (auto _ : state)
  {
    const bool should_push = state.thread_index() % 2 == 0;
    if (should_push)
    {
      const bool res = queue().push(TestData{});
      benchmark::DoNotOptimize(res);
    }
    else
    {
      TestData data;
      const bool res = queue().pop(data);
      benchmark::DoNotOptimize(data);
      benchmark::DoNotOptimize(res);
    }
  }
}

BENCHMARK_DEFINE_F(AdvancedTest, push_pop)
(benchmark::State &state)
{
  for (auto _ : state)
  {
    const bool should_push = state.thread_index() % 2 == 0;
    if (should_push)
    {
      const bool res = queue().push(TestData{});
      benchmark::DoNotOptimize(res);
    }
    else
    {
      TestData data;
      const bool res = queue().pop(data);
      benchmark::DoNotOptimize(data);
      benchmark::DoNotOptimize(res);
    }
  }
};

BENCHMARK_REGISTER_F(BasicTest, push_pop)->ThreadRange(2, 32);
BENCHMARK_REGISTER_F(AdvancedTest, push_pop)->ThreadRange(2, 32);

BENCHMARK_MAIN();