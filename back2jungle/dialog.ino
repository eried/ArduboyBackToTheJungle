// Holds the game until B is pressed (or returns B state if is not modal)
bool waitForButton(bool isModal = true, bool useAlternativeButton = false, byte offset = 7)
{
  arduboy.drawBitmap(arduboy.width() - 8, arduboy.height() - offset, useAlternativeButton ? nextAlt : next, 8, 7);

#ifdef MIRRORING
  Serial.write(arduboy.getBuffer(), arduboy.width() * arduboy.height() / 8);
#endif
  arduboy.display();

  if (isModal)
  {
    while (arduboy.pressed(useAlternativeButton ? A_BUTTON : B_BUTTON)) {};
    while (arduboy.notPressed(useAlternativeButton ? A_BUTTON : B_BUTTON)) {};
    while (arduboy.pressed(useAlternativeButton ? A_BUTTON : B_BUTTON)) {};
    return true;
  }
  else
    return arduboy.pressed(useAlternativeButton ? A_BUTTON : B_BUTTON);
}

// Displays an animal image with an animated dialog and optionally the remaining lives
void animateDialog(const unsigned char animal[], const unsigned char str[], bool showLives = false)
{
  const int xposimage = 0;
  const int xpostext = 52;
  int ypos = 8;

  arduboy.clear();

  if (showLives)
  {
    ypos = 0;

    // Show lives
    for (byte l = 0; l < lives; l++)
      arduboy.drawBitmap(2 + (l * 16), arduboy.height() - 11, life, 15, 11);
  }

  // Draw marquee
  arduboy.drawRect(xposimage, ypos + 1, 50, 50);

  // Animal
  arduboy.drawBitmap(xposimage + 1, ypos + 2, animal, 48, 48);

  bool doDelays = true;
  int pos = ypos;
  arduboy.setCursor(xpostext, pos);

  for (byte i = 0; i < BYTE_MAX; i++)
  {
    char c = pgm_read_byte(str++);

    switch (c)
    {
      case '\0':
        i = 254; // Exit
        break;

      case '_':
        pos += 9;
        arduboy.setCursor(xpostext, pos );
        break;

      case '%':
        arduboy.display();
        arduboy.delayShort(doDelays ? 200 : 1);
        break;

      case ' ':
        arduboy.print(c);
        break;

      case '$':
        arduboy.print(highscore);
        arduboy.display();
        doWinSoundEffect();
        break;

      case '&':
        arduboy.print(score);

        if (score > 0)
        {
          arduboy.display();
          doWinSoundEffect();
        }
        break;

      case '|':
        // Nothing
        break;

      case '^':
        waitForButton();
        break;


      default:
        arduboy.print(c);
        tunes.tone(900, doDelays ? 20 : ((fasterDialogs && gameState != GameState::MainMenu) ? 1 : 10));
        arduboy.display();
        arduboy.delayShort(doDelays ? 100 : ((fasterDialogs && gameState != GameState::MainMenu) ? 1 : 12));
        break;
    }

    if (arduboy.pressed(B_BUTTON))
      doDelays = false;
  }

#ifdef MIRRORING
  Serial.write(arduboy.getBuffer(), arduboy.width() * arduboy.height() / 8);
#endif

  arduboy.display();
  arduboy.delayShort(doDelays ? 1500 : ((fasterDialogs && gameState != GameState::MainMenu) ? 10 : 300));

  while (arduboy.pressed(B_BUTTON));
}

// Resets the device
// From: https://forum.arduino.cc/index.php?topic=49581.0
void softReset()
{
  asm volatile ("  jmp 0");
}

// Plays and animates the win chime
void doWinSoundEffect()
{
  tunes.stopScore();
  arduboy.invert(true);
  tunes.stopScore();
  tunes.playScore(win);
  arduboy.delayShort(1700);
  arduboy.invert(false);
}

// Handles the end of a level, display a small talk in between levels
void doWinDialog()
{
#ifdef DEBUG
  Serial.println("LEVEL WIN");
  Serial.print("currentLevel=");
  Serial.println(currentLevel);
#endif

  doWinSoundEffect();

  // Update current score
  long elapsedTime = millis() - currentLevelTime;
  score += (currentLevel + 1) * 100 + lives * 25 + (60 - min(elapsedTime / 1000, 60)); // Current level, lives and elapsed time (up to a minute) defines the score

  // Save HI-SCORE
  if (score > highscore)
  {
    highscore = score;
    setHiScore(score);
  }

  switch (currentLevel++)
  {
    case 0:
      animateDialog(snakebig, playing1);
      animateDialog(monkeybig, playing2);
      break;

    case 1:
      animateDialog(snakebig, playing1);
      animateDialog(monkeybig, playing3);
      animateDialog(monkeybig, playing4);
      break;

    case 2:
      animateDialog(monkeybig, playing41);
      animateDialog(snakebig, intro7);
      break;

    case 3:
      animateDialog(monkeybig, playing5);
      break;

    case 4:
      animateDialog(giraffebig, playing6);
      break;

    case 5:
      animateDialog(giraffebig, playing7);
      animateDialog(monkeybig, playing8);
      break;

    case 6:
      animateDialog(monkeybig, playing9);
      animateDialog(monkeybig, playing91);
      animateDialog(giraffebig, playing92);
      bool blinking = 0;
      wait = millis() + 2000;

      arduboy.clear();
      arduboy.drawCompressed(0, 0, ending);

      do
      {
        // Animate glowing
        arduboy.drawBitmap(55, 34, ending_effect, 23, 15, blinking);
        blinking = !blinking;
        arduboy.delayShort(30);
        arduboy.display();

      } while (millis() < wait || !waitForButton(false));

      while (!arduboy.notPressed(A_BUTTON + B_BUTTON)); // Wait for button release

      animateDialog(monkeybig, playing93);
      animateDialog(giraffebig, playing94);
      animateDialog(monkeybig, playing95);
      animateDialog(giraffebig, playing96);
      animateDialog(snakebig, playing1);
      animateDialog(giraffebig, playing961);

      // Ending animation
      waitForButton();
      tunes.playScore(travelling);
      arduboy.clear();
      arduboy.display();

      for (byte i = 0; i < 8; i++)
      {
        arduboy.delayShort(150);
        arduboy.invert(true);
        arduboy.delayShort(150);
        arduboy.invert(false);
      }

      animateDialog(snakebig, playing97);
      softReset();
      break;
  }
  waitForButton();
}
