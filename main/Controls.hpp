#ifndef CONTROLS_HPP
#define CONTROLS_HPP

struct Controls {
  Controls(int chUp, int chDown, int chRight, int chLeft, int chFire) :
    _chUp(chUp), _chDown(chDown), _chRight(chRight), _chLeft(chLeft), _chFire(chFire) {}

  Controls() {}

  int getUp() { return _chUp; }
  int getDown() { return _chDown; }
  int getRight() { return _chRight; }
  int getLeft() { return _chLeft; }
  int getFire() { return _chFire; }

private:
  int _chUp; int _chDown; int _chRight; int _chLeft; int _chFire;
};

#endif // CONTROLS_HPP