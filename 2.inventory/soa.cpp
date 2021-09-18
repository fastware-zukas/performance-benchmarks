#include "soa.h"

void soa::reserve(int size) {
  ids.reserve(size);
  serialNumbers.reserve(size);
  names.reserve(size);
  descriptions.reserve(size);
  quantities.reserve(size);
  weights.reserve(size);
  dimensions.reserve(size);
  sellPrices.reserve(size);
  suppliers.reserve(size);
  buyPrices.reserve(size);
}