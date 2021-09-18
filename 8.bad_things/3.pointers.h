#ifndef POINTERS_H
#define POINTERS_H

#include <memory>

class DependClass;
class ResultClass;

class SomeClass {
public:
  SomeClass(std::shared_ptr<DependClass> dep);

  int calculateAnswer();

  std::unique_ptr<ResultClass> calculateResult();

private:
  std::shared_ptr<DependClass> dependency;
};

#endif // POINTERS_H