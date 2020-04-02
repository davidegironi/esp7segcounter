/*
ESP/ESC7SegCounter v01

copyright (c) Davide Gironi, 2017

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/

#ifndef main_h
#define main_h

//set the serial output debug
#define SERIAL_ENABLED 1

//connection status led
#define WIFI_STATUSLEDPIN 5
//connection status led enabled
#define WIFI_STATUSLEDENABLED 1
//serial enabled
#define WIFI_SERIALENABLED SERIAL_ENABLED
//do not block on connection and captive portal
#define WIFI_SKIPIFNOTCONNECTED 0
//do not block on connection and captive portal timeout (seconds)
#define WIFI_SKIPIFNOTCONNECTEDTIME 180
//check connection at selected interval (milliseconds)
#define WIFI_CHECKCONNECTIONINTERVAL 10000
//check connection status enabled
#define WIFI_CHECKCONNECTIONENABLED 1
//captive portal by button pin
#define WIFI_CAPTIVEPORTALBUTTONPIN 4
//captive portal by button is enabled
#define WIFI_CAPTIVEPORTALBUTTONENABLED 0
//hozone
#define WIFI_HOSTNAME "esc7segcounter"
//hostname length
#define WIFI_HOSTNAMELENGTH 14
//captive portal name
#define WIFI_CAPTIVEPORTALNAME "ESC7SegCounter-AP"

//failed connection or ntp status reset
#define UPDATE_ENABLEFAILRESET 10
//failed get reset
#define UPDATE_GETFAILRESET 10
//get interval ms
#define UPDATE_GETINTERVALMS 30000

//max interval
#define DISPLAY_MAX 99999999

//set MAX7219 7segment data pin
#define LED7SEG_DATAPIN 13
//set MAX7219 7segment clk pin
#define LED7SEG_CLKPIN 14
//set MAX7219 7segment cs pin
#define LED7SEG_CSPIN 16
//set MAX7219 7segment number of display
#define LED7SEG_NUMDEV 2

#endif
