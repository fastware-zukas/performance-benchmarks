#include "complex.h"

#include <algorithm>

ComplexSmall::ComplexSmall() { value = 1; }

ComplexSmall::~ComplexSmall() { value = 0; }

ComplexMedium::ComplexMedium() { std::fill(value.begin(), value.end(), 1); }

ComplexMedium::~ComplexMedium() { std::fill(value.begin(), value.end(), 0); }

ComplexLarge::ComplexLarge() { std::fill(value.begin(), value.end(), 1); }

ComplexLarge::~ComplexLarge() { std::fill(value.begin(), value.end(), 0); }

ComplexXLarge::ComplexXLarge() { std::fill(value.begin(), value.end(), 1); }

ComplexXLarge::~ComplexXLarge() { std::fill(value.begin(), value.end(), 0); }
