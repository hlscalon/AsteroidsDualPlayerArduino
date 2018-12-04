#include "Game.hpp"

void Game::resetPositions() {
  _p1.resetPositions();
  _p2.resetPositions();
  _shots.erase(_shots.begin(), _shots.end());
}

void Game::print(LedControl (&leds)[2]) {
  if (_p1.getStatus() == PlayerStatus::Alive) _p1.print(leds[0]);
  if (_p2.getStatus() == PlayerStatus::Alive) _p2.print(leds[1]);
  for (const auto & s : _shots) {
    s.print(leds[s.getActualLed()]);
  }
}

void Game::movePlayer(PlayerNumber player, int byteRec, const Sound & s) {
  bool isShot = false;
  Player & p = player == PlayerNumber::P1 ? _p1 : _p2;
  if (p.getStatus() == PlayerStatus::Alive) {
    isShot = p.move(byteRec);
    if (isShot) this->addShot(p, s);
  }
}

void Game::movePlayers(int byteRec, const Sound & s) {    
  bool isShot = false;
  if (_p1.getStatus() == PlayerStatus::Alive) {
    isShot = _p1.move(byteRec);
    if (isShot) this->addShot(_p1, s);
  }

  if (_p2.getStatus() == PlayerStatus::Alive) {
    isShot = _p2.move(byteRec);
    if (isShot) this->addShot(_p2, s);
  }
}

bool Game::detectCollisions() {
  bool collisionDetected = false;
  for (const auto & s : _shots) {
    int ledShot = s.getActualLed();
    if (s.getStatus() == ShotStatus::Active && static_cast<int>(s.getFiredBy()) != ledShot) {
      Player & p = ledShot == 0 ? _p1 : _p2;
      Player & pOther = ledShot == 0 ? _p2 : _p1;
      if (p.getStatus() == PlayerStatus::Alive && s.getColumn() == p.getColumn()) {
        int shotRow = s.getRow(); int playerRow = p.getRow();
        if (shotRow == playerRow || shotRow == playerRow + 1 || shotRow == playerRow - 1) {
          p.died();
          pOther.incScore();
          collisionDetected = true;
        }
      }

      if (p.getScore() == 5 || pOther.getScore() == 5) {
        _status = GameStatus::Finished;
      }
    }
  }

  return collisionDetected;
}

void Game::removeInactiveShots() {
  _shots.erase(std::remove_if(_shots.begin(), _shots.end(), [](const Shot & s) {
    return s.getStatus() == ShotStatus::Inactive;
  }), _shots.end());
}

void Game::addShot(const Player & p, const Sound & s) {
  s.playShotFired();
  PlayerNumber pNumber = p.getPlayerNumber();
  _shots.push_back(Shot(pNumber, static_cast<int>(pNumber), p.getRow(), p.getColumn()));
}
