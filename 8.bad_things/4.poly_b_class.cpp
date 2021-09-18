#include "4.poly_b_class.h"

poly_b_class::poly_b_class(int sys, int core) : sys_val(sys), core_val(core) {}

int poly_b_class::compute(int input) { return (input * sys_val) >> core_val; }