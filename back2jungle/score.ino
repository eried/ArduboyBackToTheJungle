const byte CRC = 0xAE, OFFSET = 2;
const unsigned int CRC_C = 48000;

// Get the high score
unsigned int getHiScore()
{
  unsigned int score, crc;

  bool isValid = false;
  if (EEPROM.read(EEPROM_START) == CRC)
  {
    EEPROM_readAnything(EEPROM_START + OFFSET, score);
    EEPROM_readAnything(EEPROM_START + OFFSET + sizeof(score), crc);

    if (score + crc == CRC_C)
      isValid = true;
  }

  if (score > 500) // At least the user passed the second screen
    fasterDialogs = true;

  return isValid ? score : 0;
}

// Save the high score
void setHiScore(const unsigned int & score)
{
  EEPROM.write(EEPROM_START, CRC);
  EEPROM_writeAnything(EEPROM_START + OFFSET, score);
  EEPROM_writeAnything(EEPROM_START + OFFSET + sizeof(score), CRC_C - score);
}
