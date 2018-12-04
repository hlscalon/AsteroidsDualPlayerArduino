#ifndef UTILS_HPP
#define UTILS_HPP

#include <SoftwareSerial.h>
#include <LedControl.h>
#include <AltSoftSerial.h>

namespace Utils {

  void clearScreen(LedControl & led);
  void getNumberMatrix(int (&numbers)[8], int score);
  void clearBufferSerial();
  void clearBuffer(SoftwareSerial & serial);
  void clearBuffer(AltSoftSerial & serial);

};

#endif // UTILS_HPP
