#include <benchmark/benchmark.h>

#include <bitset>
#include <cmath>
#include <cstdio>
#include <random>
#include <unordered_map>
#include <vector>

#include "test_data/items.h"
#include "test_data/order_items.h"
#include "test_data/orders.h"

#include "Baseline.h"

class TestFix : public benchmark::Fixture {

public:
  uint32_t orderId(::benchmark::State &state) {
    state.PauseTiming();
    auto idx = order_gen(re);
    auto id = orders[idx].id;
    state.ResumeTiming();
    return id;
  }

  std::string_view getUserName(::benchmark::State &state) {
    state.PauseTiming();
    auto idx = order_gen(re);
    auto name = orders[idx].user_name;
    state.ResumeTiming();
    return name;
  }

private:
  std::random_device r;
  std::default_random_engine re{r()};
  std::uniform_int_distribution<uint32_t> item_gen{0, item_count - 1};
  std::uniform_int_distribution<uint32_t> order_gen{0, order_count - 1};
};

/*
  Problem description

  You need to improve the performance of an order management system.
  You can write a completely new implementation or copy 'Baseline.h' and
  'Baseline.cpp', rename them to your user name, and then refactor and improve.

  What you can do:
  * Change API
  * Change internal representation
  * Change implementation
  * Change input/output

  What you cannot do:
  * Change 'common_types.h'
  * Change data in 'test_data' folder
  * Change 'TestFix'
  * Skip or ignore data provided by the 'common_types.h'

  Your implementation must provide implementation and benchmarks for the
  following:
  * Processing of items, orders and order_items data
  * A way to get min bounding volume for order ids
  * A way to get order ids from user names
  * A way to get unique item ids from user names

  A simple implementation of the min bounding volume can be found
  'baseline:OrderManager::getVolume'.
*/

BENCHMARK_F(TestFix, Baseline_data_parsing)(benchmark::State &state) {

  for (auto _ : state) {
    baseline::OrderManager manager(
        {items, &items[item_count]}, {orders, &orders[order_count]},
        {order_items, &order_items[order_item_count]});

    benchmark::DoNotOptimize(manager.items().data());
    benchmark::DoNotOptimize(manager.orders().data());
  }
};

BENCHMARK_F(TestFix, Baseline_get_order_min_volume)
(benchmark::State &state) {
  baseline::OrderManager manager({items, &items[item_count]},
                                 {orders, &orders[order_count]},
                                 {order_items, &order_items[order_item_count]});
  benchmark::DoNotOptimize(manager.items().data());
  benchmark::DoNotOptimize(manager.orders().data());
  for (auto _ : state) {

    auto dimentions = manager.getVolume(
        {orderId(state), orderId(state), orderId(state), orderId(state),
         orderId(state), orderId(state), orderId(state), orderId(state),
         orderId(state), orderId(state)});
    benchmark::DoNotOptimize(dimentions);
  }
};

BENCHMARK_F(TestFix, Baseline_get_user_orders)
(benchmark::State &state) {
  baseline::OrderManager manager({items, &items[item_count]},
                                 {orders, &orders[order_count]},
                                 {order_items, &order_items[order_item_count]});
  benchmark::DoNotOptimize(manager.items().data());
  benchmark::DoNotOptimize(manager.orders().data());
  for (auto _ : state) {

    auto orders = manager.getUserOrdersIds(
        {getUserName(state), getUserName(state), getUserName(state),
         getUserName(state), getUserName(state), getUserName(state),
         getUserName(state), getUserName(state), getUserName(state),
         getUserName(state)});
    benchmark::DoNotOptimize(orders);
  }
};

BENCHMARK_F(TestFix, Baseline_get_user_items)
(benchmark::State &state) {
  baseline::OrderManager manager({items, &items[item_count]},
                                 {orders, &orders[order_count]},
                                 {order_items, &order_items[order_item_count]});
  benchmark::DoNotOptimize(manager.items().data());
  benchmark::DoNotOptimize(manager.orders().data());
  for (auto _ : state) {

    auto orders = manager.getUserItemIds(
        {getUserName(state), getUserName(state), getUserName(state),
         getUserName(state), getUserName(state), getUserName(state),
         getUserName(state), getUserName(state), getUserName(state),
         getUserName(state)});
    benchmark::DoNotOptimize(orders);
  }
};

BENCHMARK_MAIN();