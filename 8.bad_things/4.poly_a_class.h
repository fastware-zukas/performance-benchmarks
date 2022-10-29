#ifndef POLY_A_CLASS_H
#define POLY_A_CLASS_H

#include "4.poly_interface.h"

class poly_a_class : public poly_interface {
public:
  poly_a_class(int base, int exp, int top);
  ~poly_a_class();
  int compute(int input) override;

private:
  int base_val;
  int exp_val;
  int top_val;
};

#endif // POLY_A_CLASS_H