#include "3.pointers.h"

#include "3.dependency.h"

SomeClass::SomeClass(std::shared_ptr<DependClass> dep) : dependency(dep) {}

int SomeClass::calculateAnswer() {
  return dependency->getA() * 3 + dependency->getB();
}

std::unique_ptr<ResultClass> SomeClass::calculateResult() {
  return std::make_unique<ResultClass>(calculateAnswer());
}