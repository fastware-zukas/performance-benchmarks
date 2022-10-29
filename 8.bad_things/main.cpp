#include <benchmark/benchmark.h>

#include "1.more_is_better.h"
#include "10.class_exmp.h"
#include "3.dependency.h"
#include "3.free_functions.h"
#include "3.pointers.h"
#include "4.non_poly.h"
#include "4.poly_a_class.h"
#include "4.poly_b_class.h"
#include "4.poly_interface.h"
#include "5.multi_inheritance.h"
#include "5.print_scan.h"
#include "7.pimpl_exmp.h"

#include <array>
#include <charconv>
#include <cstring>
#include <memory>
#include <random>
#include <string>
#include <string_view>
#include <vector>

static void class_member_setters_10(benchmark::State &state) {

  const int data_set_size = state.range(0);

  std::vector<std::pair<int, std::string>> test_data(data_set_size);
  for (int i = 0; i < data_set_size; i++) {
    test_data[i].first = i;
    test_data[i].second = std::to_string(i);
  }

  std::vector<MyClass> test(data_set_size);

  for (auto _ : state) {
    for (const auto &[id, name] : test_data) {
      test[id].setId(id);
      test[id].setName(name);
    }
    benchmark::DoNotOptimize(test.data());

    for (const auto &my_class : test) {
      auto id = my_class.getId();
      const auto &name = my_class.getName();
      benchmark::DoNotOptimize(id);
      benchmark::DoNotOptimize(name);
    }
  }
}

static void class_member_direct_access_10_9(benchmark::State &state) {

  const int data_set_size = state.range(0);

  std::vector<std::pair<int, std::string>> test_data(data_set_size);
  for (int i = 0; i < data_set_size; i++) {
    test_data[i].first = i;
    test_data[i].second = std::to_string(i);
  }

  std::vector<MyClass> test(data_set_size);

  for (auto _ : state) {
    for (const auto &[id, name] : test_data) {
      test[id].m_id = id;
      test[id].m_name = name;
    }
    benchmark::DoNotOptimize(test.data());

    for (const auto &my_struct : test) {
      auto id = my_struct.m_id;
      const auto &name = my_struct.m_name;
      benchmark::DoNotOptimize(id);
      benchmark::DoNotOptimize(name);
    }
  }
}

struct MyStruct {
  int id;
  std::string name;
};

static void struct_member_direct_access_9(benchmark::State &state) {

  const int data_set_size = state.range(0);

  std::vector<std::pair<int, std::string>> test_data(data_set_size);
  for (int i = 0; i < data_set_size; i++) {
    test_data[i].first = i;
    test_data[i].second = std::to_string(i);
  }

  std::vector<MyStruct> test(data_set_size);

  for (auto _ : state) {
    for (const auto &[id, name] : test_data) {
      test[id].id = id;
      test[id].name = name;
    }
    benchmark::DoNotOptimize(test.data());

    for (const auto &[id, name] : test) {
      benchmark::DoNotOptimize(id);
      benchmark::DoNotOptimize(name);
    }
  }
}

bool validate(int id, std::string_view name) {
  int value = 0;
  std::from_chars(name.begin(), name.end(), value);
  return value == id;
}

static void struct_data_only_8(benchmark::State &state) {

  const int data_set_size = state.range(0);

  std::vector<std::pair<int, std::string>> test_data(data_set_size);
  for (int i = 0; i < data_set_size; i++) {
    test_data[i].first = i;
    test_data[i].second = std::to_string(i);
  }

  std::vector<MyStruct> test(data_set_size);

  for (auto _ : state) {
    for (const auto &[id, name] : test_data) {
      const bool valid = validate(id, name);
      benchmark::DoNotOptimize(valid);
      test[id].id = id;
      test[id].name = name;
    }
    benchmark::DoNotOptimize(test.data());

    for (const auto &[id, name] : test) {
      benchmark::DoNotOptimize(id);
      benchmark::DoNotOptimize(name);
    }
  }
}

