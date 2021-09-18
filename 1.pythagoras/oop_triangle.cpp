#include "oop_triangle.h"

#include <cmath>

oop_triangle::oop_triangle(float base, float height)
    : m_base(base), m_height(height), m_hypotenuse(0) {}

oop_triangle::~oop_triangle() {}

void oop_triangle::calcHypotenuse() {
  m_hypotenuse = sqrtf(m_base * m_base + m_height * m_height);
}

float oop_triangle::base() const { return m_base; }

float oop_triangle::heigh() const { return m_height; }

float oop_triangle::hypotenuse() const { return m_hypotenuse; }