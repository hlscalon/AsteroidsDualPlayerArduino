#ifndef SOUND_HPP
#define SOUND_HPP

struct Sound {

  Sound(int pin) : _pin(pin) {}

  void play(int notes[], int dur[], int numnotes);

  void playShotFired();

  void playTargetHitted();

private:
  int _pin;
};

#endif // SOUND_HPP
