#ifndef GAME_HPP
#define GAME_HPP

#include <LedControl.h>
#include <ArduinoSTL.h>

#include "Player.hpp"
#include "Shot.hpp"
#include "Sound.hpp"

enum class GameStatus {
  Active, Finished
};

struct Game {

  Game(const Player & p1, const Player & p2) : _p1(p1), _p2(p2), _status(GameStatus::Active) {
    _shots.reserve(50); // 50 tiros por vez max, 25 cada
  }

  void printScore(LedControl (&leds)[2]) {
    _p1.printScore(leds[0]); _p2.printScore(leds[1]);
  }

  void move() {
    for (const auto & s : _shots) {
      s.move();
    }
  }

  void resetPositions();

  void print(LedControl (&leds)[2]);

  void movePlayer(PlayerNumber player, int byteRec, const Sound & s);

  void movePlayers(int byteRec, const Sound & s);

  bool detectCollisions();

  void removeInactiveShots();

  void addShot(const Player & p, const Sound & s);

  void checkStatus() {
    if (_status == GameStatus::Finished) {
      _p1.resetScore();
      _p2.resetScore();
      _status = GameStatus::Active;
    }
  }

private:
  Player _p1;
  Player _p2;
  std::vector<Shot> _shots;
  GameStatus _status;
};

#endif // GAME_HPP
