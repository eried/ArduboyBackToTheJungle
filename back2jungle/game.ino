// Main game loop, use interactive false for the level intro
GameState doGame(bool interactive)
{
  GameState exitCode = !interactive ? GameState::StartLevelIntro : ( playerx > arduboy.width() - 3 ? GameState::Win : GameState::Playing);
  arduboy.clear();

  // Streets
  const int laneoffset = 61;
  arduboy.drawLine(20, 0, 20, arduboy.height()); // Lane 1
  arduboy.drawLine(20 + laneoffset, 0, 20 + laneoffset, arduboy.height()); // Lane 2

  const byte streetLine = 16;
  for (byte i = 0; i < arduboy.height(); i += streetLine)
  {
    arduboy.drawLine(39, i, 39, i + 7);  // Lane 1
    arduboy.drawLine(39 + laneoffset, i, 39 + laneoffset, i + 7); // Lane 2
  }

  arduboy.drawLine(58, 0, 58, arduboy.height()); // Lane 1
  arduboy.drawLine(58 + laneoffset, 0, 58 + laneoffset, arduboy.height()); // Lane 2

  // Player
  playerSpecs player = playerTypes[currentLevel];

  if (playerx == INVALID)
    playerx = -player.width;

  if (playery == INVALID)
    playery = (arduboy.height() - player.height) / 2;

  arduboy.drawBitmap(floor(playerx), floor(playery), player.mask, player.width, player.height, BLACK);
  arduboy.drawBitmap(floor(playerx), floor(playery), player.bitmap, player.width, player.height);

  // TODO: Make this dynamic
  const byte blockx = 34, blocky = 30, blockw = 11, blockh = 5;

  if (currentLevel < 2)
  {
    // Road blocks
    arduboy.drawBitmap(blockx, blocky, block, blockw, blockh); // Lane 1
  }

  if (interactive)
  {
    if (arduboy.pressed(B_BUTTON))
    {
      // Pause game
      doPause();
      return GameState::Playing;
    }

    int multiplier = 1;
    if ( millis() > debounce)
    {
      if (arduboy.pressed(A_BUTTON))
      {
        debounce = millis() + 500;
        multiplier = FORWARD_BOOST;
      }
    }
    else if (!arduboy.pressed(A_BUTTON))
      debounce -= 50; // Decrease the wait
    else
      debounce += 50; // Increase the wait

    double newx = playerx, newy = playery;
    if (arduboy.pressed(LEFT_BUTTON))    newx -= multiplier * player.vx;
    if (arduboy.pressed(RIGHT_BUTTON))   newx += multiplier * player.vx;
    if (arduboy.pressed(UP_BUTTON))      newy -= multiplier * player.vy;
    if (arduboy.pressed(DOWN_BUTTON))    newy += multiplier * player.vy;

    // Check if the new position is valid
    bool valid = false;

    if (newx >= 0 && newy >= -2 && (newy + player.height) <= arduboy.height() + 2)
    {
      if (currentLevel < 2)
        valid = !arduboy.collide({(byte)newx, (byte)newy, player.width, player.height},
      {blockx + 2, blocky + 2, blockw - 4, blockh - 4});
      else
        valid = true;
    }

    if (valid && (playerx != newx || playery != newy)) // Update
    {
      if (multiplier > 1)
        tunes.tone(1100, 30);
      else if (wait < millis())
      {
        tunes.tone(600, 6);
        wait = millis() + 200;
      }
      playerx = newx;
      playery = newy;
    }
  }

  // Border
  arduboy.drawBitmap(0, 0, fence, 4, 32);
  arduboy.drawBitmap(0, 32, fence, 4, 32);
  arduboy.drawBitmap(arduboy.width() - 5, 0, right_foliage, 6, 32);
  arduboy.drawBitmap(arduboy.width() - 5, 32, right_foliage, 6, 32);

  // Enemies
  byte enemy = 0;
  for (byte e = enemiesOffset[currentLevel]; e < enemiesOffset[currentLevel + 1]; e++)
  {
    if (currentFrame > enemies[e].wait)
    {
      enemySpecs specs = enemyTypes[enemies[e].type];
      enemiesPos[enemy] += enemies[e].vy;

      if (playerx + player.width >= enemies[e].x && playerx <= enemies[e].x + specs.width)
        arduboy.drawBitmap(enemies[e].x, floor(enemiesPos[enemy]), specialCar == e ? car2mask : specs.mask , specs.width, specs.height, BLACK);
      arduboy.drawBitmap(enemies[e].x, floor(enemiesPos[enemy]), specialCar == e ? car2 : specs.bitmap, specs.width, specs.height);

      // Loop
      if (enemiesPos[enemy] < -32)
      {
        enemiesPos[enemy] = arduboy.height();

        if (specialCar == e)
          specialCar = -1;
        else if (specialCar == -1 && enemies[e].type == CAR)
          if (random(0, 50) == 7)
          {
            if (specialCarBonus)
            {
              score += 500;
              specialCarBonus = false;
            }
            tunes.stopScore();
            tunes.playScore(car2music);
            specialCar = e;
          }
      }

      // Check collision
      if (arduboy.collide({(byte)playerx + 2, (byte)playery + 2, (byte)(player.width - 4), (byte)(player.height - 4)},
      {enemies[e].x + 2, (byte)(enemiesPos[enemy] + 2), (byte)(specs.width - 4), (byte)(specs.height - 4)}))
      {
        doCrash();
        exitCode = GameState::Lose;
      }
    }
    enemy++;
  }

  // Foliage
  arduboy.drawBitmap(63, -10, tree, 18, 18);
  arduboy.drawBitmap(57, 14, tree, 18, 18);
  arduboy.drawBitmap(65, 37, tree, 18, 18);
  arduboy.drawBitmap(56, 58, tree, 18, 18);

  currentFrame++;

#ifdef MIRRORING
  Serial.write(arduboy.getBuffer(), arduboy.width() * arduboy.height() / 8);
#endif

  arduboy.display();
  return exitCode;
}

