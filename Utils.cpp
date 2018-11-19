#include <LedControl.h>
#include "Utils.hpp"

namespace Utils {

  void clearScreen(LedControl & led) {
    for (int row = 0; row < 8; ++row) {
      led.setRow(0, row, 0x0);
    }
  }

  void clearBuffer() {
    while (Serial.available()) {
      char l = Serial.read();
    }
  }

  void getNumberMatrix(int (&numbers)[8], int score) {
    switch (score) {
      case 0:
        numbers[0] = numbers[7] = numbers[1] = numbers[6] = 0x0;
        numbers[2] = numbers[5] = 0x7E;
        numbers[3] = numbers[4] = 0x42;
        return;
      case 1:
        numbers[0] = numbers[1] = numbers[2] = numbers[6] = numbers[7] = 0x0;
        numbers[4] = numbers[5] = 0x7E;
        numbers[3] = 0x06;
        return;
      case 2:
        numbers[0] = numbers[1] = numbers[6] = numbers[7] = 0x0;
        numbers[2] = 0x7A;
        numbers[3] = numbers[4] = 0x5A;
        numbers[5] = 0x5E;
        return;
      case 3:
        numbers[0] = numbers[1] = numbers[6] = numbers[7] = 0x0;
        numbers[2] = numbers[3] = numbers[4] = 0x5A;
        numbers[5] = 0x7E;
        return;
      case 4:
        numbers[0] = numbers[1] = numbers[6] = numbers[7] = 0x0;
        numbers[2] = 0x1E;
        numbers[3] = numbers[4] = 0x18;
        numbers[5] = 0x7E;
        return;
      case 5:
        numbers[0] = numbers[1] = numbers[6] = numbers[7] = 0x0;
        numbers[2] = 0x5E;
        numbers[3] = numbers[4] = 0x5A;
        numbers[5] = 0x7A;
        return;
    }

    for (int row = 0; row < 8; ++row) numbers[row] = 0x00;
  }

};