static void class_behaviour_8_7(benchmark::State &state) {

  const int data_set_size = state.range(0);

  std::vector<std::pair<int, std::string>> test_data(data_set_size);
  for (int i = 0; i < data_set_size; i++) {
    test_data[i].first = i;
    test_data[i].second = std::to_string(i);
  }

  std::vector<MyClass> test(data_set_size);

  for (auto _ : state) {
    for (const auto &[id, name] : test_data) {
      test[id].setId(id);
      test[id].setName(name);
      const bool valid = test[id].validate();
      benchmark::DoNotOptimize(valid);
    }
    benchmark::DoNotOptimize(test.data());

    for (const auto &my_class : test) {
      auto id = my_class.getId();
      const auto &name = my_class.getName();
      benchmark::DoNotOptimize(id);
      benchmark::DoNotOptimize(name);
    }
  }
}

static void class_pimpl_7(benchmark::State &state) {

  const int data_set_size = state.range(0);

  std::vector<std::pair<int, std::string>> test_data(data_set_size);
  for (int i = 0; i < data_set_size; i++) {
    test_data[i].first = i;
    test_data[i].second = std::to_string(i);
  }

  std::vector<pimpl_exmp> test(data_set_size);

  for (auto _ : state) {
    for (const auto &[id, name] : test_data) {
      test[id].setId(id);
      test[id].setName(name);
      const bool valid = test[id].validate();
      benchmark::DoNotOptimize(valid);
    }
    benchmark::DoNotOptimize(test.data());

    for (const auto &my_class : test) {
      auto id = my_class.getId();
      const auto &name = my_class.getName();
      benchmark::DoNotOptimize(id);
      benchmark::DoNotOptimize(name);
    }
  }
}

struct AllMyData {
  int id;
  std::string name;
  std::pair<int, int> type;
  std::array<std::string, 128> attribute{};
  std::array<uint8_t, 128> data{};
};

static void mix_data_6(benchmark::State &state) {

  const int data_set_size = state.range(0);

  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_int_distribution<uint8_t> uniform_dist(0, 255);

  std::vector<AllMyData> test_data(data_set_size);
  for (int i = 0; i < data_set_size; ++i) {
    test_data[i].id = i;
    test_data[i].name = std::to_string(i);
    test_data[i].type = std::make_pair(i, data_set_size - i);
    std::generate(test_data[i].attribute.begin(), test_data[i].attribute.end(),
                  [&]() { return std::to_string(uniform_dist(e1)); });
    std::generate(test_data[i].data.begin(), test_data[i].data.end(),
                  [&]() { return uniform_dist(e1); });
  }

  for (auto _ : state) {
    for (auto &allData : test_data) {
      for (auto &val : allData.data) {
        val = (val | 0x01010101) << 1;
      }
    }
  }
}

struct DenseDataSoA {
  std::vector<int> ids;
  std::vector<std::string> names;
  std::vector<std::pair<int, int>> types;
  std::vector<std::array<std::string, 128>> attributes;
  std::vector<std::array<uint8_t, 128>> typeData;
};

static void dense_data_6(benchmark::State &state) {

  const int data_set_size = state.range(0);

  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_int_distribution<uint8_t> uniform_dist(0, 255);

  DenseDataSoA test_data;
  test_data.ids.resize(data_set_size);
  test_data.names.resize(data_set_size);
  test_data.types.resize(data_set_size);
  test_data.attributes.resize(data_set_size);
  test_data.typeData.resize(data_set_size);

  for (int i = 0; i < data_set_size; ++i) {
    test_data.ids[i] = i;
    test_data.names[i] = std::to_string(i);
    test_data.types[i] = std::make_pair(i, data_set_size - 1);
    std::generate(test_data.attributes[i].begin(),
                  test_data.attributes[i].end(),
                  [&]() { return std::to_string(uniform_dist(e1)); });
    std::generate(test_data.typeData[i].begin(), test_data.typeData[i].end(),
                  [&]() { return uniform_dist(e1); });
  }

  for (auto _ : state) {
    for (auto &typeData : test_data.typeData) {
      for (auto &val : typeData) {
        val = (val | 0x01010101) << 1;
      }
    }
  }
}

