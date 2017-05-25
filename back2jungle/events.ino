// Pause game
void doPause()
{
  tunes.stopScore();
  tunes.playScore(pause);
  arduboy.invert(true);
  arduboy.setCursor(47,28);
  arduboy.print("PAUSED");
  arduboy.display();
  arduboy.delayShort(300);
  waitForButton();
  arduboy.invert(false);
}

// Animal crashed with a car
void doCrash()
{
  tunes.stopScore();
  tunes.tone(1300, 400);

  arduboy.invert(true);
  arduboy.delayShort(500);
  tunes.tone(1290, 600);

  arduboy.invert(false);
  arduboy.delayShort(100);
  arduboy.invert(true);
  arduboy.delayShort(900);
  arduboy.invert(false);
}

// Intro dialog and animation
void doIntro()
{
  animateDialog(monkeybig, intro1);
  animateDialog(giraffebig, intro2);
  animateDialog(monkeybig, intro3);
  animateDialog(snakebig, intro4);
  animateDialog(monkeybig, intro5);
  animateDialog(monkeybig, intro51, true);
  animateDialog(giraffebig, intro6);
  animateDialog(snakebig, intro7);
  waitForButton();
}

// Plays a random monkey game tip
void doRandomTip()
{
  byte r = random(0, 30);
  unsigned char const * tip = tip3; // 5/30 chance
  if (r >= 21) // 9/30 chance
    tip = tip1;
  else if (r >= 15) // 5/30 chance
    tip = tip2;
  else if (r >= 10) //  5/30 chance
    tip = tip4;
  else
  switch (r)
  {
    // About 1/30 chance each
    case 2: tip = tip7; break;
    case 4: tip = tip9; break;
    case 5: tip = tip6; break;
    case 6: tip = tip5; break;
    case 7: tip = tip8; break;
  }
  animateDialog(monkey2, tip);
}

// Splash animation
void doSplash()
{
  arduboy.clear();
  arduboy.display();
  arduboy.delayShort(500);

  for (int i = -8; i < 26; i = i + 1)
  {
    arduboy.clear();
    arduboy.setCursor(13, i);
    arduboy.print("Innovation Garage");
    arduboy.display();
    arduboy.delayShort(20);
  }

  arduboy.delayShort(600);
  arduboy.setCursor(41, 35);
  arduboy.print("presents");
  arduboy.display();
  arduboy.delayShort(2000);
}

// Starts the menu music
void doIntroTheme()
{
  if (!tunes.playing())
    tunes.playScore(introtheme);
}
