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

  switch (r)
  {
    // About 3% chance each
    case 7: animateDialog(monkey2, tip8); break;
    case 2: animateDialog(monkey2, tip7); break;
    case 4: animateDialog(monkey2, tip9); break;
    case 5: animateDialog(monkey2, tip6); break;
    case 6: animateDialog(monkey2, tip5); break;

    default:

      if (r > 20) // 33% chance
        animateDialog(monkey2, tip1);
      else if (r > 14) //
        animateDialog(monkey2, tip2);
      else if (r > 9)
        animateDialog(monkey2, tip4);
      else
        animateDialog(monkey2, tip3);
  }
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
    arduboy.setCursor(4, i);
    arduboy.print("Innovation Garage AS");
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