static void multiple_inheritances_5(benchmark::State &state) {

  const int width = state.range(0);
  const int height = state.range(1);

  std::random_device r;
  std::default_random_engine e1{r()};
  std::uniform_int_distribution<uint16_t> uniform_dist{
      std::numeric_limits<uint16_t>::min(),
      std::numeric_limits<uint16_t>::max()};

  std::vector<Pixel> res;
  res.reserve(width * height);

  for (int w = 0; w < width; ++w) {
    for (int h = 0; h < height; ++h) {
      auto r = uniform_dist(e1);
      auto g = uniform_dist(e1);
      auto b = uniform_dist(e1);
      res.push_back({r, g, b});
    }
  }

  AllInOne object(std::move(res));

  for (auto _ : state) {
    object.copy();
  }
}

static void no_inheritances_5(benchmark::State &state) {

  const int width = state.range(0);
  const int height = state.range(1);

  std::random_device r;
  std::default_random_engine e1{r()};
  std::uniform_int_distribution<uint16_t> uniform_dist{
      std::numeric_limits<uint16_t>::min(),
      std::numeric_limits<uint16_t>::max()};

  std::vector<pixel_t> res;
  res.reserve(width * height);

  for (int w = 0; w < width; ++w) {
    for (int h = 0; h < height; ++h) {
      auto r = uniform_dist(e1);
      auto g = uniform_dist(e1);
      auto b = uniform_dist(e1);
      res.push_back({r, g, b});
    }
  }

  all_in_one_t object;
  object.frontBuffer = std::move(res);

  for (auto _ : state) {
    copy(&object);
  }
}

static void polymorphism_4(benchmark::State &state) {

  const int count = state.range(0);

  std::random_device r;
  std::default_random_engine e1{r()};
  std::uniform_int_distribution<int> obj_dist{0, count};
  std::uniform_int_distribution<int> uniform_dist{-1000, 1000};

  std::vector<std::unique_ptr<poly_interface>> objects(count);
  for (auto &obj : objects) {
    const bool type = obj_dist(e1) < count / 2;
    if (type) {
      obj = std::make_unique<poly_a_class>(3, 2, 5);
    } else {
      obj = std::make_unique<poly_b_class>(2, 4);
    }
  }

  std::vector<int> proc_data(count);
  for (auto &val : proc_data) {
    val = uniform_dist(e1);
  }

  for (auto _ : state) {
    for (int i = 0; i < count; ++i) {
      const auto res = objects[i]->compute(proc_data[i]);
      benchmark::DoNotOptimize(res);
    }
  }
}

static void non_polymorphism_4(benchmark::State &state) {

  const int count = state.range(0);

  std::random_device r;
  std::default_random_engine e1{r()};
  std::uniform_int_distribution<int> obj_dist{0, count};
  std::uniform_int_distribution<int> uniform_dist{-1000, 1000};

  std::vector<type_a> type_a_objects;
  type_a_objects.reserve(count);
  std::vector<type_b> type_b_objects;
  type_b_objects.reserve(count);

  for (int i = 0; i < count; ++i) {
    const bool type = obj_dist(e1) < count / 2;
    if (type) {
      type_a_objects.push_back({3, 2, 5});
    } else {
      type_b_objects.push_back({2, 4});
    }
  }

  std::vector<int> proc_data_a(type_a_objects.size());
  for (auto &val : proc_data_a) {
    val = uniform_dist(e1);
  }

  std::vector<int> proc_data_b(type_b_objects.size());
  for (auto &val : proc_data_b) {
    val = uniform_dist(e1);
  }

  for (auto _ : state) {
    for (int i = 0, l = proc_data_a.size(); i < l; i++) {
      const auto res = type_a_compute(&type_a_objects[i], proc_data_a[i]);
      benchmark::DoNotOptimize(res);
    }

    for (int i = 0, l = proc_data_b.size(); i < l; i++) {
      const auto res = type_b_compute(&type_b_objects[i], proc_data_b[i]);
      benchmark::DoNotOptimize(res);
    }
  }
}

