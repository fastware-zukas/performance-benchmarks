#include "soa_triangle.h"

#include <cassert>
#include <cmath>

#include <immintrin.h>

std::vector<float> calcHypotenuses(const std::vector<float> &base_data,
                                   const std::vector<float> &height_data) {

  assert(base_data.size() == height_data.size());
  std::vector<float> hypotenuses(base_data.size());

  const int size = base_data.size();
  int i = 0;

  constexpr int group = 8;

  for (const int s_g = size - (group - 1); i < s_g; i += group) {

    __m256 a = _mm256_loadu_ps(&base_data[i]);
    __m256 b = _mm256_loadu_ps(&height_data[i]);

    a = _mm256_mul_ps(a, a);
    b = _mm256_mul_ps(b, b);

    __m256 r = _mm256_add_ps(a, b);

    __m256 h = _mm256_sqrt_ps(r);

    _mm256_storeu_ps(&hypotenuses[i], h);
  }

  for (; i < size; ++i) {
    hypotenuses.push_back(
        sqrtf(base_data[i] * base_data[i] + height_data[i] * height_data[i]));
  }

  return hypotenuses;
}