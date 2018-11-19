#ifndef UTILS_HPP
#define UTILS_HPP

namespace Utils {

  void clearScreen(LedControl & led);
  void getNumberMatrix(int (&numbers)[8], int score);
  void clearBuffer();
  
};

#endif // UTILS_HPP
