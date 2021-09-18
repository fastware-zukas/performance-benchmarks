#ifndef SOA_H
#define SOA_H

#include <vector>

template <int size> struct text { char value[size]; };

struct dimension {
  double width;
  double height;
  double depth;
};

struct soa {

  std::vector<int> ids;
  std::vector<text<128>> serialNumbers;
  std::vector<text<64>> names;
  std::vector<text<1024>> descriptions;
  std::vector<int> quantities;
  std::vector<double> weights;
  std::vector<dimension> dimensions;
  std::vector<double> sellPrices;
  std::vector<text<128>> suppliers;
  std::vector<double> buyPrices;

  void reserve(int size);
};

#endif