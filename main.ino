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

enum class PlayerNumber {
  P1, P2
};

struct Player {
  Player(PlayerNumber playerNumber, int column, const Controls & controls) : 
    _column(column),
    _positions(0x38), // 00111000
    _score(0),
    _playerNumber(playerNumber), _controls(controls) {
  } 

  bool move(int ch) {
    // eh tudo ao contrario 
    bool isShot = false;
    if (ch == _controls.getUp()) {
      if (this->getPosition(7)) return; // 128
      _positions = _positions << 1;
    } else if (ch == _controls.getDown()) {
      if (this->getPosition(0)) return;
      _positions = _positions >> 1;
    } else if (ch == _controls.getRight()) {
      if (_column == 0) return;
      _column -= 1;
    } else if (ch == _controls.getLeft()) {
      if (_column == 7) return;
      _column += 1;
    } else if (ch == _controls.getFire()) {
      isShot = true;
    } else {
      // todos os outros caracteres lixo
    }

    return isShot;
  }

  int getColumn() const {
    return _column;
  }

  int getRow() const {
    for (int row = 0; row < 8; ++row) {
      if (this->getPosition(row)) {
        return row + 1; // sempre vai ser a prox
      }
    }
    return 0;
  }

  int getPosition(int row) const {
    return (_positions & (0x1 << row)) > 0;
  }

  void print(LedControl & led) const {
    for (int row = 0; row < 8; row++) {
      led.setLed(0, _column, row, this->getPosition(row));
    }
  }

  PlayerNumber getPlayerNumber() const {
    return _playerNumber;
  }

private:
  int _column; int _positions; int _score; PlayerNumber _playerNumber; Controls _controls;
};

enum class ShotStatus {
  Active, Inactive
};

struct Shot {
  Shot(PlayerNumber firedBy, int actualLed, int row, int column) :
    _actualLed(actualLed), _row(row), _column(column),
    _firedBy(firedBy), _status(ShotStatus::Active)
    {}

  void move() {
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

  int getActualLed() const {
    return _actualLed;
  }

private:
  int _actualLed; int _row; int _column; PlayerNumber _firedBy; ShotStatus _status;
};

struct Game {
  Game() {
    _shots.reserve(10); // 50 tiros por vez max
  }

  void print(LedControl (&leds)[2]) {
    for (const auto & s : _shots) {
      s.print(leds[s.getActualLed()]);
    }
  }

  void move() {
    for (const auto & s : _shots) {
      s.move();
    }
  }

  void removeInactiveShots() {
    _shots.erase(std::remove_if(_shots.begin(), _shots.end(), [](const Shot & s) {
      return s.getStatus() == ShotStatus::Inactive;
    }), _shots.end());
  }

  void addShot(const Shot & s) {
    _shots.push_back(s);
  }

private:
  std::vector<Shot> _shots;
};

const int dataPin1 = 4, clkPin1 = 6, csPin1 = 5;
const int dataPin2 = 11, clkPin2 = 13, csPin2 = 12;
LedControl leds[2] = {
  LedControl(dataPin1, clkPin1, csPin1, 1), // left
  LedControl(dataPin2, clkPin2, csPin2, 1)  // right
};

Player p1(PlayerNumber::P1, 0, Controls('w', 's', 'a', 'd', ' ')); // left
Player p2(PlayerNumber::P2, 7, Controls('8', '5', '4', '6', '0')); // right
Game game;

void clearScreen(LedControl & led) {
  for (int row = 0; row < 8; ++row) {
    led.setRow(0, row, 0x0);
  }
}

void addShot(Game & game, const Player & p) {
  PlayerNumber pNumber = p.getPlayerNumber();
  game.addShot(Shot(pNumber, static_cast<int>(pNumber), p.getRow(), p.getColumn()));
}

unsigned long timeGameRefresh, timeGameRemoveShots;
void setup() {
  Serial.begin(9600); // configura a serial para 9600

  leds[0].shutdown(0, false); leds[0].setIntensity(0, 2);
  leds[1].shutdown(0, false); leds[1].setIntensity(0, 2);

  timeGameRefresh = timeGameRemoveShots = millis();
}

void loop() {
  clearScreen(leds[0]); clearScreen(leds[1]);
  p1.print(leds[0]); p2.print(leds[1]);
  game.print(leds);

  if (millis() - timeGameRefresh > 200) {
    game.move();
    timeGameRefresh = millis();
  }

  if (millis() - timeGameRemoveShots > 5000) {
    game.removeInactiveShots();
    timeGameRemoveShots = millis();
  }

  int byteRec;
  if (Serial.available() > 0) {
    byteRec = Serial.read();
    
    bool isShot;
    isShot = p1.move(byteRec);
    if (isShot) addShot(game, p1);
    
    isShot = p2.move(byteRec);
    if (isShot) addShot(game, p2);
  }

}
