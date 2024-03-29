#ifndef POLY_INTERFACE_H
#define POLY_INTERFACE_H

#include <vector>

class poly_interface {
public:
  virtual int compute(int) = 0;
  virtual ~poly_interface() = default;
};

#endif // POLY_INTERFACE_H