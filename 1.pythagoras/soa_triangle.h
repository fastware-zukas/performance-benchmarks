#ifndef SOA_TRIANGLE_H
#define SOA_TRIANGLE_H

#include <vector>

struct soa_triangles {
  std::vector<float> base_data;
  std::vector<float> height_data;
  std::vector<float> hypotenuse_data;
};

std::vector<float> calcHypotenuses(const std::vector<float> &base_data,
                                   const std::vector<float> &height_data);

#endif