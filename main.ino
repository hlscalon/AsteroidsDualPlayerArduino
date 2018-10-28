#include <LedControl.h>
#include <ArduinoSTL.h>

struct Controls {
  Controls(int chUp, int chDown, int chRight, int chLeft, int chFire) :
    _chUp(chUp), _chDown(chDown), _chRight(chRight), _chLeft(chLeft), _chFire(chFire) {}

  int getUp() { return _chUp; }
  int getDown() { return _chDown; }
  int getRight() { return _chRight; }
  int getLeft() { return _chLeft; }
  int getFire() { return _chFire; }

private:
  int _chUp; int _chDown; int _chRight; int _chLeft; int _chFire;
};

struct Player {
  Player(int column, const Controls & controls) : 
    _column(column), _controls(controls),
    _positions(0x18), // 00011000
    _score(0), _lastColumn(-1) {
  } 

  void move(int ch) {
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

  int getPosition(int row) {
    return (_positions & (0x1 << row)) > 0;
  }

  void print(LedControl * led) {
    // limpar coluna
    if (_lastColumn > -1) {
      for (int row = 0; row < 8; row++) {
        led->setLed(0, _lastColumn, row, 0);
      }
      _lastColumn = -1;
    }
    // posicao atual
    for (int row = 0; row < 8; row++) {
      led->setLed(0, _column, row, this->getPosition(row));
    }
  }

private:
  int _column;
  int _positions; // 00000000
  int _score;
  int _lastColumn;
  Controls _controls;
};

struct Shot {

private:
  int _firedBy;
  int _actualLed;
};

struct Game {

private:
  std::vector<Shot> shots;
};

const int dataPin1 = 4, clkPin1 = 6, csPin1 = 5;
const int dataPin2 = 11, clkPin2 = 13, csPin2 = 12;
LedControl leds[2] = {
  LedControl(dataPin1, clkPin1, csPin1, 1), // left
  LedControl(dataPin2, clkPin2, csPin2, 1)  // right
};

Player p1(0, Controls('w', 's', 'a', 'd', ' '));
Player p2(7, Controls('8', '5', '4', '6', '0'));
Game game;

void setup() {
  Serial.begin(9600); // configura a serial para 9600

  leds[0].shutdown(0, false);
  leds[0].setIntensity(0, 2);
  leds[1].shutdown(0, false);
  leds[1].setIntensity(0, 2);
}

void loop() {
  p1.print(&leds[0]); p2.print(&leds[1]);

  int byteRec; 
  if (Serial.available() > 0) {
    byteRec = Serial.read();
    p1.move(byteRec);
    p2.move(byteRec);
  }
}
