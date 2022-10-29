#ifndef BASELINE_H
#define BASELINE_H

#include <common_types.h>
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace baseline {

class Dimentions {
public:
  Dimentions();
  Dimentions(uint32_t width, uint32_t height, uint32_t depth);

  uint32_t getWidth() const;
  uint32_t getHeight() const;
  uint32_t getDepth() const;

  void setWidth(uint32_t width);
  void setHeight(uint32_t height);
  void setDepth(uint32_t depth);

private:
  uint32_t d_width;
  uint32_t d_height;
  uint32_t d_depth;
};

class Item {

public:
  Item();
  Item(uint32_t id, const std::string &name,
       const std::string &marketIdentifier, const Dimentions &dimentions);

  uint32_t getId() const;
  const std::string &getName() const;
  const std::string &getMarketIdentifier() const;
  const Dimentions &getDimentions() const;

  void setId(uint32_t id);
  void setName(const std::string &name);
  void setMarketIdentifier(const std::string &marketIdentifier);
  void setDimentions(const Dimentions &dimentions);

private:
  uint32_t d_id;
  std::string d_name;
  std::string d_marketIdentifier;
  Dimentions d_dimentions;
};

class Order {
public:
  Order();
  Order(uint32_t id, const std::string &userName,
        const std::string &shippingAddress,
        const std::vector<std::shared_ptr<Item>> &items);

  uint32_t getId() const;
  const std::string &getUserName() const;
  const std::string &getShippingAddress() const;
  const std::vector<std::shared_ptr<Item>> &getItems() const;

  void setId(uint32_t id);
  void setUserName(const std::string &userName);
  void setShippingAddress(const std::string &shippingAddress);
  void setItems(const std::vector<std::shared_ptr<Item>> &items);

  void addItem(const std::shared_ptr<Item> &item);

private:
  uint32_t d_id;
  std::string d_userName;
  std::string d_shippingAddress;
  std::vector<std::shared_ptr<Item>> d_items;
};

class OrderManager {
public:
  OrderManager(const std::vector<item_row> &items,
               const std::vector<order_row> &orders,
               const std::vector<order_item_row> &order_items);

  const std::vector<std::shared_ptr<Item>> &items() const;
  const std::vector<std::shared_ptr<Order>> &orders() const;

  Dimentions getVolume(const std::vector<uint32_t> &orderIds) const;
  std::vector<uint32_t>
  getUserOrdersIds(const std::vector<std::string_view> &userNames) const;

  std::vector<uint32_t>
  getUserItemIds(const std::vector<std::string_view> &userNames) const;

private:
  std::vector<std::shared_ptr<Item>> d_items;
  std::vector<std::shared_ptr<Order>> d_orders;

  std::unordered_map<uint32_t, std::shared_ptr<Order>> d_orderMap;
  std::unordered_map<std::string_view, std::vector<std::shared_ptr<Order>>>
      d_userOrders;
};

} // namespace baseline

#endif