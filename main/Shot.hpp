#ifndef SHOT_HPP
#define SHOT_HPP

#include <LedControl.h>

#include "Player.hpp"

enum class ShotStatus {
  Active, Inactive
};

struct Shot {
  Shot(PlayerNumber firedBy, int actualLed, int row, int column) :
    _actualLed(actualLed), _row(row), _column(column),
    _firedBy(firedBy), _status(ShotStatus::Active)
    {}

  void changeStatus(ShotStatus status) {
    _status = status;
  }

  ShotStatus getStatus() const {
    return _status;
  }

  void print(LedControl & led) const {
    if (_status == ShotStatus::Active) {
      led.setLed(0, _column, _row, 1);
    }
  }

  int getColumn() const {
    return _column;
  }

  int getRow() const {
    return _row;
  }

  PlayerNumber getFiredBy() const {
    return _firedBy;
  }

  int getActualLed() const {
    return _actualLed;
  }

  void move();

private:
  int _actualLed;
  int _row;
  int _column;
  PlayerNumber _firedBy;
  ShotStatus _status;
};

#endif // SHOT_HPP
