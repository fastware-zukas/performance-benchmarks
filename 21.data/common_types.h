#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <cstdint>
#include <string_view>

struct item_row {
  uint32_t id;
  std::string_view name;
  std::string_view market_identifier;
  uint32_t width;
  uint32_t height;
  uint32_t depth;
};

struct order_row {
  uint32_t id;
  std::string_view user_name;
  std::string_view shipping_address;
};

struct order_item_row {
  uint32_t order_id;
  uint32_t item_id;
};

#endif // COMMON_TYPES_H