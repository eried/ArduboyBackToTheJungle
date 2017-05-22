void loop()
{
  if (!arduboy.nextFrame())
    return;

  switch (gameState)
  {
    case SPLASH:
      doIntroTheme();
      doSplash();

    case MAINMENU:
      doIntroTheme();
      doMainMenu();
      break;

    case INTRO:
      specialCarBonus = false;
      score = 0;
      currentLevel = 0;
      lives = STARTLIVES;

      doIntro();
      gameState = STARTLEVEL;
      break;

    case END:
      if (!tunes.playing())
        tunes.playScore(gameovertheme);

      arduboy.clear();
      if (wait < millis())
      {
        // Show gameover text
        arduboy.setCursor(12, 25);
        arduboy.setTextSize(2);
        arduboy.print("GAME OVER");
        arduboy.setTextSize(1);
      }

      if (millis() > debounce)
      {
        // Show score
        arduboy.setCursor(0, arduboy.height() - 7);
        arduboy.print("Score: ");
        arduboy.print(score);

        arduboy.setCursor(0, 0);
        arduboy.print("Hi-score: ");
        arduboy.print(highscore);
        arduboy.display();

        waitForButton();
        gameState = MAINMENU;
        tunes.stopScore();
      }
      arduboy.display();

      break;

    case WIN:
      doWinDialog();
      gameState = STARTLEVEL;
      break;

    case GAMEOVER:
    case LOSE:
      if (--lives <= 0)
      {
        animateDialog(monkeybig, gameover2);
        arduboy.delayShort(1000);
        gameState = END;
        wait =  millis() + (arduboy.audio.enabled() ? 2000 : 0);
        debounce = wait + 2000;
      }
      else
      {
        animateDialog(monkeybig, gameover1, true);
        waitForButton();
        gameState = STARTLEVEL;
      }
      break;

    case STARTLEVEL:
      for (byte i = 0; i < enemiesMax; i++)
        enemiesPos[i] = arduboy.height();

      tunes.playScore(levelintro);
      playerx = INVALID;
      playery = INVALID;
      currentFrame = 0;
      specialCar = -1;

    case STARTLEVELINTRO:
      if (playerx != INVALID)
        playerx += 0.15;

      if (playerx > 3)
      {
        gameState = PLAYING;
        currentLevelTime = millis();
      }

    default: // Level
      if (!tunes.playing() && gameState == PLAYING)
        tunes.playScore(level);

      gameState = doGame(gameState == PLAYING);

#ifdef DEBUG
      Serial.print("PLAYING, NEXT STATE: ");
      Serial.println(gameState);
#endif
      break;
  }
}


