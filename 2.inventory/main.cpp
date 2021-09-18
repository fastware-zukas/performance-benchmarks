#include <algorithm>
#include <cmath>
#include <random>
#include <utility>
#include <vector>

#include "item.h"
#include "soa.h"

#include <benchmark/benchmark.h>

// orders are a pair of item id and item quantity
// calculate a number of containers required to ship all orders
int shippingCountainerCount(const std::vector<Item> &items,
                            const std::vector<std::pair<int, int>> &orders,
                            double containerHeight, double containerWidth,
                            double containerDepth) {

  const double containerVolume =
      containerHeight * containerWidth * containerDepth;

  double currentVolume = 0;

  for (const auto &order : orders) {

    const auto itemIt =
        std::find_if(items.begin(), items.end(), [&](const auto &item) {
          return order.first == item.GetId();
        });
    if (itemIt == items.end())
      return -1;

    auto width = itemIt->GetWidth();
    auto height = itemIt->GetHeight();
    auto depth = itemIt->GetDepth();

    auto volume = width * height * depth;
    auto sumItemVolue = volume * order.second;

    currentVolume += sumItemVolue;
  }
  return std::ceil(currentVolume / containerVolume);
}

int shippingCountainerCount(const soa &items,
                            const std::vector<std::pair<int, int>> &orders,
                            double containerHeight, double containerWidth,
                            double containerDepth) {

  const double containerVolume =
      containerHeight * containerWidth * containerDepth;

  double currentVolume = 0;

  for (const auto &order : orders) {

    const auto itemIt =
        std::find(items.ids.begin(), items.ids.end(), order.first);
    if (itemIt == items.ids.end())
      return -1;

    const auto index = std::distance(items.ids.begin(), itemIt);

    auto dimension = items.dimensions[index];

    auto volume = dimension.width * dimension.height * dimension.depth;
    auto sumItemVolue = volume * order.second;

    currentVolume += sumItemVolue;
  }
  return std::ceil(currentVolume / containerVolume);
}

static void oop_compute_shipping_container_count(benchmark::State &state) {
  const int item_count = state.range(0);
  const int order_count = state.range(1);

  std::random_device r;

  std::default_random_engine re(r());
  std::uniform_real_distribution<double> size_dist(0.2, 6.5);
  std::uniform_int_distribution<int> item_id_dist(1, item_count);
  std::uniform_int_distribution<int> quantity_dist(1, 50);

  std::vector<Item> items;
  items.reserve(item_count);
  for (int i = 1; i < item_count; i++) {
    Item item;
    item.SetId(i);
    item.SetWidth(size_dist(re));
    item.SetHeight(size_dist(re));
    item.SetDepth(size_dist(re));
    items.push_back(item);
  }

  std::vector<std::pair<int, int>> orders;
  orders.reserve(order_count);
  for (int i = 0; i < order_count; i++) {
    orders.emplace_back(item_id_dist(re), quantity_dist(re));
  }

  for (auto _ : state) {
    int count = shippingCountainerCount(items, orders, 10, 10, 30);
    benchmark::DoNotOptimize(count);
  }
}

// Register the function as a benchmark
BENCHMARK(oop_compute_shipping_container_count)
    ->Unit(benchmark::kMicrosecond)
    ->Args({100, 20})
    ->Args({500, 60})
    ->Args({2500, 180})
    ->Args({12500, 540})
    ->Args({62500, 1620})
    ->Args({312500, 4860});

static void soa_compute_shipping_container_count(benchmark::State &state) {

  const int item_count = state.range(0);
  const int order_count = state.range(1);

  std::random_device r;

  std::default_random_engine re(r());
  std::uniform_real_distribution<double> size_dist(0.2, 6.5);
  std::uniform_int_distribution<int> item_id_dist(1, item_count);
  std::uniform_int_distribution<int> quantity_dist(1, 50);

  soa items;
  items.reserve(item_count);
  for (int i = 1; i < item_count; i++) {
    items.ids.push_back(i);
    items.dimensions.push_back(
        dimension{size_dist(re), size_dist(re), size_dist(re)});
  }

  std::vector<std::pair<int, int>> orders;
  orders.reserve(order_count);
  for (int i = 0; i < order_count; i++) {
    orders.emplace_back(item_id_dist(re), quantity_dist(re));
  }

  for (auto _ : state) {
    int count = shippingCountainerCount(items, orders, 10, 10, 30);
    benchmark::DoNotOptimize(count);
  }
}

BENCHMARK(soa_compute_shipping_container_count)
    ->Unit(benchmark::kMicrosecond)
    ->Args({100, 20})
    ->Args({500, 60})
    ->Args({2500, 180})
    ->Args({12500, 540})
    ->Args({62500, 1620})
    ->Args({312500, 4860});

BENCHMARK_MAIN();