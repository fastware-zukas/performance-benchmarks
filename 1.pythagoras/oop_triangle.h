#ifndef OOP_TRIANGLE_H
#define OOP_TRIANGLE_H

class oop_triangle {
private:
  float m_base;
  float m_height;
  float m_hypotenuse;

public:
  oop_triangle(float base, float height);
  ~oop_triangle();

  void calcHypotenuse();

  float base() const;

  float heigh() const;

  float hypotenuse() const;
};

#endif