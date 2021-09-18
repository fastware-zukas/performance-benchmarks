#include "4.poly_a_class.h"

poly_a_class::poly_a_class(int base, int exp, int top)
    : base_val(base), exp_val(exp), top_val(top) {}

int poly_a_class::compute(int input) {
  return top_val + (input >> base_val) * exp_val;
}