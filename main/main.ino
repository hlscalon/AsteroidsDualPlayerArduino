#include <LedControl.h>
#include <AltSoftSerial.h>

#include "Utils.hpp"
#include "Controls.hpp"
#include "Player.hpp"
#include "Shot.hpp"
#include "Game.hpp"
#include "Sound.hpp"

AltSoftSerial serialP1; // Rx = 8, Tx = 9

const int dataPin1 = 4, csPin1 = 5, clkPin1 = 6;
const int dataPin2 = 11, csPin2 = 12, clkPin2 = 13;
LedControl leds[2] = {
  LedControl(dataPin1, clkPin1, csPin1, 1), // left
  LedControl(dataPin2, clkPin2, csPin2, 1)  // right
};

Game game(
  Player(PlayerNumber::P1, 0, Controls('w', 's', 'a', 'd', 'x')), // left
  Player(PlayerNumber::P2, 7, Controls('8', '5', '4', '6', '0')) // right
);

const int soundPin = 2;
Sound sound(soundPin);

unsigned long timeGameRefresh, timeGameRemoveShots;
void setup() {
  Serial.begin(9600);
  serialP1.begin(38400);

  leds[0].shutdown(0, false); leds[0].setIntensity(0, 2);
  leds[1].shutdown(0, false); leds[1].setIntensity(0, 2);

  timeGameRefresh = timeGameRemoveShots = millis();
}

void loop() {
  Utils::clearScreen(leds[0]); Utils::clearScreen(leds[1]);
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
  if (serialP1.available() > 0) {
    byteRec = serialP1.read();
    game.movePlayer(PlayerNumber::P1, byteRec, sound);
  }

  if (Serial.available() > 0) {
    byteRec = Serial.read(); 
    game.movePlayers(byteRec, sound);
    game.movePlayer(PlayerNumber::P2, byteRec, sound);
  }

  if (game.detectCollisions()) {
    Utils::clearScreen(leds[0]); Utils::clearScreen(leds[1]);
    game.printScore(leds);
    sound.playTargetHitted();
    game.checkStatus();
    game.resetPositions();
    Utils::clearBufferSerial();
    Utils::clearBuffer(serialP1);
  }

}
