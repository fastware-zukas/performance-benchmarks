#ifndef ITEM_H
#define ITEM_H

#include <memory>
#include <string>
#include <vector>

class Item {
private:
  int m_id;
  std::string m_serialNumber;
  std::string m_name;
  std::string m_description;
  int m_quantity;
  double m_weight;
  double m_width;
  double m_height;
  double m_depth;
  double m_sellPrice;
  std::string m_supplier;
  double m_buyPrice;

public:
  Item();
  ~Item();

  int GetId() const;
  void SetId(int id);

  const std::string &GetSerialNumber() const;
  void SetSerialNumber(const std::string &sn);

  const std::string &GetName() const;
  void SetName(const std::string &name);

  const std::string &GetDescription() const;
  void SetDescription(const std::string &desc);

  int GetQuantity() const;
  void SetQuantity(int quantity);

  double GetWeight() const;
  void SetWeight(double weight);

  double GetWidth() const;
  void SetWidth(double width);

  double GetHeight() const;
  void SetHeight(double height);

  double GetDepth() const;
  void SetDepth(double depth);

  double GetSellPrice() const;
  void SetSellPrice(double sp);

  const std::string &GetSupplier() const;
  void SetSupplier(const std::string &supplier);

  double GetBuyPrice() const;
  void SetBuyPrice(double bp);
};

#endif