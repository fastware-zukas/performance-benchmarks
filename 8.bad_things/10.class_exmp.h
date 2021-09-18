#ifndef CLASS_EXMP_H
#define CLASS_EXMP_H

#include <string>

class MyClass {

public:
  MyClass();
  ~MyClass();

  int getId() const;
  const std::string &getName() const;

  void setId(int id);
  void setName(std::string name);

  int m_id;
  std::string m_name;
  // many ofther members

  bool validate() const;
};

#endif