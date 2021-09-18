#include "7.pimpl_exmp.h"

#include <charconv>

class pimpl {

public:
  pimpl();
  ~pimpl();

  int m_id;
  std::string m_name;
};

pimpl::pimpl() {}

pimpl::~pimpl() {}

void destroy(pimpl *ptr) { delete ptr; }

pimpl_exmp::pimpl_exmp() : m_pimpl(new pimpl(), destroy) {}

pimpl_exmp::~pimpl_exmp() {}

int pimpl_exmp::getId() const { return m_pimpl->m_id; }

const std::string &pimpl_exmp::getName() const { return m_pimpl->m_name; }

void pimpl_exmp::setId(int id) { m_pimpl->m_id = id; }

void pimpl_exmp::setName(std::string name) {
  m_pimpl->m_name = std::move(name);
}

bool pimpl_exmp::validate() const {
  int val = 0;
  std::from_chars(&m_pimpl->m_name[0],
                  &m_pimpl->m_name[0] + m_pimpl->m_name.size(), val);
  return val == m_pimpl->m_id;
}
