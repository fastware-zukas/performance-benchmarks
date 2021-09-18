#include "3.dependency.h"

DependClass::DependClass(int a, int b) : av(a), ab(b) {}

int DependClass::getA() const { return av; }

int DependClass::getB() const { return ab; }

ResultClass::ResultClass(int r) : res(r) {}

int ResultClass::getResult() const { return res; }