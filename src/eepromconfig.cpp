/*
ESP/ESC7SegCounter v01

copyright (c) Davide Gironi, 2017

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/

#include "eepromconfig.h"

//main eepromConfig
EEPROMConfigStruct eepromConfig;

/**
 * Read EEPROM configuration
 */
void eepromRead() {
  EEPROM.get(0, eepromConfig);
}

/**
 * Write EEPROM configuration
 */
void eepromWrite() {
  EEPROM.put(0, eepromConfig);
  EEPROM.commit();
}

/**
 * Initialize EEPROM configuration
 */
void eepromInit() {
  EEPROM.begin(SPI_FLASH_SEC_SIZE);
  delay(10);
  eepromRead();
  if(eepromConfig.initialized != 0x10) {
    eepromConfig.initialized = 0x10;
    eepromConfig.deviceid = 1;
    eepromConfig.ntptimezoneh = 0;
    eepromConfig.ntptimezonem = 0;
    eepromConfig.ntpdaylight = 0;
    strcpy(eepromConfig.geturlhost, "");
    eepromConfig.geturlport = 80;
    strcpy(eepromConfig.geturluri, "");
    eepromWrite();
  }
}
