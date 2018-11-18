#ifndef GAME_HPP
#define GAME_HPP

#include <LedControl.h>
#include <ArduinoSTL.h>

#include "Player.hpp"
#include "Shot.hpp"
#include "Sound.hpp"

struct Game {

  Game(const Player & p1, const Player & p2) : _p1(p1), _p2(p2) {
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

  void Game::movePlayers(int byteRec, const Sound & s);

  bool detectCollisions();

  void removeInactiveShots();

  void addShot(const Player & p, const Sound & s);

private:
  Player _p1;
  Player _p2;
  std::vector<Shot> _shots;
};

#endif // GAME_HPP
