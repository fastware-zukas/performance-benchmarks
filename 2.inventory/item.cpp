#include "item.h"

Item::Item() {}

Item::~Item() {}

int Item::GetId() const { return m_id; }
void Item::SetId(int id) { m_id = id; }

const std::string &Item::GetSerialNumber() const { return m_serialNumber; }
void Item::SetSerialNumber(const std::string &sn) { m_serialNumber = sn; }

const std::string &Item::GetName() const { return m_name; }
void Item::SetName(const std::string &name) { m_name = name; }

const std::string &Item::GetDescription() const { return m_description; }
void Item::SetDescription(const std::string &desc) { m_description = desc; }

int Item::GetQuantity() const { return m_quantity; }
void Item::SetQuantity(int quantity) { m_quantity = quantity; }

double Item::GetWeight() const { return m_weight; }
void Item::SetWeight(double weight) { m_weight = weight; }

double Item::GetWidth() const { return m_width; }
void Item::SetWidth(double width) { m_width = width; }

double Item::GetHeight() const { return m_height; }
void Item::SetHeight(double height) { m_height = height; }

double Item::GetDepth() const { return m_depth; }
void Item::SetDepth(double depth) { m_depth = depth; }

double Item::GetSellPrice() const { return m_sellPrice; }
void Item::SetSellPrice(double sp) { m_sellPrice = sp; }

const std::string &Item::GetSupplier() const { return m_supplier; }
void Item::SetSupplier(const std::string &supplier) { m_supplier = supplier; }

double Item::GetBuyPrice() const { return m_buyPrice; }
void Item::SetBuyPrice(double bp) { m_buyPrice = bp; }