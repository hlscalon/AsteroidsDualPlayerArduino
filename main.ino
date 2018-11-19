#include <LedControl.h>
#include <SoftwareSerial.h>

#include "Utils.hpp"
#include "Controls.hpp"
#include "Player.hpp"
#include "Shot.hpp"
#include "Game.hpp"
#include "Sound.hpp"

const int serialRxP1 = 9, serialTxP1 = 10;
SoftwareSerial serialP1(serialRxP1, serialTxP1);

const int serialRxP2 = 2, serialTxP2 = 3;
SoftwareSerial serialP2(serialRxP2, serialTxP2);

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

const int soundPin = 8;
Sound sound(soundPin);

unsigned long timeGameRefresh, timeGameRemoveShots;
void setup() {
  Serial.begin(9600);
  serialP1.begin(38400);
  serialP2.begin(38400);

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
  if (Serial.available() > 0) {
    byteRec = Serial.read(); 
    game.movePlayers(byteRec, sound);
  }

  /*if (serialP1.available() > 0) {
    byteRec = serialP1.read();
    game.movePlayers(byteRec, sound);
  }*/

  if (game.detectCollisions()) {
    Utils::clearScreen(leds[0]); Utils::clearScreen(leds[1]);
    game.printScore(leds);
    sound.playTargetHitted();
    game.resetPositions();
    Utils::clearBuffer(); // Utils::clearBuffer(serialP1);
  }

}
