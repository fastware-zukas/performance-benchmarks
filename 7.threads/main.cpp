#include <benchmark/benchmark.h>

#include <random>
#include <string_view>
#include <vector>

#include <atomic>
#include <execution>
#include <map>
#include <mutex>
#include <set>
#include <utility>

class SaleFixture : public benchmark::Fixture {
  static constexpr const char *country_codes[]{
      "AF", "AL", "DZ", "AS", "AD", "AO", "AI", "AQ", "AG", "AR", "AM", "AW",
      "AU", "AT", "AZ", "BS", "BH", "BD", "BB", "BY", "BE", "BZ", "BJ", "BM",
      "BT", "BO", "BQ", "BA", "BW", "BV", "BR", "IO", "BN", "BG", "BF", "BI",
      "CV", "KH", "CM", "CA", "KY", "CF", "TD", "CL", "CN", "CX", "CC", "CO",
      "KM", "CD", "CG", "CK", "CR", "HR", "CU", "CW", "CY", "CZ", "CI", "DK",
      "DJ", "DM", "DO", "EC", "EG", "SV", "GQ", "ER", "EE", "SZ", "ET", "FK",
      "FO", "FJ", "FI", "FR", "GF", "PF", "GD", "GP", "GU", "GT", "GG", "GN",
      "GW", "GY", "HT", "HM", "VA", "HN", "HK", "HU", "IS", "IN", "ID", "IR",
      "IQ", "IE", "IM", "IL", "IT", "JM", "JP", "JE", "JO", "KZ", "KE", "KI",
      "KP", "KR", "KW", "KG", "LA", "LV", "LB", "LS", "LR", "LY", "LI", "LT",
      "LU", "MO", "MG", "MW", "MY", "MV", "ML", "MT", "MH", "MQ", "MR", "MU",
      "YT", "MX", "FM", "MD", "MC", "MN", "ME", "MS", "MA", "MZ", "MM", "NA",
      "NR", "NP", "NL", "NC", "NZ", "NI", "NE", "NG", "NU", "NF", "MP", "NO",
      "OM", "PK", "PW", "PS", "PA", "PG", "PY", "PE", "PH", "PN", "PL", "PT",
      "PR", "QA", "MK", "RO", "RU", "RW", "RE", "BL", "SH", "KN", "LC", "MF",
      "PM", "VC", "WS", "SM", "ST", "SA", "SN", "RS", "SC", "SL", "SG", "SX",
      "SK", "SI", "SB", "SO", "ZA", "GS", "SS", "ES", "LK", "SD", "SR", "SJ",
      "SE", "CH", "SY", "TW", "TJ", "TZ", "TH", "TL", "TG", "TK", "TO", "TT",
      "TN", "TR", "TM", "TC", "TV", "UG", "UA", "AE", "GB", "UM", "US", "UY",
      "UZ", "VU", "VE", "VN", "VG", "VI", "WF", "EH", "YE", "ZM", "ZW", "AX",
      "VN", "VG", "VI", "WF", "EH", "YE", "ZM", "ZW", "AX"};

public:
  void SetUp(const ::benchmark::State &state) {}

  void TearDown(const ::benchmark::State &state) {}

  std::string_view getCountryCode() {
    return {country_codes[d_country_code(re)]};
  }

  int getBranchCode() { return d_branch_number(re); }

  int count() { return d_count(re); }

  int getTopN() { return d_top(re); }

private:
  std::random_device r;
  std::default_random_engine re{r()};

  std::uniform_int_distribution<int> d_country_code{
      0, sizeof(country_codes) / sizeof(const char *) - 1};

  std::uniform_int_distribution<int> d_branch_number{0, 99};
  std::uniform_int_distribution<int> d_count{1, 50};
  std::uniform_int_distribution<int> d_top{1, 1000};
};

/*
    PROBLEM DESCRIPTION

    Design a software system that receives the sale of each vehicle from country
    in the world. Each country may have up to a hundred branches. The system
    should provide an API to query top N branches which sold the most cars. The
    result set must be in descending order.

    The system must be as efficient as possible due to energy scarcity.
    Provide algorithmic complexity analysis of all your algorithms.

    Example skeleton API provided below.
*/

// Naive implementation
namespace naive {

static struct naive_data {
  std::mutex mutex;
  std::map<std::pair<std::string, int>, int> amountMap;

