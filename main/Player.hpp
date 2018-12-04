#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <LedControl.h>

#include "Controls.hpp"
#include "Utils.hpp"

enum class PlayerNumber {
  P1, P2
};

enum class PlayerStatus {
  Alive, Dead
};

struct Player {
  Player(PlayerNumber playerNumber, int column, const Controls & controls) : 
    _column(column), _defaultColumn(column),
    _positions(0x38), _defaultPositions(0x38),// 00111000
    _score(0), _status(PlayerStatus::Alive),
    _playerNumber(playerNumber), _controls(controls) {
  }

  Player() {}

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

  void resetPositions() {
    _column = _defaultColumn; _positions = _defaultPositions;
    _status = PlayerStatus::Alive;
  }

  PlayerStatus getStatus() const {
    return _status;
  }

  void died() {
    _status = PlayerStatus::Dead;
  }

  PlayerNumber getPlayerNumber() const {
    return _playerNumber;
  }

  int incScore() {
    _score++;
  }

  int getScore() const {
    return _score;
  }

  void resetScore() {
    _score = 0;
  }

  bool move(int ch);

  void print(LedControl & led) const;

  void printScore(LedControl & led) const;

private:
  int _column;
  int _defaultColumn;
  int _positions;
  int _defaultPositions;
  int _score;
  PlayerStatus _status;
  PlayerNumber _playerNumber;
  Controls _controls;
};

#endif // PLAYER_HPP
