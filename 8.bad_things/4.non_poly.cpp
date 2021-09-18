#include "4.non_poly.h"

int type_a_compute(type_a *obj, int input) {
  return obj->top_val + (input >> obj->base_val) * obj->exp_val;
}

int type_b_compute(type_b *obj, int input) {
  return (input * obj->sys_val) >> obj->core_val;
}