  class SetCompare {
  public:
    bool operator()(const std::pair<std::string, int> &lhs,
                    const std::pair<std::string, int> &rhs) const;
  };

  std::set<std::pair<std::string, int>, SetCompare> ccbnSet;
} instance{};

bool naive_data::SetCompare::operator()(
    const std::pair<std::string, int> &lhs,
    const std::pair<std::string, int> &rhs) const {
  bool greater_amount = instance.amountMap[lhs] > instance.amountMap[rhs];
  bool equal_amount = instance.amountMap[lhs] == instance.amountMap[rhs];
  bool less_key = lhs < rhs;
  return equal_amount ? less_key : greater_amount;
}

static void record_vehicle_sale(std::string_view countryCode, int branchNumber,
                                int count) {
  std::lock_guard lock(instance.mutex);
  instance.amountMap[std::pair<std::string, int>(countryCode, branchNumber)] +=
      count;
  instance.ccbnSet.erase(
      std::pair<std::string, int>(countryCode, branchNumber));
  instance.ccbnSet.insert(
      std::pair<std::string, int>(countryCode, branchNumber));
}

struct Result {
  std::string countryCode;
  int branchNumber;
  int carsSold;
};

static std::vector<Result> get_top_branches(int top) {
  std::vector<Result> result;
  std::lock_guard lock(instance.mutex);
  int size = std::min(static_cast<size_t>(top), instance.ccbnSet.size());

  auto it = instance.ccbnSet.begin();
  for (int i = 0; i < size; ++i) {
    const auto &ccbn = *it;
    result.push_back(Result{ccbn.first, ccbn.second, instance.amountMap[ccbn]});
    it++;
  }

  return result;
}
} // namespace naive

BENCHMARK_DEFINE_F(SaleFixture, naive_impl)(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto a = getCountryCode();
    auto b = getBranchCode();
    auto c = count();
    auto d = getTopN();
    state.ResumeTiming();
    naive::record_vehicle_sale(a, b, c);
    auto res = naive::get_top_branches(d);
    benchmark::DoNotOptimize(res);
  }
}

BENCHMARK_REGISTER_F(SaleFixture, naive_impl)
    ->Threads(1)
    ->Threads(2)
    ->Threads(4)
    ->Threads(8)
    ->Threads(16)
    ->UseRealTime();

// A developer with 3 years of exp
namespace dev3years {

class SetItem {
public:
  std::string countryCode; // char[2] - 17 bytes +- (2bytes required)
  int branchNumber;        // 4 bytes (0-99) 7 bits < 2 bytes 0-127
  int volume;              // unsigend int 32bits

  friend bool operator>(const SetItem &lhs, const SetItem &rhs) {

    bool greater_volume = lhs.volume > rhs.volume;
    bool equal_volume = lhs.volume == rhs.volume;
    bool equal_cc = lhs.countryCode == rhs.countryCode;
    bool less_cc = lhs.countryCode < rhs.countryCode;
    bool less_bn = lhs.branchNumber < rhs.branchNumber;

    return equal_volume ? (equal_cc ? less_bn : less_cc) : greater_volume;
  }
};

static struct dev3years_data {
  std::mutex mutex;
  std::map<std::pair<std::string, int>, int> amountMap;
  std::set<SetItem, std::greater<>> ccbnSet;
} instance{};

static void record_vehicle_sale(std::string_view countryCode, int branchNumber,
                                int count) {

  auto ccbn = std::pair<std::string, int>(countryCode, branchNumber);

  std::lock_guard lock(instance.mutex);
  auto &current = instance.amountMap[ccbn];

  SetItem item{ccbn.first, ccbn.second, current};

  instance.ccbnSet.erase(item);

  current += count;
  item.volume = current;

  instance.ccbnSet.insert(item);
}

struct Result {
  std::string countryCode;
  int branchNumber;
  int carsSold;
};

static std::vector<Result> get_top_branches(int top) {
  std::vector<Result> result;

  std::lock_guard lock(instance.mutex);
  int size = std::min(static_cast<size_t>(top), instance.ccbnSet.size());

  auto it = instance.ccbnSet.begin();
  for (int i = 0; i < size; ++i) {
    const auto &ccbn = *it;
    result.push_back(Result{ccbn.countryCode, ccbn.branchNumber, ccbn.volume});
    it++;
  }

  return result;
}
} // namespace dev3years

