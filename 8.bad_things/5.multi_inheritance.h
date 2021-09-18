#ifndef multi_inheritance_h
#define multi_inheritance_h

#include <cstdint>
#include <limits>
#include <random>
#include <string>
#include <vector>

struct Pixel {
  uint16_t red;
  uint16_t green;
  uint16_t blue;
};

class Printer {
public:
  void print(const std::vector<Pixel> &pixels);

private:
  std::string uri;
  int port;
  std::vector<Pixel> backBuffer;
};

class Scanner {
public:
  Scanner(std::vector<Pixel> buffer);
  std::vector<Pixel> scan();

private:
  std::string uri;
  int port;
  std::vector<Pixel> frontBuffer;
};

class AllInOne : public Printer, public Scanner {
public:
  AllInOne(std::vector<Pixel> scannerBuffer)
      : Scanner(std::move(scannerBuffer)) {}
  void copy() { Printer::print(Scanner::scan()); }
};

#endif // multi_inheritance_h
