/*
ESP/ESC7SegCounter v01

copyright (c) Davide Gironi, 2017

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/

#ifndef pages_h
#define pages_h

#include <Arduino.h>

//include main configuration
#include "main.h"

//WebServer library
#include <ESP8266WebServer.h>

//WiFiManager helper library
#include "WiFiManagerHelper/WiFiManagerHelper.h"
extern WiFiManagerHelper wifiManagerHelper;

//include eepromConfig
#include "eepromconfig.h"
extern struct EEPROMConfigStruct eepromConfig;

//NPT client library
#include <NtpClientLib.h>
//include ntphelper
#include "ntphelper.h"

//html page constants
const char HTTP_PAGE[] PROGMEM = "<!DOCTYPE html>";
const char HTTP_HTMLSTART[] PROGMEM = "<html lang=\"en\">";
const char HTTP_HTMLEND[] PROGMEM = "</html>";
const char HTTP_HEADSTART[] PROGMEM = "<head>";
const char HTTP_HEADEND[] PROGMEM = "</head>";
const char HTTP_HEADMETA[] PROGMEM = "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
const char HTTP_HEADTITLE[] PROGMEM = "<title>ESP/ESC7SegCounter</title>";
const char HTTP_HEADSTYLE[] PROGMEM = "<style>body{text-align:center;font-family:verdana;font-size:80%}div.b{text-align:left;display:inline-block;min-width:260px;max-width:650px;width:100%;margin:20px 0 0}h2{text-align:center}div.m{background-color:#000;padding:10px;color:#fff}div.m a{color:#fff;text-decoration:none}p{margin:0 0 10px;padding:0}input,label{display:block}input[type=text]{width:100%;box-sizing:border-box;webkit-box-sizing:border-box;-moz-box-sizing:border-box}textarea{width:100%;box-sizing:border-box;webkit-box-sizing:border-box;-moz-box-sizing:border-box}</style>";
const char HTTP_HEADSCRIPT[] PROGMEM = "<script></script>";
const char HTTP_BODYSTART[] PROGMEM = "<body>";
const char HTTP_BODYEND[] PROGMEM = "</body>";
const char HTTP_CONTENTSTART[] PROGMEM = "<div class='b'><h2>ESP/ESC7SegCounter</h2><div class='m'><a href='/'>Home</a> | <a href='wifi' class='w'>WiFi</a> | <a href='settings' class='s'>Settings</a></div><div class='p'>";
const char HTTP_CONTENTEND[] PROGMEM = "</div></div>";
const char HTTP_PAGENOTFOUND[] PROGMEM = "<h2>Page not found!</h2>";
const char HTTP_PAGEHOMESTATUS[] PROGMEM = "<h2>Status</h2>Time: <b>{t}</b><br/><br/>";
const char HTTP_PAGEWIFI[] PROGMEM = "<h2>WiFi Configuration</h2><script>function validate(e){var n='';if(isNaN(e.elements.n.value)||!(parseInt(e.elements.n.value)>=0&&parseInt(e.elements.n.value)<=999)) n+='Invalid Device ID, must be a number between 0 and 999. ';if(isNaN(e.elements.h.value)||!(parseInt(e.elements.h.value)>=-12&&parseInt(e.elements.h.value)<=14)) h+='Invalid NTP Timezone hours offset, must be a number between -12 and 14. ';if(isNaN(e.elements.m.value)||!(parseInt(e.elements.m.value)>=-59&&parseInt(e.elements.m.value)<=59)) m+='Invalid NTP Timezone minutes offset, must be a number between -59 and 59. ';if(n !=''){alert(n); return false;}return true;}</script><form method='post' onsubmit='return validate(this);' action='wifi'><p><label>Device ID</label><input type='text' id='n' name='n' length=3 value='{n}'/></p><p><label>Reset WiFi settings</label><input type='checkbox' id='r' name='r' value='1'/></p><p><label>NTP Timezone hours offset</label><input type='text' id='h' name='h' length=3 value='{h}'/></p><p><label>NTP Timezone minutes offset</label><input type='text' id='m' name='m' length=3 value='{m}'/></p><p><label>NTP Daylight</label><input type='checkbox' id='d' name='d' value='1'{d}/></p><br/><input type='hidden' id='s' name='s' value='1'/><button type='submit'>save</button></form>";
const char HTTP_PAGEWIFISAVE[] PROGMEM = "<h2>WiFi Configuration</h2>Settings has been saved.<br/>Device will be rebooted in 10s.<br/><script>setTimeout(function(){ window.location.replace('http://{l}'); }, 10000);</script>";
const char HTTP_PAGESETTINGS[] PROGMEM = "<h2>Settings</h2><script>function validate(e){var n=''; if(e.elements.uh.value.length > 255) n+='Invalid GET url hostname, max 200 characters. '; if(!(parseInt(e.elements.up.value)>=0&&parseInt(e.elements.up.value)<=65535)) n+='Invalid GET url port, must be a number between 0 and 65535. '; if(e.elements.ur.value.length > 500) n+='Invalid GET url uri, max 500 characters. '; if(n !=''){alert(n); return false;}return true;}</script><form method='post' onsubmit='return validate(this);' action='settings'><p><label>GET url host</label><input type='text' id='uh' name='uh' length=255 value='{uh}'/></p><p><label>GET url port</label><input type='text' id='up' name='up' length=255 value='{up}'/></p><p><label>GET url uri</label><input type='text' id='ur' name='ur' length=500 value='{ur}'/></p><br/><input type='hidden' id='s' name='s' value='1'/><button type='submit'>save</button></form>";
const char HTTP_PAGESETTINGSSAVE[] PROGMEM = "<h2>Settings</h2>Settings has been saved.<br/>Page will be reloaded in 2s.<br/><script>setTimeout(function(){ window.location=window.location.origin+window.location.pathname; }, 2000);</script>";

//export functions
extern void pageInit();
extern void pageHandleClient();

#endif