// Shows and handles the main menu
void doMainMenu()
{
  bool exit = false;
  char option = 0;

  // Track menu movement pattern
  byte pos = 0;
  const byte max = 17;
  byte seq[] = {2, 1, 2, 2, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2, 2, 2};
  byte mov[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  do {
    doIntroTheme();
    arduboy.clear();
    arduboy.drawCompressed(14, 2, intro);
    arduboy.setCursor(24, 55);
    arduboy.print((char)0x11);

    switch (option)
    {
      case 0: arduboy.setCursor(33, 55); arduboy.print("Start game"); break;
      case 1: arduboy.setCursor(33, 55); arduboy.print("Sound: "); arduboy.print(arduboy.audio.enabled() ? " ON" : "OFF"); break;
      case 2: arduboy.setCursor(39, 55); arduboy.print("Hi-score"); break;
      case 3: arduboy.setCursor(43, 55); arduboy.print("Credits"); break;
      case 4: arduboy.setCursor(52, 55); arduboy.print("Help"); break;
    }
    arduboy.setCursor(arduboy.width() - 24 - 8, 55);
    arduboy.print((char)0x10);

#ifdef MIRRORING
    Serial.write(arduboy.getBuffer(), arduboy.width() * arduboy.height() / 8);
#endif

    arduboy.display();
    arduboy.pollButtons();

    if (arduboy.justPressed(B_BUTTON))
    {
      while (arduboy.pressed(B_BUTTON));

      switch (option)
      {
        case 0: {
            exit = true;
            bool ee = true;
            for (byte i = 0; i < max; i++)
              if (mov[pos++] != seq[i])
              {
                ee = false;
                break;
              }
              else
                pos %= max;

            if (ee)
            {
              FORWARD_BOOST += 500;
              doWinSoundEffect();
            }
          } break;
        case 1: tunes.stopScore(); arduboy.audio.toggle(); arduboy.audio.saveOnOff(); break;
        case 2: animateDialog(monkey2, (score == 0 && highscore == 0) ? hiscore0 : hiscore1); waitForButton(); break;
        case 3: animateDialog(qr, credits0); waitForButton(); animateDialog(qr, credits01); waitForButton(); animateDialog(qr, credits1); animateDialog(qr, credits2); waitForButton(); break;
        case 4: animateDialog(monkey2, tip0); doRandomTip(); waitForButton(); break;
      }
    }
    else
    {
      if (arduboy.justPressed(LEFT_BUTTON))
      {
        mov[pos++] = 1;
        option--;
        tunes.tone(1000, 30);
      }

      if (arduboy.justPressed(RIGHT_BUTTON))
      {
        mov[pos++] = 2;
        option++;
        tunes.tone(1000, 30);
      }

      pos %= max; // Bounded
      option = option < 0 ? 4 : (option > 4 ? 0 : option);
    }
  } while (!exit);

  tunes.stopScore();
  tunes.tone(987, 120);
  arduboy.delayShort(60);
  tunes.tone(1318, 400);
  gameState = GameState::Intro;
}
