#ifndef PRINT_SCAN_H
#define PRINT_SCAN_H

#include <cstdint>
#include <limits>
#include <random>
#include <string>
#include <vector>

struct pixel_t {
  uint16_t red;
  uint16_t green;
  uint16_t blue;
};

struct all_in_one_t {
  std::string uri;
  int port;
  std::vector<pixel_t> frontBuffer;
  std::vector<pixel_t> backBuffer;
};

void print(all_in_one_t *obj, const std::vector<pixel_t> &buffer);

void scan(all_in_one_t *obj, std::vector<pixel_t> *pixels);

void copy(all_in_one_t *obj);

#endif // PRINT_SCAN_H