#include <benchmark/benchmark.h>

#include <array>

#include <iostream>
#include <sstream>

#include <cstdio>
#include <cstring>

constexpr std::array<const char *, 7> testCases{
    "Aapple Aorange Dapple Astrawberry",
    "Aapple Aorange Dapple Agrape Astrawberry",
    "Abba Aaaa Acca Daaa",
    "Aac Abc Dac Acc",
    "Aaz Abz Acz Adz",
    "Aapple Apear Aorange Dpear Aapple Aapple Atomato Dapple Abanana",
    "Aaz Abz Acz Adz Aez Afz Agz Ahz Aiz Ajz Akz Alz Amz Anz Aoz Apz Aqz Arz "
    "Asz Atz Auz Avz Awz Axz Ayz"};
constexpr std::array<const char *, 7> testRestults{
    "orange strawberry ",
    "grape orange strawberry ",
    "bba cca ",
    "cc bc ",
    "bz cz dz az ",
    "banana orange tomato ",
    "bz cz dz ez fz gz hz iz jz kz lz mz nz oz pz qz rz sz tz uz vz wz xz yz "
    "az "};

class HashTable {
public:
  struct Entry {
    std::string Data;
    std::string Status = "never used";
  };
  static const int Size = 26;

public:
  HashTable() = default;

  void Add(const std::string &entryString) {
    bool exists = Find(entryString);
    if (!exists) {
      int insertIndex = GetInsetIndex(entryString);
      m_Entries[insertIndex].Data = entryString;
      m_Entries[insertIndex].Status = "occupied";
    }
  }

  void Delete(const std::string &entryString) {

    int index;
    bool exists = Find(entryString, &index);
    if (exists)
      m_Entries[index].Status = "tombstone";
  }

  std::string Print() {
    std::stringstream ss;
    for (int i = 0; i < Size; i++) {
      if (m_Entries[i].Status == "occupied")
        ss << m_Entries[i].Data << " ";
    }
    return ss.str();
  }

private:
  int GetIndex(const std::string &entryString) {
    return entryString.back() - 'a';
  }

  bool Find(const std::string &entryString, int *outIndex = nullptr) {
    int index = GetIndex(entryString);
    while (true) {

      if (m_Entries[index].Data == entryString) {
        if (outIndex)
          *outIndex = index;
        return true;
      }

      if (m_Entries[index].Status == "never used")
        return false;

      index = (index + 1) % Size;
    }

    return false;
  }

  int GetInsetIndex(const std::string &entryString) {
    int index = GetIndex(entryString);
    while (true) {

      if (m_Entries[index].Status == "never used" ||
          m_Entries[index].Status == "tombstone")
        return index;

      index = (index + 1) % Size;
    }

    return -1;
  }

private:
  Entry m_Entries[Size];
};

std::string the_cherno_main(std::string input) {

  HashTable hashTable;

  std::stringstream ss(input);
  while (ss.good()) {
    std::string token;
    ss >> token;

    std::string entryName = token.substr(1);
    if (token[0] == 'A') {
      hashTable.Add(entryName);
    } else if (token[0] == 'D') {
      hashTable.Delete(entryName);
    }
  }

  return hashTable.Print();
}

static void the_cherno_impl(benchmark::State &state) {
  for (auto _ : state) {
    for (int j = 0, l = testCases.size(); j < l; ++j) {
      auto res = the_cherno_main(testCases[j]);
      benchmark::DoNotOptimize(res.data());
#ifndef NDEBUG
      if (strcmp(res.c_str(), testRestults[j]) != 0) {
        printf("[the cherno] ERROR - Testcase: %s, Expexted: %s, Got: %s\n",
               testCases[j], testRestults[j], res.c_str());
        return;
      }
#endif
    }
  }
}

enum status_e : uint64_t {
  occupied = 0b00,
  never_used = 0b01,
  tombstone = 0b10
};

struct alignas(uint64_t) entry {
  uint64_t status : 2;
  uint64_t length : 4; // length. 4 bits max value 15, string max 10.
  uint64_t value : 50; // max 10 chars, each char 5 bits 0-26 (a-z). 50 bits.
};

