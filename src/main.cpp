#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>

#include "constants.h"
#include "mode/mode.h"
#include "websocket.h"
#include "secrets.h"
#include "ota.h"
#include "webserver.h"
#include "screen.h"
#include "mode/mode.h"

#ifdef ENABLE_SERVER
WiFiMulti wifiMulti;
#endif

void setup()
{
  Serial.begin(115200);

  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

// server
#ifdef ENABLE_SERVER
  // wifi
  wifiMulti.addAP(WIFI_SSID_1, WIFI_PASSWORD_1);
  wifiMulti.addAP(WIFI_SSID_2, WIFI_PASSWORD_2);
  wifiMulti.addAP(WIFI_SSID_3, WIFI_PASSWORD_3);

  WiFi.setHostname(WIFI_HOSTNAME);

  int attempts = 0;
  while (wifiMulti.run() != WL_CONNECTED && attempts < 7)
  {
    delay(2000);
    Serial.print(".");
    attempts++;
  }
  if (wifiMulti.run() != WL_CONNECTED)
  {
    Serial.println("Couldn't connect to WiFi, resetting");
    ESP.restart();
  }
  else
  {
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
  }

  // set time server
  configTzTime(TZ_INFO, NTP_SERVER);

  initOTA(server);
  initWebsocketServer(server);
  initWebServer();
#endif

  Screen.setup();

  Screen.clear();
  loadMode();
  Screen.loadFromStorage();
}

void loop()
{
  loopOfAllModes();

#ifdef ENABLE_SERVER
  cleanUpClients();
#endif
  delay(20);
}
