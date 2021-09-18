#include "5.multi_inheritance.h"

Scanner::Scanner(std::vector<Pixel> buffer) : frontBuffer(std::move(buffer)) {}

std::vector<Pixel> Scanner::scan() {

  for (int i = 0; i < frontBuffer.size(); ++i) {
    frontBuffer[i].red |= 0x11010011010;
    frontBuffer[i].green |= 0x1001010001;
    frontBuffer[i].blue |= 0x110101001100;
  }

  return frontBuffer;
}