BENCHMARK_DEFINE_F(SaleFixture, dev3years_impl)(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto a = getCountryCode();
    auto b = getBranchCode();
    auto c = count();
    auto d = getTopN();
    state.ResumeTiming();
    dev3years::record_vehicle_sale(a, b, c);
    auto res = dev3years::get_top_branches(d);
    benchmark::DoNotOptimize(res);
  }
}

BENCHMARK_REGISTER_F(SaleFixture, dev3years_impl)
    ->Threads(1)
    ->Threads(2)
    ->Threads(4)
    ->Threads(8)
    ->Threads(16)
    ->UseRealTime();

// Professional software engineer with an in-depth understanding of hardware,
// data and memory layouts
namespace eng {

union hash_value {
  struct {
    uint32_t cc1 : 5;       // 5 bits 0-31 (A-Z) max 27 vals
    uint32_t cc2 : 5;       // 5 bits 0-31 (A-Z) max 27 vals
    uint32_t bn : 7;        // 7bits 0-127 (0-99) max 99 vals
    uint32_t _padding : 15; // 15 high bits set to 0
  };                        // total 32 bits

  uint32_t value; // total 17 bits - max value 131071
};

static hash_value getHash(std::string_view countryCode, int branchNumber) {
  hash_value v;
  v.cc1 = countryCode[0] - 'A';
  v.cc2 = countryCode[1] - 'A';
  v.bn = branchNumber;
  v._padding = 0;
  return v;
}

struct SetItem {
  union {
    struct {
      hash_value hash;
      uint32_t volume;
    };
    uint64_t key;
  };
};

static struct eng_data {
  std::mutex mutex;
  std::vector<uint32_t> volumes;
  std::set<uint64_t, std::greater<>> ccbnSet;
} instance{};

static void init() { instance.volumes.resize(131071); }

static void record_vehicle_sale(std::string_view countryCode, int branchNumber,
                                int count) {

  std::lock_guard lock(instance.mutex);

  hash_value hash = getHash(countryCode, branchNumber);
  auto current_volume = instance.volumes[hash.value];
  uint32_t new_volume = current_volume + count;
  instance.volumes[hash.value] = new_volume;

  SetItem item;
  item.hash = hash;
  item.volume = current_volume;

  auto set_node = instance.ccbnSet.extract(item.key);
  item.volume = new_volume;
  if (!set_node.empty()) {
    set_node.value() = item.key;
    instance.ccbnSet.insert(std::move(set_node));
  } else {
    instance.ccbnSet.insert(item.key);
  }
}

struct Result {
  char countryCode[2];
  int branchNumber;
  int carsSold;
};

static std::vector<Result> get_top_branches(int top) {
  std::vector<Result> result;

  std::lock_guard lock(instance.mutex);
  int size = std::min(static_cast<size_t>(top), instance.ccbnSet.size());

  result.resize(size);

  auto it = instance.ccbnSet.begin();
  for (int i = 0; i < size; ++i) {

    SetItem item;
    item.key = *it++;
    char c1 = char(item.hash.cc1) + 'A';
    char c2 = char(item.hash.cc2) + 'A';

    result[i].countryCode[0] = c1;
    result[i].countryCode[1] = c2;
    result[i].branchNumber = item.hash.bn;
    result[i].carsSold = item.volume;
  }

  return result;
}
} // namespace eng

BENCHMARK_DEFINE_F(SaleFixture, eng_impl)(benchmark::State &state) {
  if (state.thread_index == 0) {
    eng::init();
  }
  for (auto _ : state) {
    state.PauseTiming();
    auto a = getCountryCode();
    auto b = getBranchCode();
    auto c = count();
    auto d = getTopN();
    state.ResumeTiming();
    eng::record_vehicle_sale(a, b, c);
    auto res = eng::get_top_branches(d);
    benchmark::DoNotOptimize(res);
  }
}

BENCHMARK_REGISTER_F(SaleFixture, eng_impl)
    ->Threads(1)
    ->Threads(2)
    ->Threads(4)
    ->Threads(8)
    ->Threads(16)
    ->UseRealTime();

BENCHMARK_MAIN();