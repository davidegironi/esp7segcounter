/*
ESP/ESC7SegCounter v01

copyright (c) Davide Gironi, 2017

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/

#ifndef ntphelper_h
#define ntphelper_h

#include <Arduino.h>

//NPT client library
#include <NtpClientLib.h>

//set the NTP pool server
#define NTPHELPER_POOLSERVER "pool.ntp.org"

//export functions
extern void ntpUpdate();
extern void ntpInit(int timezoneh, int timezonem, bool daylight);
extern uint8_t ntpIsUpdated();
extern String ntpGetDateTimeStr();
extern String ntpGetDateStr();
extern String ntpGetTimeStr();

#endif
