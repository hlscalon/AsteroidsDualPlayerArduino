#include "Shot.hpp"

void Shot::move() {
  if (_firedBy == PlayerNumber::P1) {
    if (_column == 7 && _actualLed == 1) {
      _status = ShotStatus::Inactive;
    } else if (_column == 7 && _actualLed == 0) {
      _column = 0;
      _actualLed = 1;
    } else {
      _column++;
    }
  } else {
    if (_column == 0 && _actualLed == 0) {
      _status = ShotStatus::Inactive;
    } else if (_column == 0) {
      _column = 7;
      _actualLed = 0;
    } else {
      _column--;
    }
  }
}