static void non_polymorphism_union_4(benchmark::State &state) {

  const int count = state.range(0);

  std::random_device r;
  std::default_random_engine e1{r()};
  std::uniform_int_distribution<int> obj_dist{0, count};
  std::uniform_int_distribution<int> uniform_dist{-1000, 1000};

  struct type_union {
    enum { A, B } type;
    union {
      type_a a;
      type_b b;
    };
  };

  std::vector<type_union> type_objects(count);

  for (auto &type_obj : type_objects) {
    const bool type = obj_dist(e1) < count / 2;
    if (type) {
      type_obj.type = type_union::A;
      type_obj.a = {3, 2, 5};
    } else {
      type_obj.type = type_union::B;
      type_obj.b = {2, 4};
    }
  }

  std::vector<int> proc_data(count);
  for (auto &val : proc_data) {
    val = uniform_dist(e1);
  }

  for (auto _ : state) {
    for (int i = 0; i < count; i++) {
      switch (type_objects[i].type) {
      case type_union::A: {
        const auto res = type_a_compute(&type_objects[i].a, proc_data[i]);
        benchmark::DoNotOptimize(res);
        break;
      }
      case type_union::B: {
        const auto res = type_b_compute(&type_objects[i].b, proc_data[i]);
        benchmark::DoNotOptimize(res);
        break;
      }
      }
    }
  }
}

static void shared_ptr_3(benchmark::State &state) {

  const int count = state.range(0);

  std::shared_ptr<DependClass> dep = std::make_shared<DependClass>(3, 2);

  std::vector<SomeClass> objects;
  objects.reserve(count);

  for (int i = 0; i < count; ++i) {
    objects.push_back(SomeClass(dep));
  }

  for (auto _ : state) {
    for (auto &obj : objects) {
      const auto res = obj.calculateAnswer();
      benchmark::DoNotOptimize(res);
    }
  }
}

static void free_fn_3_2(benchmark::State &state) {

  const int count = state.range(0);

  deps_t deps{3, 2};

  for (auto _ : state) {
    for (int i = 0; i < count; ++i) {
      const auto res = calculateAnswer(deps);
      benchmark::DoNotOptimize(res);
    }
  }
}

static void more_smart_ptr_2(benchmark::State &state) {

  const int count = state.range(0);

  std::shared_ptr<DependClass> dep = std::make_shared<DependClass>(3, 2);

  std::vector<SomeClass> objects;
  objects.reserve(count);

  for (int i = 0; i < count; ++i) {
    objects.push_back(SomeClass(dep));
  }

  for (auto _ : state) {
    for (auto &obj : objects) {
      const auto res = obj.calculateResult();
      const auto val = res->getResult();
      benchmark::DoNotOptimize(val);
    }
  }
}

static void one_at_the_time_01(benchmark::State &state) {

  const int count = state.range(0);

  std::random_device r;
  std::default_random_engine e1{r()};
  std::uniform_int_distribution<int> uniform_dist{-1000, 1000};

  OneItemAtTheTime object;
  std::vector<std::pair<int, int>> inputs(count);
  std::generate(inputs.begin(), inputs.end(), [&]() {
    return std::make_pair(uniform_dist(e1), uniform_dist(e1));
  });

  for (auto _ : state) {
    for (auto [a, b] : inputs) {
      const auto res = object.oneAtTheTime(a, b);
      benchmark::DoNotOptimize(res);
    }
  }
}

static void many_at_the_time_01(benchmark::State &state) {

  const int count = state.range(0);

  std::random_device r;
  std::default_random_engine e1{r()};
  std::uniform_int_distribution<int> uniform_dist{-1000, 1000};

  std::vector<int> as(count);
  std::vector<int> bs(count);
  std::vector<int> res(count);

  std::generate(as.begin(), as.end(), [&]() { return uniform_dist(e1); });
  std::generate(bs.begin(), bs.end(), [&]() { return uniform_dist(e1); });

  for (auto _ : state) {
    manyAtTheTime(as, bs, res);
    benchmark::DoNotOptimize(res.data());
  }
}

