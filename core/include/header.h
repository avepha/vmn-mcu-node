#include <Arduino.h>
#include <Task.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#include "./modules/eeprom.h"

#include "./modules/VmnSensors/water.h"
#include "./modules/VmnSensors/ec.h"
#include "./modules/VmnSensors/LoadCell.h"
