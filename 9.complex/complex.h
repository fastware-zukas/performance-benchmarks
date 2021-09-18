#ifndef COMPLEX_H
#define COMPLEX_H

#include <array>

class ComplexSmall {

public:
  ComplexSmall();
  ~ComplexSmall();

  int value;
};

class ComplexMedium {

public:
  ComplexMedium();
  ~ComplexMedium();

  std::array<int, 16> value;
};

class ComplexLarge {

public:
  ComplexLarge();
  ~ComplexLarge();

  std::array<int, 32> value;
};

class ComplexXLarge {

public:
  ComplexXLarge();
  ~ComplexXLarge();

  std::array<int, 64> value;
};

#endif // COMPLEX_H