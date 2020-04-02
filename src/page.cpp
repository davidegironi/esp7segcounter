/*
ESP/ESC7SegCounter v01

copyright (c) Davide Gironi, 2017

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/

#include "page.h"

//initializer server
ESP8266WebServer server(80);

/**
 * Main page content
 * @return return the main page content
 */
String page() {
  String content = "";
  content += FPSTR(HTTP_PAGE);
  content += FPSTR(HTTP_HTMLSTART);
  content += FPSTR(HTTP_HEADSTART);
  content += FPSTR(HTTP_HEADMETA);
  content += FPSTR(HTTP_HEADTITLE);
  content += FPSTR(HTTP_HEADSTYLE);
  content += FPSTR(HTTP_HEADSCRIPT);
  content += FPSTR(HTTP_HEADEND);
  content += FPSTR(HTTP_BODYSTART);
  content += FPSTR(HTTP_CONTENTSTART);
  content += "{c}";
  content += FPSTR(HTTP_CONTENTEND);
  content += FPSTR(HTTP_BODYEND);
  content += FPSTR(HTTP_HTMLEND);
  return content;
}

/**
 * Emit the home page
 */
void pageHome() {
  //set page content
  String contentpage = FPSTR(HTTP_PAGEHOMESTATUS);

  //get main page content
  String content = page();
  content.replace("{c}", contentpage);
  if(ntpIsUpdated()) {
    String formattedTime = ntpGetDateTimeStr();
    content.replace("{t}", formattedTime);
  } else {
    content.replace("{t}", "/");
  }

  //write page
  server.send(200, "text/html", content);
}

/**
 * Emit the WiFi settings page
 */
void pageWifi() {
  String contentpage = "";
  int deviceid = 0;
  String deviceids = "";
  int ntptimezoneh = 0;
  String ntptimezonehs = "";
  int ntptimezonem = 0;
  String ntptimezonems = "";

  //get vars
  if (server.arg("s") != "") {
    if (server.arg("n") != "") {
      eepromConfig.deviceid = atoi(server.arg("n").c_str());
    }
    if (server.arg("h") != "") {
      eepromConfig.ntptimezoneh = atoi(server.arg("h").c_str());
    }
    if (server.arg("m") != "") {
      eepromConfig.ntptimezonem = atoi(server.arg("m").c_str());
    }
    if (server.arg("d") != "")
      eepromConfig.ntpdaylight = 1;
    else
      eepromConfig.ntpdaylight = 0;
    if (server.arg("r") == "1") {
      wifiManagerHelper.resetSettings();
    }
    eepromWrite();
  }

  //read vars
  eepromRead();
  deviceid = eepromConfig.deviceid;
  char deviceidc[4];
  snprintf(deviceidc, sizeof(deviceidc), "%03d", deviceid);
  ntptimezoneh = eepromConfig.ntptimezoneh;
  char ntptimezonehc[4];
  snprintf(ntptimezonehc, sizeof(ntptimezonehc), "%d", ntptimezoneh);
  ntptimezonem = eepromConfig.ntptimezonem;
  char ntptimezonemc[4];
  snprintf(ntptimezonemc, sizeof(ntptimezonemc), "%d", ntptimezonem);
  char ntpdaylightc[8];
  if(eepromConfig.ntpdaylight)
    snprintf(ntpdaylightc, sizeof(ntpdaylightc), "checked");
  else
    snprintf(ntpdaylightc, sizeof(ntpdaylightc), " ");

  //set page output
  if (server.arg("s") != "") {
    //save and reboot
    contentpage = FPSTR(HTTP_PAGEWIFISAVE);
    char hostname [WIFI_HOSTNAMELENGTH + 4];
    strcpy(hostname, WIFI_HOSTNAME);
    strcat(hostname, deviceidc);
    contentpage.replace("{l}", hostname);
  } else {
    contentpage = FPSTR(HTTP_PAGEWIFI);
    contentpage.replace("{n}", deviceidc);
    contentpage.replace("{h}", ntptimezonehc);
    contentpage.replace("{m}", ntptimezonemc);
    contentpage.replace("{d}", ntpdaylightc);
  }

  //get main page content
  String content = page();
  content.replace("{c}", contentpage);

  //write page
  server.send(200, "text/html", content);

  //reboot on save
  if (server.arg("s") != "") {
    delay(1000);
    ESP.restart();
    delay(1000);
  }
}

/**
 * Emit the Settings page
 */
void pageSettings() {
  String contentpage = "";
  String geturlhost = "";
  int geturlport = 80;
  String geturluri = "";

  //get vars
  if (server.arg("s") != "") {
    geturlhost = server.arg("uh");
    if(geturlhost.length() < sizeof(eepromConfig.geturlhost)/sizeof(eepromConfig.geturlhost[0]))
      strcpy(eepromConfig.geturlhost, geturlhost.c_str());
    eepromConfig.geturlport = atoi(server.arg("up").c_str());
    geturluri = server.arg("ur");
    if(geturluri.length() < sizeof(eepromConfig.geturluri)/sizeof(eepromConfig.geturluri[0]))
      strcpy(eepromConfig.geturluri, geturluri.c_str());
    eepromWrite();
  }

  //read vars
  eepromRead();
  geturlhost = String(eepromConfig.geturlhost);
  geturlport = eepromConfig.geturlport;
  char geturlportc[6];
  snprintf(geturlportc, sizeof(geturlportc), "%d", geturlport);
  geturluri = String(eepromConfig.geturluri);

  //set page content
  if (server.arg("s") != "") {
    contentpage = FPSTR(HTTP_PAGESETTINGSSAVE);
  } else {
    contentpage = FPSTR(HTTP_PAGESETTINGS);
    contentpage.replace("{uh}", geturlhost);
    contentpage.replace("{up}", geturlportc);
    contentpage.replace("{ur}", geturluri);
  }

  //get main page content
  String content = page();
  content.replace("{c}", contentpage);

  //write page
  server.send(200, "text/html", content);
}

/**
 * Emit the Page Not Found
 */
void pageNotFound() {
  //set page content
  String contentpage = FPSTR(HTTP_PAGENOTFOUND);

  //get main page content
  String content = page();
  content.replace("{c}", contentpage);

  //write page
  server.send(200, "text/html", content);
}

/**
 * initialized the web server
 */
void pageInit() {
  //initialize server
  server.on("/", pageHome);
  server.on("/wifi", pageWifi);
  server.on("/settings", pageSettings);
  server.onNotFound(pageNotFound);
  server.begin();
}

void pageHandleClient() {
  server.handleClient();
}
