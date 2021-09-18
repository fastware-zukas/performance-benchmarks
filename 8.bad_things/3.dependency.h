#ifndef DEPENDENCY_H
#define DEPENDENCY_H

class DependClass {

public:
  DependClass(int a, int b);

  int getA() const;
  int getB() const;

private:
  int av;
  int ab;
};

class ResultClass {
public:
  ResultClass(int r);
  int getResult() const;

private:
  int res;
};

#endif // DEPENDENCY_H