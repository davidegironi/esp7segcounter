/*
ESP/ESC7SegCounter v01

copyright (c) Davide Gironi, 2017

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/

#include "ntphelper.h"

//check if ntp get time
uint8_t ntpHasTime = 0;

/**
 * Update the NTP
 */
void ntpUpdate() {
  static unsigned long nptupdate_last = 0;
	if ((millis() - nptupdate_last) > 60000) {
		nptupdate_last = millis();
    NTP.getTimeDateString();
  }
}

/**
 * Initialize NTP
 */
void ntpInit(int timezoneh, int timezonem, bool daylight) {
  NTP.onNTPSyncEvent([](NTPSyncEvent_t ntpEvent) {
    if (!ntpEvent) {
      ntpHasTime = 1;
    }
  });
  NTP.begin(NTPHELPER_POOLSERVER, timezoneh, daylight, timezonem);
  NTP.setInterval(1);
}

/**
 * Cheeck if NTP has time updated
 * @return 1 is true, 0 is false
 */
uint8_t ntpIsUpdated() {
  return ntpHasTime;
}

/**
 * Return date time formatted string
 * @return date time formatted
 */
String ntpGetDateTimeStr() {
  return ntpGetDateStr() + " " + ntpGetTimeStr();
}

/**
 * Return date formatted string
 * @return date formetted
 */
String ntpGetDateStr() {
  return NTP.getDateStr();
}

/**
 * Return time formatted string
 * @return time formetted
 */
String ntpGetTimeStr() {
  return NTP.getTimeStr().substring(0,5);
}
