#include "1.more_is_better.h"

int OneItemAtTheTime::oneAtTheTime(int a, int b) { return a * 3 + b; }

void manyAtTheTime(const std::vector<int> &as, const std::vector<int> &bs,
                   std::vector<int> &res) {

  // asset if a size != b size != res size

  for (int i = 0, l = as.size(); i < l; i++) {
    res[i] = as[i] * 3 + bs[i];
  }
}