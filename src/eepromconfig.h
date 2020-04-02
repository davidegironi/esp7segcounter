/*
ESP/ESC7SegCounter v01

copyright (c) Davide Gironi, 2017

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/

#ifndef eepromconfig_h
#define eepromconfig_h

#include <Arduino.h>

//EEPROM library
#include <EEPROM.h>
//include for limits
#include <spi_flash.h>

//EEPROMConfigStruct
struct EEPROMConfigStruct
{
  uint8_t initialized;
  int deviceid;
  int ntptimezoneh;
  int ntptimezonem;
  uint8_t ntpdaylight;
  char geturlhost[255];
  int geturlport;
  char geturluri[500];
};

//export functions
extern void eepromRead();
extern void eepromWrite();
extern void eepromInit();

#endif
