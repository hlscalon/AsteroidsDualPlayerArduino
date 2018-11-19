#include "Player.hpp"

bool Player::move(int ch) {
  // eh tudo ao contrario 
  bool isShot = false;
  if (ch == _controls.getUp()) {
    if (!this->getPosition(7)) { // 128
      _positions = _positions << 1;
    }
  } else if (ch == _controls.getDown()) {
    if (!this->getPosition(0)) {
      _positions = _positions >> 1;
    }
  } else if (ch == _controls.getRight()) {
    if (_column != 0) {
      _column -= 1;
    }
  } else if (ch == _controls.getLeft()) {
    if (_column != 7) {
      _column += 1;
    }
  } else if (ch == _controls.getFire()) {
    isShot = true;
  } else {
    // todos os outros caracteres lixo
  }

  return isShot;
}

void Player::print(LedControl & led) const {
  for (int row = 0; row < 8; row++) {
    led.setLed(0, _column, row, this->getPosition(row));
  }
}

void Player::printScore(LedControl & led) const {
  int numbers[8];
  Utils::getNumberMatrix(numbers, _score);
  for (int row = 0; row < 8; ++row) {
    led.setRow(0, row, numbers[row]);
  }
}
