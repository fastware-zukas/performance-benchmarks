#ifndef MORE_IS_BETTER_H
#define MORE_IS_BETTER_H

#include <vector>

class OneItemAtTheTime {
public:
  int oneAtTheTime(int a, int b);
};

void manyAtTheTime(const std::vector<int> &as, const std::vector<int> &bs,
                   std::vector<int> &res);

#endif // MORE_IS_BETTER_H
