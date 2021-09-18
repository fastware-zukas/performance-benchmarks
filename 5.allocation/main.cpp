

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <random>
#include <string_view>
#include <utility>
#include <vector>

#include "alloc.h"

#include <benchmark/benchmark.h>

class BenchObject {

public:
  BenchObject(int id, std::string name, float width, float length, float height)
      : m_id(id), m_name(std::move(name)), m_width(width), m_length(length),
        m_height(height) {}

  int id() const { return m_id; }
  std::string_view name() const { return {m_name.c_str(), m_name.length()}; }
  float width() const { return m_width; }
  float length() const { return m_length; }
  float height() const { return m_height; }

private:
  int m_id;
  std::string m_name;
  float m_width;
  float m_length;
  float m_height;
};

class CompositeBenchObject {
public:
  CompositeBenchObject(std::vector<std::unique_ptr<BenchObject>> objects)
      : m_objects(std::move(objects)) {}

  float volume() const {
    return std::accumulate(m_objects.cbegin(), m_objects.cend(), 0.f,
                           [](auto current, const auto &obj) {
                             return current + (obj->width() * obj->length() *
                                               obj->height());
                           });
  }

private:
  std::vector<std::unique_ptr<BenchObject>> m_objects;
};

static void memory_alloc_stress_test(benchmark::State &state) {
  const int bucket_count = state.range(0);
  const int obj_per_bucket = state.range(1);

  std::random_device r;

  std::default_random_engine re(r());
  std::uniform_real_distribution<float> dimention(1, 20);

  memory_profile::begin();

  for (auto _ : state) {

    memory_profile::clear();

    std::vector<std::shared_ptr<CompositeBenchObject>> buckets;
    buckets.reserve(bucket_count);

    for (int i = 0; i < bucket_count; ++i) {

      std::vector<std::unique_ptr<BenchObject>> objects;
      objects.reserve(obj_per_bucket);

      for (int j = 0; j < obj_per_bucket; ++j) {
        const int id = i * obj_per_bucket + j;
        std::string name = std::to_string(id);
        const float width = dimention(re);
        const float length = dimention(re);
        const float height = dimention(re);
        objects.push_back(std::make_unique<BenchObject>(id, std::move(name),
                                                        width, length, height));
      }

      buckets.push_back(
          std::make_shared<CompositeBenchObject>(std::move(objects)));
    }

    for (const auto &bucket : buckets) {
      const float volume = bucket->volume();
      benchmark::DoNotOptimize(volume);
    }
  }

  memory_profile::end();
}

struct BenchObjectData {
  std::vector<int> ids;
  std::vector<std::string> names;
  std::vector<float> widths;
  std::vector<float> lengths;
  std::vector<float> heights;
  std::vector<std::pair<size_t, size_t>> objectIndexes;
};

static void memory_alloc_stress_test_soa(benchmark::State &state) {
  const int bucket_count = state.range(0);
  const int obj_per_bucket = state.range(1);

  std::random_device r;

  std::default_random_engine re(r());
  std::uniform_real_distribution<float> dimention(1, 20);

  memory_profile::begin();

  const auto acum = [](const std::vector<float> &widths,
                       const std::vector<float> &lengths,
                       const std::vector<float> &heights,
                       const std::pair<size_t, size_t> &indexes) {
    float sum = 0;
    for (size_t i = indexes.first, l = indexes.first + indexes.second; i < l;
         i++) {
      sum += widths[i] * heights[i] * heights[i];
    }
    return sum;
  };

  for (auto _ : state) {

    memory_profile::clear();

    BenchObjectData obj;

    obj.ids.reserve(bucket_count * obj_per_bucket);
    obj.names.reserve(bucket_count * obj_per_bucket);
    obj.widths.reserve(bucket_count * obj_per_bucket);
    obj.lengths.reserve(bucket_count * obj_per_bucket);
    obj.heights.reserve(bucket_count * obj_per_bucket);
    obj.objectIndexes.reserve(bucket_count);

    for (int i = 0; i < bucket_count; ++i) {

      for (int j = 0; j < obj_per_bucket; ++j) {
        const int id = i * obj_per_bucket + j;
        std::string name = std::to_string(id);
        const float width = dimention(re);
        const float length = dimention(re);
        const float height = dimention(re);
        obj.ids.push_back(id);
        obj.names.push_back(std::move(name));
        obj.widths.push_back(width);
        obj.lengths.push_back(length);
        obj.heights.push_back(height);
      }

      obj.objectIndexes.emplace_back(i * bucket_count, obj_per_bucket);
    }

    for (const auto &indexes : obj.objectIndexes) {
      const float volume = acum(obj.widths, obj.lengths, obj.heights, indexes);
      benchmark::DoNotOptimize(volume);
    }
  }

  memory_profile::end();
}

// Register the function as a benchmark
BENCHMARK(memory_alloc_stress_test)
    ->Unit(benchmark::kMicrosecond)
    ->Args({5, 20})
    ->Args({15, 60})
    ->Args({45, 180})
    ->Args({135, 540})
    ->Args({405, 1620})
    ->Args({1215, 4860});

BENCHMARK(memory_alloc_stress_test_soa)
    ->Unit(benchmark::kMicrosecond)
    ->Args({5, 20})
    ->Args({15, 60})
    ->Args({45, 180})
    ->Args({135, 540})
    ->Args({405, 1620})
    ->Args({1215, 4860});

BENCHMARK_MAIN();
