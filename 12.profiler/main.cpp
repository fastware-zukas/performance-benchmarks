
#include <chrono>
#include <cmath>
#include <cstdio>

#include <pthread.h>

/*
 *
 * Profiler implementation
 *
 */
class Profiler {
  using clock_t = std::chrono::high_resolution_clock;

public:
  Profiler(const char *function, const char *file, long line)
      : m_start_time(clock_t::now()), m_function(function), m_file(file),
        m_line(line) {}
  ~Profiler() {
    const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
                              clock_t::now() - m_start_time)
                              .count();

    const auto thread_id = pthread_self();

    printf("{ \"thread\" : %lu, \"file\" : \"%s\", \"line\" : %ld, "
           "\"function\" : \"%s\", \"duration\" : %.3f  }\n",
           thread_id, m_file, m_line, m_function, double(duration) / 1000.f);
  }

private:
  const clock_t::time_point m_start_time;
  const char *m_function;
  const char *m_file;
  const long m_line;
};

/*
 *
 * Utility macros
 *
 */
#define __PERF_INIT(x, y) Profiler x##y(__FUNCTION__, __FILE__, __LINE__)
#define __PERF_APPEND(x, y) __PERF_INIT(x, y)
#define PROFILE() __PERF_APPEND(__perf_, __COUNTER__)

/*
 *
 * Test function
 *
 */
__attribute_noinline__ uint64_t do_something(uint64_t break_val) {
  PROFILE();

  uint64_t res = (uint64_t(std::sqrt(break_val)) >> 1) + 1;
  res = (uint64_t(std::sqrt(res)) << 1) + 1;
  res = (uint64_t(std::sqrt(res)) >> 1) + 1;
  res = (uint64_t(std::sqrt(res)) << 1) + 1;

  return res;
}

/*
 *
 * Driver code
 *
 */
int main(int argc, const char **argv) {

  PROFILE();
  uint64_t br = 0;
  {
    PROFILE();
    printf("argc = %d\n", argc);
    br = argc == 2 ? atol(argv[1]) : 10000000000;
  }
  uint64_t res = 0;
  {
    PROFILE();
    res = do_something(br);
  }

  {
    PROFILE();
    printf("res = %lu\n", res);
  }

  {
    PROFILE();
    { PROFILE(); }
  }

  return 0;
}