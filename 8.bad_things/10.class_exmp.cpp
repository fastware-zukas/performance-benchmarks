#include "10.class_exmp.h"

#include <charconv>

MyClass::MyClass() {}

MyClass::~MyClass() {}

int MyClass::getId() const { return m_id; }

const std::string &MyClass::getName() const { return m_name; }

void MyClass::setId(int id) { m_id = id; }

void MyClass::setName(std::string name) { m_name = std::move(name); }

bool MyClass::validate() const {
  int val = 0;
  std::from_chars(&m_name[0], &m_name[0] + m_name.size(), val);
  return val == m_id;
}