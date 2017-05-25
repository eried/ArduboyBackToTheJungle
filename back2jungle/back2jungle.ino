#include <Arduboy2.h>
#include <ArduboyPlaytune.h>
#include <PGMWrap.h>

#include "EEPROMAnything.h"
#include "music.h"
#include "sprites.h"
#include "phrases.h"
#include "images.h"
#include "structs.h"

//#define DEBUG
//#define MIRRORING

//Initialize the arduboy object
Arduboy2 arduboy;
ArduboyPlaytune tunes(arduboy.audio.enabled);

const int INVALID = -32768, EEPROM_START = EEPROM_STORAGE_SPACE_START + 100;
const byte BYTE_MAX = 0xFF;

// ------ Game parameters -------
const char STARTLIVES = 3, STARTINGLEVEL = 0;
int FORWARD_BOOST = 12; // Use higher value to make the player jump further.
// -----------------------------

char lives = STARTLIVES;

unsigned long currentFrame = 0, currentLevelTime = 0;
byte currentLevel = STARTINGLEVEL, specialCar;
GameState gameState = GameState::Splash;
unsigned long wait = 0, debounce = 0;
unsigned int score = 0, highscore = 0;
bool specialCarBonus = true, fasterDialogs = false;
double playerx, playery;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);
  arduboy.clear();
  arduboy.initRandomSeed();

  initializeAudio();

#if defined(DEBUG) || defined(MIRRORING)
  Serial.begin(9600);
#endif

#ifdef DEBUG
  delay(1000);
  Serial.println("Debug enabled");
#endif

  // Load highscore
  highscore = getHiScore();
}

void initializeAudio()
{
  // audio setup
  tunes.initChannel(PIN_SPEAKER_1);
#ifndef AB_DEVKIT
  // if not a DevKit
  tunes.initChannel(PIN_SPEAKER_2);
#else
  // if it's a DevKit
  tunes.initChannel(PIN_SPEAKER_1); // use the same pin for both channels
  tunes.toneMutesScore(true);       // mute the score when a tone is sounding
#endif
}

