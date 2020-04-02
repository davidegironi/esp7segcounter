/*
ESP/ESC7SegCounter v01

copyright (c) Davide Gironi, 2018

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/

#include <Arduino.h>

//include main
#include "main.h"

//WiFiManager helper library
#include "WiFiManagerHelper/WiFiManagerHelper.h"
//initialize WiFiManagerHelper
WiFiManagerHelper wifiManagerHelper;

//LedControl library
#include "LedControl.h"
//initialize LedControl
LedControl ledControl = LedControl(LED7SEG_DATAPIN, LED7SEG_CLKPIN, LED7SEG_CSPIN, LED7SEG_NUMDEV);

//include pages
#include "page.h"

//include eepromConfig
#include "eepromconfig.h"
extern struct EEPROMConfigStruct eepromConfig;

//include ntphelper
#include "ntphelper.h"

//HTTP client library
#include <ESP8266HTTPClient.h>

//display runner
Ticker displayRunnerTicker;

//alive led ticker
uint8_t aliveLedStatus = 0;

//display mode
uint8_t displaynums = 0;

//check updated interval time
static unsigned long updategetLast = 0;

//numbers to display
static volatile uint32_t num[LED7SEG_NUMDEV];

//previous numbers to display
static uint32_t numprev[LED7SEG_NUMDEV];

//blinker for updated numbers to display
static uint32_t numblink[LED7SEG_NUMDEV];

//display mode
String stringdisp = "";


/*
 * Check if a string is a number
 */
uint8_t isNumeric(String str) {
  uint8_t i = 0;
  uint8_t canhasdot = 1;
  uint8_t canhasminus = 1;
  for(i=0; i<str.length(); i++) {
    if(isDigit(str.charAt(i)) ||
      (str.charAt(i) == '.' && canhasdot == 1) ||
      (str.charAt(i) == '-' && canhasminus == 1)) {
      if(str.charAt(i) == '.')
        canhasdot = 0;
      if(str.charAt(i) == '-')
        canhasminus = 0;
    } else
      return 0;
  }
  return 1;
}

/**
 * Set a number to a 7segment display
 */
void setNumber(uint8_t displayid, uint32_t number, uint8_t dp) {
  for(uint8_t i=0; i<8; i++) {
    uint32_t n = number;
    //find the digit to display
    uint32_t exponent = 1;
    uint8_t exponentt = i;
    while (exponentt--) {
      exponent *= 10;
    }
    n = n / exponent;
    n = n % 10;
    if(i > 0 && n==0 && number < exponent) {
      n = 16;
    }
    //set digit to display, just the dp toggle if needed
    ledControl.setDigit(displayid, i, n, dp && i==0);
  }
}

/**
 * Display runner
 */
void displayRunner() {
  uint8_t i=0;
  
  if(displaynums) {
    //display numbers
    for(i=0; i<LED7SEG_NUMDEV; i++) {
      if(numblink[i] > 0) {
        numblink[i]--;
        if(numblink[i] % 2)
          setNumber(i, num[i], 0);
        else
          ledControl.clearDisplay(i);
      } else
        setNumber(i, num[i], aliveLedStatus);
    }
    aliveLedStatus = !aliveLedStatus;
  } else {
    for(uint8_t i=0; i<8; i++) {
      if(i < stringdisp.length()) {
        ledControl.setChar(0, i, stringdisp.charAt(stringdisp.length() - 1 - i), 0);
      } else {
        ledControl.setChar(0, i, ' ', 0);
      }
    }
  }  
}

/**
 * Set a string to be display
 */
void setDisplay(String disp) {
  uint8_t i = 0;
  if(disp == "")
    displaynums = 1;
  else {
    displaynums = 0;
    for(i=0; i<LED7SEG_NUMDEV; i++) {
      num[i] = 0;
      numprev[i] = 0;
      numblink[i] = 0;
    }
    for(i=0; i<LED7SEG_NUMDEV; i++) {
      ledControl.clearDisplay(i);
    }
  }
  stringdisp = disp;
}

/**
 * Get data and set display numbers
 */
