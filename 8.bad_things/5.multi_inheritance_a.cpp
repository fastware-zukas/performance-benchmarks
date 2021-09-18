#include "5.multi_inheritance.h"

#include <algorithm>
#include <benchmark/benchmark.h>

void Printer::print(const std::vector<Pixel> &pixels) {
  backBuffer.resize(pixels.size());
  std::copy(pixels.begin(), pixels.end(), backBuffer.begin());
  benchmark::DoNotOptimize(backBuffer.data());
}