BENCHMARK(class_member_setters_10)->Arg(100);
BENCHMARK(class_member_direct_access_10_9)->Arg(100);
BENCHMARK(struct_member_direct_access_9)->Arg(100);
BENCHMARK(struct_data_only_8)->Arg(100);
BENCHMARK(class_behaviour_8_7)->Arg(100);
BENCHMARK(class_pimpl_7)->Arg(100);
BENCHMARK(mix_data_6)->Arg(100);
BENCHMARK(dense_data_6)->Arg(100);
BENCHMARK(multiple_inheritances_5)->Args({1748, 1240}); // A6
BENCHMARK(no_inheritances_5)->Args({1748, 1240});       // A6
BENCHMARK(polymorphism_4)->Arg(100);
BENCHMARK(non_polymorphism_4)->Arg(100);
BENCHMARK(non_polymorphism_union_4)->Arg(100);
BENCHMARK(shared_ptr_3)->Arg(100);
BENCHMARK(free_fn_3_2)->Arg(100);
BENCHMARK(more_smart_ptr_2)->Arg(100);
BENCHMARK(one_at_the_time_01)->Arg(100);
BENCHMARK(many_at_the_time_01)->Arg(100);

BENCHMARK(class_member_setters_10)->Arg(1000);
BENCHMARK(class_member_direct_access_10_9)->Arg(1000);
BENCHMARK(struct_member_direct_access_9)->Arg(1000);
BENCHMARK(struct_data_only_8)->Arg(1000);
BENCHMARK(class_behaviour_8_7)->Arg(1000);
BENCHMARK(class_pimpl_7)->Arg(1000);
BENCHMARK(mix_data_6)->Arg(1000);
BENCHMARK(dense_data_6)->Arg(1000);
BENCHMARK(multiple_inheritances_5)->Args({3508, 2480}); // A4
BENCHMARK(no_inheritances_5)->Args({3508, 2480});       // A4
BENCHMARK(polymorphism_4)->Arg(1000);
BENCHMARK(non_polymorphism_4)->Arg(1000);
BENCHMARK(non_polymorphism_union_4)->Arg(1000);
BENCHMARK(shared_ptr_3)->Arg(1000);
BENCHMARK(free_fn_3_2)->Arg(1000);
BENCHMARK(more_smart_ptr_2)->Arg(1000);
BENCHMARK(one_at_the_time_01)->Arg(1000);
BENCHMARK(many_at_the_time_01)->Arg(1000);

BENCHMARK(class_member_setters_10)->Arg(10000);
BENCHMARK(class_member_direct_access_10_9)->Arg(10000);
BENCHMARK(struct_member_direct_access_9)->Arg(10000);
BENCHMARK(struct_data_only_8)->Arg(10000);
BENCHMARK(class_behaviour_8_7)->Arg(10000);
BENCHMARK(class_pimpl_7)->Arg(10000);
BENCHMARK(mix_data_6)->Arg(10000);
BENCHMARK(dense_data_6)->Arg(10000);
BENCHMARK(multiple_inheritances_5)->Args({7016, 4961}); // A2
BENCHMARK(no_inheritances_5)->Args({7016, 4961});       // A2
BENCHMARK(polymorphism_4)->Arg(10000);
BENCHMARK(non_polymorphism_4)->Arg(10000);
BENCHMARK(non_polymorphism_union_4)->Arg(10000);
BENCHMARK(shared_ptr_3)->Arg(10000);
BENCHMARK(free_fn_3_2)->Arg(10000);
BENCHMARK(more_smart_ptr_2)->Arg(10000);
BENCHMARK(one_at_the_time_01)->Arg(10000);
BENCHMARK(many_at_the_time_01)->Arg(10000);

BENCHMARK_MAIN();