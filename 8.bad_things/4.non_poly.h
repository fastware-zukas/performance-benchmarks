#ifndef NON_POLY_H
#define NON_POLY_H

#include <vector>

struct type_a {
  int base_val;
  int exp_val;
  int top_val;
};

struct type_b {
  int sys_val;
  int core_val;
};

int type_a_compute(type_a *obj, int input);

int type_b_compute(type_b *obj, int input);

#endif // NON_POLY_H
