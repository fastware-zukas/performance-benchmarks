#ifndef POLY_B_CLASS_H
#define POLY_B_CLASS_H

#include "4.poly_interface.h"

class poly_b_class : public poly_interface {
public:
  poly_b_class(int sys, int core);
  ~poly_b_class();
  int compute(int input) override;

private:
  int sys_val;
  int core_val;
};

#endif // POLY_B_CLASS_H