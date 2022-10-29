#include "Baseline.h"

#include <algorithm>

namespace baseline {

// Dimentions

Dimentions::Dimentions() = default;

Dimentions::Dimentions(uint32_t width, uint32_t height, uint32_t depth)
    : d_width(width), d_height(height), d_depth(depth) {}

uint32_t Dimentions::getWidth() const { return d_width; }

uint32_t Dimentions::getHeight() const { return d_height; }

uint32_t Dimentions::getDepth() const { return d_depth; }

void Dimentions::setWidth(uint32_t width) { d_width = width; }

void Dimentions::setHeight(uint32_t height) { d_height = height; }

void Dimentions::setDepth(uint32_t depth) { d_depth = depth; }

// Item

Item::Item() = default;

Item::Item(uint32_t id, const std::string &name,
           const std::string &marketIdentifier, const Dimentions &dimentions)
    : d_id(id), d_name(name), d_marketIdentifier(marketIdentifier),
      d_dimentions(dimentions) {}

uint32_t Item::getId() const { return d_id; }

const std::string &Item::getName() const { return d_name; }

const std::string &Item::getMarketIdentifier() const {
  return d_marketIdentifier;
}

const Dimentions &Item::getDimentions() const { return d_dimentions; }

void Item::setId(uint32_t id) { d_id = id; }

void Item::setName(const std::string &name) { d_name = name; }

void Item::setMarketIdentifier(const std::string &marketIdentifier) {
  d_marketIdentifier = marketIdentifier;
}

void Item::setDimentions(const Dimentions &dimentions) {
  d_dimentions = dimentions;
}

// Order

Order::Order() = default;

Order::Order(uint32_t id, const std::string &userName,
             const std::string &shippingAddress,
             const std::vector<std::shared_ptr<Item>> &items)
    : d_id(id), d_userName(userName), d_shippingAddress(shippingAddress),
      d_items(items) {}

uint32_t Order::getId() const { return d_id; }

const std::string &Order::getUserName() const { return d_userName; }

const std::string &Order::getShippingAddress() const {
  return d_shippingAddress;
}

const std::vector<std::shared_ptr<Item>> &Order::getItems() const {
  return d_items;
}

void Order::setId(uint32_t id) { d_id = id; }

void Order::setUserName(const std::string &userName) { d_userName = userName; }

void Order::setShippingAddress(const std::string &shippingAddress) {
  d_shippingAddress = shippingAddress;
}

void Order::setItems(const std::vector<std::shared_ptr<Item>> &items) {
  d_items = items;
}

void Order::addItem(const std::shared_ptr<Item> &item) {
  d_items.push_back(item);
}

// OrderManager

OrderManager::OrderManager(const std::vector<item_row> &items,
                           const std::vector<order_row> &orders,
                           const std::vector<order_item_row> &order_items) {

  std::unordered_map<uint32_t, std::shared_ptr<Item>> itemMap;

  for (const auto &item_data : items) {
    auto item = std::make_shared<Item>(
        item_data.id, std::string(item_data.name),
        std::string(item_data.market_identifier),
        Dimentions(item_data.width, item_data.height, item_data.depth));
    d_items.push_back(item);
    itemMap[item->getId()] = item;
  }

  for (const auto &order_data : orders) {

    auto order = std::make_shared<Order>(
        order_data.id, std::string(order_data.user_name),
        std::string(order_data.shipping_address),
        std::vector<std::shared_ptr<baseline::Item>>{});

    d_orders.push_back(order);
    d_orderMap[order->getId()] = order;
    d_userOrders[order->getUserName()].push_back(order);
  }

  for (const auto &order_item_data : order_items) {
    auto &order = d_orderMap[order_item_data.order_id];
    order->addItem(itemMap[order_item_data.item_id]);
  }
}

const std::vector<std::shared_ptr<Item>> &OrderManager::items() const {
  return d_items;
}

const std::vector<std::shared_ptr<Order>> &OrderManager::orders() const {
  return d_orders;
}

Dimentions
OrderManager::getVolume(const std::vector<uint32_t> &orderIds) const {

  Dimentions dim(0, 0, 0);
  for (auto orderId : orderIds) {
    if (auto it = d_orderMap.find(orderId); it != d_orderMap.end()) {

      for (const auto &order_item : it->second->getItems()) {

        if (dim.getWidth() == 0) {
          dim = order_item->getDimentions();
          continue;
        }

        auto w = dim.getWidth() + order_item->getDimentions().getWidth();
        auto h = dim.getHeight() + order_item->getDimentions().getHeight();
        auto d = dim.getDepth() + order_item->getDimentions().getDepth();

        auto maxw =
            std::max(dim.getWidth(), order_item->getDimentions().getWidth());
        auto maxh =
            std::max(dim.getHeight(), order_item->getDimentions().getHeight());
        auto maxd =
            std::max(dim.getDepth(), order_item->getDimentions().getDepth());

        auto wv = w * maxh * maxd;
        auto hv = maxw * h * maxd;
        auto dv = maxw * maxh * d;

        if (wv <= hv && wv <= dv) {
          dim.setWidth(w);
          dim.setHeight(maxh);
          dim.setDepth(maxd);
          continue;
        }

        if (hv <= wv && hv <= dv) {
          dim.setWidth(maxw);
          dim.setHeight(h);
          dim.setDepth(maxd);
          continue;
        }

        if (dv <= wv && dv <= hv) {
          dim.setWidth(maxw);
          dim.setHeight(maxh);
          dim.setDepth(d);
          continue;
        }
      }
    }
  }

  return dim;
}

std::vector<uint32_t> OrderManager::getUserOrdersIds(
    const std::vector<std::string_view> &userNames) const {

  std::vector<uint32_t> orderIds;
  for (auto userName : userNames) {
    if (auto it = d_userOrders.find(userName); it != d_userOrders.end()) {

      std::transform(it->second.begin(), it->second.end(),
                     std::back_inserter(orderIds),
                     [](const auto &order) { return order->getId(); });
    }
  }

  return orderIds;
}

std::vector<uint32_t> OrderManager::getUserItemIds(
    const std::vector<std::string_view> &userNames) const {
  std::vector<uint32_t> itemIds;
  for (auto userName : userNames) {
    if (auto it = d_userOrders.find(userName); it != d_userOrders.end()) {

      for (const auto &order : it->second) {
        std::transform(order->getItems().begin(), order->getItems().end(),
                       std::back_inserter(itemIds),
                       [](const auto &item) { return item->getId(); });
      }
    }
  }

  std::sort(itemIds.begin(), itemIds.end());
  itemIds.erase(std::unique(itemIds.begin(), itemIds.end()), itemIds.end());

  return itemIds;
}

} // namespace baseline