void getData() {
  static uint8_t getDataErrorCounter = 0;

  WiFiClient client;
  HTTPClient http;
  //set the url uri
  String geturluri = eepromConfig.geturluri;
  String datestr = ntpGetDateStr();
  String timestr = ntpGetTimeStr();
  String day = datestr.substring(0, 2);
  String month = datestr.substring(3, 5);
  String year = datestr.substring(6, 10);
  String hour = timestr.substring(0, 2);
  String minute = timestr.substring(3, 4);
  String second = timestr.substring(6, 8);
  geturluri.replace("%DATE%", String(year) + "-" + String(month) + + "-" + String(day));
  geturluri.replace("%TIME%", String(hour) + "-" + String(minute) + + "-" + String(second));
  geturluri.replace("%DATETIME%", String(year) + "-" + String(month) + + "-" + String(day) + "T" + String(hour) + "-" + String(minute) + + "-" + String(second));
#if defined SERIAL_ENABLED
  Serial.println("Retrive data from " + String(eepromConfig.geturlhost) + ":" + eepromConfig.geturlport + String(geturluri));
#endif
  //get data
  http.begin(client, eepromConfig.geturlhost, eepromConfig.geturlport, geturluri);
  int httpCode = http.GET();
  if(httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    if(payload != "") {
#if defined SERIAL_ENABLED
      Serial.println("Payload below");
      Serial.println(payload);
#endif

      //sanitize characters
      payload.replace("\"", "");

      //get payload parts
      int maxIndex = payload.length() - 1;
      int index = 0;
      int indexnext;
      uint8_t haserror = 0;
      uint8_t currentdisplay = 0;
      do {
        uint32_t numt = 0;
        //sply payload using divisor
        indexnext = payload.indexOf(',', index);
        String payloadpart = payload.substring(index, indexnext);
        //check if payload part is a number
        if (isNumeric(payloadpart)) {
          numt = atol(payloadpart.c_str());
          if (numt > DISPLAY_MAX)
            numt = DISPLAY_MAX;
          else if (numt < 0)
            numt = 0;
          //set the display the payload part
#if defined SERIAL_ENABLED
          Serial.print("Payload part number: ");
          Serial.println(numt);
#endif
          if(currentdisplay < LED7SEG_NUMDEV) {
            num[currentdisplay] = numt;
            if(numprev[currentdisplay] != num[currentdisplay])
              numblink[currentdisplay] = 4;
            numprev[currentdisplay] = numt;
          }
          index = indexnext + 1;
          currentdisplay++;
        } else {
          haserror = 1;
#if defined SERIAL_ENABLED
          Serial.println("Payload is not a number.");
#endif
        }
      } while((indexnext != -1) && (indexnext < maxIndex) && (!haserror) && (currentdisplay < LED7SEG_NUMDEV));
      if(haserror) {
        //increment error counter
        getDataErrorCounter++;
      } else {
        //switch to number displays
        displaynums = 1;
        aliveLedStatus = 1;
        setDisplay("");
      }
    }
  } else {
    //increment error counter
    getDataErrorCounter++;
#if defined SERIAL_ENABLED
    Serial.print("Fail to get data. Error code: ");
    Serial.println(httpCode);
#endif
  }
  http.end();
  //check error getting data
  if(getDataErrorCounter > UPDATE_GETFAILRESET) {
    getDataErrorCounter = 0;
#if defined SERIAL_ENABLED
    Serial.println("Fail to get data.");
#endif
    setDisplay("500...");
  }
}

/**
 * Main setup
 */
