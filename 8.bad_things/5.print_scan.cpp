#include "5.print_scan.h"

#include <algorithm>
#include <benchmark/benchmark.h>

void print(all_in_one_t *obj, const std::vector<pixel_t> &buffer) {
  obj->backBuffer.resize(buffer.size());
  std::copy(buffer.begin(), buffer.end(), obj->backBuffer.begin());
  benchmark::DoNotOptimize(obj->backBuffer.data());
}

void scan(all_in_one_t *obj, std::vector<pixel_t> *pixels) {
  pixels->resize(obj->frontBuffer.size());
  for (int i = 0, l = obj->frontBuffer.size(); i < l; ++i) {
    (*pixels)[i].red = obj->frontBuffer[i].red | 0x11010011010;
    (*pixels)[i].green = obj->frontBuffer[i].green | 0x1001010001;
    (*pixels)[i].blue = obj->frontBuffer[i].blue | 0x110101001100;
  }
}

void copy(all_in_one_t *obj) {
  for (int i = 0, l = obj->frontBuffer.size(); i < l; ++i) {
    obj->frontBuffer[i].red |= 0x11010011010;
    obj->frontBuffer[i].green |= 0x1001010001;
    obj->frontBuffer[i].blue |= 0x110101001100;
  }
  obj->backBuffer.resize(obj->frontBuffer.size());
  std::copy(obj->frontBuffer.begin(), obj->frontBuffer.end(),
            obj->backBuffer.begin());
  benchmark::DoNotOptimize(obj->backBuffer.data());
}