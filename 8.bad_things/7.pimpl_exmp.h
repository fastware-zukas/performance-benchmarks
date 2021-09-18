#ifndef PIMPL_EXMP_H
#define PIMPL_EXMP_H

#include <memory>
#include <string>

class pimpl_exmp {

public:
  pimpl_exmp();
  ~pimpl_exmp();

  int getId() const;
  const std::string &getName() const;

  void setId(int id);
  void setName(std::string name);

  bool validate() const;

private:
  using deleter = void (*)(class pimpl *);
  std::unique_ptr<class pimpl, deleter> m_pimpl;
};

#endif