struct hash_table {
  static const int size = 26;
  entry entries[size];
};

void init_table(hash_table &table) {
  for (int i = 0; i < table.size; ++i) {
    table.entries[i] = {never_used, 0, 0};
  }
}

char get_char(entry e, int index) {
  const auto indexed_value = e.value >> (index * 5);
  const auto mask = 0b11111;
  const auto result = indexed_value & mask;
  return static_cast<char>(result);
}

int get_index(entry e) { return get_char(e, e.length - 1); }

void add_entry(hash_table &table, entry e) {

  const int natural_index = get_index(e);
  if (table.entries[natural_index].status == occupied &
      table.entries[natural_index].value == e.value) {
    return;
  } else if (table.entries[natural_index].status != occupied) {
    table.entries[natural_index] = e;
    table.entries[natural_index].status = occupied;
  } else {
    int operational_index = (natural_index + 1) % table.size;
    while (natural_index != operational_index) {
      if (table.entries[operational_index].status == occupied) {
        operational_index = (operational_index + 1) % table.size;
        continue;
      }
      table.entries[operational_index] = e;
      table.entries[operational_index].status = occupied;
      break;
    }
  }
}

void remove_entry(hash_table &table, entry e) {

  const int nautal_index = get_index(e);
  int operational_index = nautal_index;
  do {
    if (table.entries[operational_index].status == occupied &
        table.entries[operational_index].value == e.value) {
      table.entries[operational_index] = {tombstone, 0, 0};
      break;
    }
    operational_index = (operational_index + 1) % table.size;

  } while (nautal_index != operational_index);
}

std::string get_output(const hash_table &table) {

  static const int buffer_length = 320;
  char buffer[buffer_length]; // 26 * (10 + 1) = 286
  memset(buffer, 0, sizeof(buffer));
  int len = 0;

  for (int i = 0; i < table.size; i++) {
    const entry e = table.entries[i];
    if (e.status == occupied) {
      for (int j = 0; j < e.length; j++) {
        buffer[len++] = get_char(e, j) + 'a';
      }
      buffer[len++] = ' ';
    }
  }

  return std::string(buffer, len);
}

entry compute_entry(const char *entry_str, const char **end_ptr) {
  entry e;
  memset(&e, 0, sizeof(entry));
  for (int i = 0; *entry_str != '\0' && *entry_str != ' '; i += 5) {
    e.length++;
    e.value = e.value | static_cast<int64_t>(*entry_str++ - 'a') << i;
  }
  *end_ptr = entry_str ? entry_str + 1 : entry_str;
  return e;
}

void print_table(const hash_table &table) {
  for (int i = 0; i < table.size; i++) {
    const entry e = table.entries[i];
    printf("idx = %d, char = %c, value = ", i, 'a' + i);
    for (int j = 0; j < e.length; j++) {
      printf("%c", get_char(e, j) + 'a');
    }
    printf("\n");
  }
}

std::string fastware_main(std::string input) {

  hash_table table;
  init_table(table);

  const char *buffer = input.c_str();
  char command = '\0';
  while (*buffer != '\0') {
    command = *buffer++;
    entry e = compute_entry(buffer, &buffer);
    if (command == 'A') {
      add_entry(table, e);
    } else if (command == 'D') {
      remove_entry(table, e);
    }
  }
  return get_output(table);
}

static void fastware_impl(benchmark::State &state) {
  for (auto _ : state) {
    for (int j = 0, l = testCases.size(); j < l; ++j) {
      auto res = fastware_main(testCases[j]);
      benchmark::DoNotOptimize(res.data());
#ifndef NDEBUG
      if (strcmp(res.c_str(), testRestults[j]) != 0) {
        printf("[fastware] ERROR - Testcase: %s, Expexted: %s, Got: %s\n",
               testCases[j], testRestults[j], res.c_str());

        return;
      }
#endif
    }
  }
}

BENCHMARK(the_cherno_impl);
BENCHMARK(fastware_impl);

BENCHMARK_MAIN();