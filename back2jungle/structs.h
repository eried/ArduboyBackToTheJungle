const byte CAR = 0, TRUCK = 1, BIKE = 2, SLIMBIKE = 3;

enum class GameState : unsigned char
{
  None,
  Playing,
  Splash,
  SkipIntro,
  StartLevelIntro,
  StartLevel,
  Lose,
  Win,
  End,
  GameOver,
  Intro,
  MainMenu,
};

struct enemySpecs {
  const byte width;
  const byte height;
  const unsigned char* bitmap;
  const unsigned char* mask;
};

struct playerSpecs {
  const byte width;
  const byte height;
  const unsigned char* bitmap;
  const unsigned char* mask;
  const double vx;
  const double vy;
};

const playerSpecs playerTypes[] = {
  {14, 8, snake, snakemask, 0.36, 0.30}, /* Snake */
  {14, 8, snake, snakemask, 0.31, 0.25}, /* Snake slower */
  {11, 14, penguin, penguinmask, 0.13, 0.2}, /* Penguin */
  {16, 11, monkey, monkeymask, 0.14, 0.15}, /* Monkey */
  {14, 14, lion, lionmask, 0.19, 0.13}, /* Lion */
  {12, 17, giraffe, giraffemask, 0.15, 0.12}, /* Giraffe */
  {18, 14, elephant, elephantmask, 0.12, 0.11}, /* Elephant */
};

const enemySpecs enemyTypes[] = {
  {13, 27, car, carmask}, /* CAR */
  {15, 32, truck, truckmask}, /* TRUCK */
  {8, 20, bike, bikemask}, /* BIKE */
  {5, 18, slimbike, slimbikemask},
};

const int level1enemies = 2,
          level2enemies = 5,
          level3enemies = 7,
          level4enemies = 8,
          level5enemies = 10,
          level6enemies = 11,
          level7enemies = 14;

const int enemiesMax = 14;
double enemiesPos[enemiesMax];

uint8_p enemiesOffset[] PROGMEM = {0, level1enemies,
                                   level1enemies + level2enemies, /* level 2 */
                                   level1enemies + level2enemies + level3enemies,
                                   level1enemies + level2enemies + level3enemies + level4enemies,
                                   level1enemies + level2enemies + level3enemies + level4enemies + level5enemies,
                                   level1enemies + level2enemies + level3enemies + level4enemies + level5enemies + level6enemies,
                                   level1enemies + level2enemies + level3enemies + level4enemies + level5enemies + level6enemies + level7enemies,
                                  };

struct enemy
{
  uint8_p type;
  uint8_p x;
  uint16_p wait;
  double_p vy;
};

const enemy enemies[] PROGMEM =
{
  // Level 1
  { CAR, 85, 0, -0.1 },
  { CAR, 102, 60, -0.15 },

  // Level 2
  { BIKE, 83, 0, -0.26 },
  { BIKE, 90, 100, -0.26 },
  { BIKE, 84, 230, -0.26 },
  { CAR, 102, 0, -0.35 },
  { CAR, 102, 100, -0.35 },

  // Level 3
  { BIKE, 83, 0, -0.28 },
  { BIKE, 90, 80, -0.28 },
  { BIKE, 84, 160, -0.28 },
  { CAR, 102, 0, -0.35 },
  { CAR, 103, 120, -0.35 },
  { TRUCK, 41, 100, -0.35 },
  { TRUCK, 22, 200, -0.31 },

  // Level 4
  { SLIMBIKE, 11, 300, -0.04},
  { BIKE, 83, 0, -0.3 },
  { BIKE, 90, 200, -0.3 },
  { CAR, 103, 0, -0.15 },
  { CAR, 103, 220, -0.15 },
  { CAR, 22, 0, -0.3 },
  { CAR, 23, 120, -0.3 },
  { TRUCK, 41, 100, -0.27 },

  // Level 5
  { SLIMBIKE, 13, 200, -0.02},
  { SLIMBIKE, 7, 300, -0.02},
  { SLIMBIKE, 73, 500, -0.06},
  { BIKE, 107, 0, -0.3 },
  { BIKE, 102, 200, -0.3 },
  { CAR, 83, 0, -0.15 },
  { BIKE, 84, 220, -0.15 },
  { TRUCK, 22, 0, -0.3 },
  { CAR, 23, 130, -0.3 },
  { TRUCK, 41, 100, -0.4 },

  // Level 6
  { SLIMBIKE, 13, 200, -0.03},
  { SLIMBIKE, 6, 10, -0.01},
  { SLIMBIKE, 73, 500, -0.03},
  { TRUCK, 102, 0, -0.27 },
  { BIKE, 110, 180, -0.27 },
  { BIKE, 102, 130, -0.27 },
  { BIKE, 83, 0, -0.15 },
  { BIKE, 84, 220, -0.15 },
  { CAR, 22, 0, -0.3 },
  { CAR, 23, 130, -0.3 },
  { TRUCK, 41, 100, -0.27 },

  // Level 7
  { SLIMBIKE, 13, 700, -0.02},
  { SLIMBIKE, 6, 100, -0.01},
  { SLIMBIKE, 73, 0, -0.02},
  { SLIMBIKE, 62, 500, -0.03},
  { CAR, 102, 0, -0.27 },
  { BIKE, 49, 140, -0.37 },
  { BIKE, 49, 240, -0.37 },
  { BIKE, 49, 340, -0.37 },
  { TRUCK, 102, 130, -0.27 },
  { TRUCK, 83, 0, -0.15 },
  { TRUCK, 84, 230, -0.15 },
  { CAR, 24, 0, -0.3 },
  { BIKE, 23, 130, -0.3 },
  { BIKE, 40, 100, -0.27 },
};
