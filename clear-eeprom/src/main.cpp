#include <Arduino.h>
#include <EEPROM.h>

void setup()
{
  for (int i = 0; i < EEPROM.length(); i++)
  {
    EEPROM.write(i, 0);
    EEPROM.commit();
  }
}

void loop()
{
}