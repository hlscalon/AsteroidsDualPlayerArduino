#include "LedControl.h"

struct Controls {
  Controls(int chUp, int chDown, int chRight, int chLeft) :
    _chUp(chUp), _chDown(chDown), _chRight(chRight), _chLeft(chLeft) {}

  int getUp() { return _chUp; }
  int getDown() { return _chDown; }
  int getRight() { return _chRight; }
  int getLeft() { return _chLeft; }

private:
  int _chUp; int _chDown; int _chRight; int _chLeft;
};

struct Player {
  Player(int column, const Controls & controls, int dataPin, int clkPin, int csPin) : 
    _column(column), _controls(controls),
    _positions(0x18), // 00011000
    _score(0), _lastColumn(-1) {
    _ledControl = LedControl(dataPin, clkPin, csPin, 1);
    _ledControl.shutdown(0, false);
    _ledControl.setIntensity(0, 2);
  } 

  void mover(int ch) {
    // eh tudo ao contrario 
    if (ch == _controls.getUp()) {
      if (this->getPosition(7)) return; // 128
      _positions = _positions << 1;
    } else if (ch == _controls.getDown()) {
      if (this->getPosition(0)) return;
      _positions = _positions >> 1;
    } else if (ch == _controls.getRight()) {
      if (_column == 0) return;
      _lastColumn = _column;
      _column -= 1;
    } else if (ch == _controls.getLeft()) {
      if (_column == 7) return;
      _lastColumn = _column;
      _column += 1;
    } else {
      // todos os outros caracteres lixo
    }
  }

  int getColumn() {
    return _column;
  }

  int getPosition(int row) {
    return (_positions & (0x1 << row)) > 0;
  }

  void print() {
    // limpar coluna
    if (_lastColumn > -1) {
      for (int row = 0; row < 8; row++) {
        _ledControl.setLed(0, _lastColumn, row, 0);
      }
      _lastColumn = -1;
    }
    // posicao atual
    for (int row = 0; row < 8; row++) {
      _ledControl.setLed(0, _column, row, this->getPosition(row));
    }
  }

private:
  int _column;
  int _positions; // 00000000
  int _score;
  int _lastColumn;
  Controls _controls;
  LedControl _ledControl = LedControl(0, 0, 0, 1);
};

Player p1(0, Controls('w', 's', 'a', 'd'), 4, 6, 5);
Player p2(7, Controls('8', '5', '4', '6'), 11, 13, 12);

void setup() {
  Serial.begin(9600); // configura a serial para 9600
}

void loop() {
  p1.print(); p2.print();

  int byteRec; 
  if (Serial.available() > 0) {
    byteRec = Serial.read();
    p1.mover(byteRec);
    p2.mover(byteRec);
  }
}