void setup() {
  uint8_t i=0;

#if defined SERIAL_ENABLED
  //initialize Serial
  Serial.begin(115200);
  delay(500);
  Serial.println("Starting...");
#endif

  //initialize eeprom
#if defined SERIAL_ENABLED
  Serial.println("Loading eeprom");
#endif
  eepromInit();
  eepromRead();

  //initilize 7seg
  for(i=0; i<LED7SEG_NUMDEV; i++) {
    ledControl.shutdown(i, false);
    ledControl.setIntensity(i, 8);
    ledControl.clearDisplay(i);
  }
  
  //initialize numbers
  for(i=0; i<LED7SEG_NUMDEV; i++) {
    num[i] = 0;
    numprev[i] = 0;
    numblink[i] = 0;
  }

  //initialize the display runner
  displayRunnerTicker.attach(1, displayRunner);

  //set the string to display
  setDisplay("con...");

  //set WiFi
#if defined SERIAL_ENABLED
  Serial.println("Set the WiFi");
#endif
  char *apname = (char *)WIFI_CAPTIVEPORTALNAME;
  char deviceidc[4];
  snprintf(deviceidc, sizeof(deviceidc), "%03d", eepromConfig.deviceid);
  char hostname [WIFI_HOSTNAMELENGTH + 4];
  strcpy(hostname, WIFI_HOSTNAME);
  strcat(hostname, deviceidc);
  //set the connection manager
  wifiManagerHelper.setSerialDebug(WIFI_SERIALENABLED);
  wifiManagerHelper.setCheckConnection(WIFI_CHECKCONNECTIONINTERVAL, WIFI_CHECKCONNECTIONENABLED);
  wifiManagerHelper.setStatusLed(WIFI_STATUSLEDPIN, WIFI_STATUSLEDENABLED);
  wifiManagerHelper.setCaptivePortalOnButton(WIFI_CAPTIVEPORTALBUTTONPIN, WIFI_CAPTIVEPORTALBUTTONENABLED);
  //try to connect
  wifiManagerHelper.connect(hostname, apname, WIFI_SKIPIFNOTCONNECTEDTIME, WIFI_SKIPIFNOTCONNECTED);
#if defined SERIAL_ENABLED
  if(wifiManagerHelper.isConnected()) {
    Serial.println("Status: connected");
    Serial.print("    IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Status: not connected");
  }
#endif

  //disply the current ip
  if(wifiManagerHelper.isConnected()) {
    setDisplay(String(WiFi.localIP()[0]));
    delay(2000);
    setDisplay(String(WiFi.localIP()[1]));
    delay(2000);
    setDisplay(String(WiFi.localIP()[2]));
    delay(2000);
    setDisplay(String(WiFi.localIP()[3]));
    delay(2000);
  }

  //initialize webserver
#if defined SERIAL_ENABLED
  Serial.println("Starting webserver");
#endif
  pageInit();

  //initialize NTP
#if defined SERIAL_ENABLED
  Serial.println("Starting NTP client");
#endif
  ntpInit(eepromConfig.ntptimezoneh, eepromConfig.ntptimezonem, eepromConfig.ntpdaylight);

  //initialize last time update
  updategetLast = millis();
}

/**
 * Main loop
 */
void loop() {
  static uint8_t updateEnabled = 0;
  static int updateEnableFailCounter = 0;

  //check connection
  wifiManagerHelper.checkConnection();

  //update NTP
  ntpUpdate();

  //page server handler
  pageHandleClient();

  if(updateEnabled) {
    //check update
    if((millis() - updategetLast) > UPDATE_GETINTERVALMS) {
#if defined SERIAL_ENABLED
      Serial.println("Update");
#endif
      updategetLast = millis();

      //check connection
      if(wifiManagerHelper.isConnected() && ntpIsUpdated()) {
        //get text from url
        if(strcmp(eepromConfig.geturlhost, "") != 0) {
          getData();
        } else {
          setDisplay(".");
        }
      } else {
        updateEnableFailCounter++;
        if(updateEnableFailCounter == UPDATE_ENABLEFAILRESET) {
#if defined SERIAL_ENABLED
          Serial.println("Reset connection and NTP status");
#endif
          setDisplay("con...");
          updateEnableFailCounter = 0;
          updateEnabled = 0;
        }
      }
    }
  } else {
    //check the first connection and ntp status
    if(wifiManagerHelper.isConnected() && ntpIsUpdated()) {
#if defined SERIAL_ENABLED
      Serial.println("Connected and NTP updated");
#endif
      updateEnabled = 1;
      setDisplay(".");
      //perform update at next iteration
      updategetLast = millis() - UPDATE_GETINTERVALMS;
    }
  }
}
