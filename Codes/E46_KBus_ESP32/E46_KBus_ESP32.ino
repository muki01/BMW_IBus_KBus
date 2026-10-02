// E46_KBus_ESP32 - BMW E46 K-Bus controller with a web interface (ESP32)
//
// Does everything the Arduino version (E46_KBus_Code) does - welcome lights,
// goodbye lights and follow-me-home from the remote key - and adds a web
// interface to control the car from your phone.
//
// The ESP32 goes to deep sleep when the bus has been silent for a while and
// wakes up again as soon as there is traffic on the bus. While it is awake it
// runs a Wi-Fi access point: connect to it and open http://192.168.4.1
//
// Requires the "BMW IBus KBus" library:
// https://github.com/muki01/BMW_IBus_KBus_Library
//
// Set your Wi-Fi password and pins in Config.h before uploading.

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <driver/gpio.h>
#include <driver/rtc_io.h>
#include <esp_sleep.h>
#include <BMW_IBus_KBus.h>
#include "Config.h"
#include "Commands.h"
#include "WebPage.h"

static_assert(sizeof(WIFI_PASSWORD) - 1 >= 8, "Set your own WIFI_PASSWORD in Config.h (8 characters or more)");

BMW_IBus_KBus ibus;
WebServer server(80);
Preferences preferences;

byte source, length, destination, databytes[36];

// Key fob light functions (same logic as the Arduino version)
static bool carLocked = false;
static bool goodByeLightsOn = false;
int top_Btn_Presed = 0;
int mid_Btn_Presed = 0;
static unsigned long goodByeLightsTime = 0;
static unsigned long lastTimeTopBtn = 0;
static unsigned long lastTimeMidBtn = 0;

// Settings, stored in flash and changed in the web interface
unsigned int sleepAfter = DEFAULT_SLEEP_AFTER;
unsigned int webAwake = DEFAULT_WEB_AWAKE;
bool keyFobLights = true;

// Sleep timers
unsigned long lastBusTime = 0;
unsigned long lastWebTime = 0;
bool webUsed = false;

// Last messages for the bus monitor in the web interface
const byte LOG_SIZE = 20;
byte logData[LOG_SIZE][40];
byte logLength[LOG_SIZE];
byte logHead = 0;
byte logCount = 0;

// Called on every level change of the SEN/STA pin
void IBUS_ISR_ATTR startTimer() {
  ibus.startTimer();
}

void setup() {
  gpio_hold_dis((gpio_num_t)ENABLE_PIN);  // release the pins that were held during deep sleep
  gpio_deep_sleep_hold_dis();
  rtc_gpio_deinit((gpio_num_t)SEN_STA_PIN);

  Serial.begin(115200);
  Serial.println(F("--BMW E46 K-Bus--"));

  ibus.setPins(SEN_STA_PIN, ENABLE_PIN, LED_PIN);
  ibus.setIbusSerial(Serial2);
  Serial2.setPins(BUS_RX_PIN, BUS_TX_PIN);
  ibus.setIbusDebug(Serial);
  ibus.setIbusPacketHandler(packetHandler);
  attachInterrupt(digitalPinToInterrupt(SEN_STA_PIN), startTimer, CHANGE);

  loadSettings();
  lastBusTime = millis();
  if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_EXT0) {
    touchWeb();  // powered on by hand: stay awake long enough to connect a phone
  }

  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/commands", HTTP_GET, handleCommands);
  server.on("/api/state", HTTP_GET, handleState);
  server.on("/api/cmd", HTTP_POST, handleCommand);
  server.on("/api/settings", HTTP_POST, handleSettings);
  server.on("/api/awake", HTTP_POST, handleAwake);
  server.on("/api/sleep", HTTP_POST, handleSleep);
  server.begin();
}

void loop() {
  ibus.run();
  server.handleClient();

  if (goodByeLightsOn == true && millis() - goodByeLightsTime > 2000) {
    goodByeLightsOn = false;
    goodByeLightsTime = millis();
    ibus.write(TurnOffLights, sizeof(TurnOffLights));
  }

  if (secondsUntilSleep() == 0) {
    goToSleep();
  }
}

void packetHandler(byte *packet) {
  source = packet[0];
  length = packet[1];
  destination = packet[2];
  for (int i = 0, s = 3; i <= length - 3; i++, s++) {
    databytes[i] = packet[s];
  }

  lastBusTime = millis();
  logPacket(packet);

  if (keyFobLights == false) {
    return;
  }

  if ((source == 0x00) && (destination == 0xBF) && (databytes[0] == 0x72) && (databytes[1] == 0x22)) {
    carLocked = false;

    if (millis() - lastTimeTopBtn >= 14500) {
      top_Btn_Presed = 0;
    }

    if (top_Btn_Presed == 0) {
      ibus.write(ParkLights_And_Signals, sizeof(ParkLights_And_Signals));
      top_Btn_Presed = 1;
      lastTimeTopBtn = millis();
    } else {
      ibus.write(ParkLights_And_Signals_And_FogLights, sizeof(ParkLights_And_Signals_And_FogLights));
      top_Btn_Presed = 0;
      lastTimeTopBtn = millis();
    }
  }

  if ((source == 0x00) && (destination == 0xBF) && (databytes[0] == 0x72) && (databytes[1] == 0x12)) {
    top_Btn_Presed = 0;

    if (carLocked == false) {
      goodByeLightsOn = true;
      goodByeLightsTime = millis();
      carLocked = true;
      ibus.write(GoodbyeLights, sizeof(GoodbyeLights));
    } else if (carLocked) {
      mid_Btn_Presed++;

      if (mid_Btn_Presed == 2 && millis() - lastTimeMidBtn <= 4000) {
        ibus.write(FollowMeHome, sizeof(FollowMeHome));
        mid_Btn_Presed = 0;
      } else if (millis() - lastTimeMidBtn > 4000) {
        mid_Btn_Presed = 1;
      }

      lastTimeMidBtn = millis();
    }
  }
}

// -----------------------------------------------------------------------------
// Sleep

unsigned long secondsLeft(unsigned long elapsed, unsigned int limitSeconds) {
  unsigned long limit = limitSeconds * 1000UL;
  if (elapsed >= limit) {
    return 0;
  }
  return (limit - elapsed + 999) / 1000;
}

// The ESP32 stays awake while there is bus traffic or somebody uses the web interface
unsigned long secondsUntilSleep() {
  unsigned long busLeft = secondsLeft(millis() - lastBusTime, sleepAfter);
  unsigned long webLeft = webUsed ? secondsLeft(millis() - lastWebTime, webAwake) : 0;
  return max(busLeft, webLeft);
}

void touchWeb() {
  lastWebTime = millis();
  webUsed = true;
}

void goToSleep() {
  Serial.println(F("Going to sleep"));
  Serial.flush();

  server.stop();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_OFF);
  detachInterrupt(digitalPinToInterrupt(SEN_STA_PIN));

  digitalWrite(LED_PIN, LOW);
  digitalWrite(ENABLE_PIN, LOW);         // Shutdown TH3122, like the Arduino version
  gpio_hold_en((gpio_num_t)ENABLE_PIN);  // keep EN low while the ESP32 sleeps
  gpio_deep_sleep_hold_en();

  rtc_gpio_pullup_dis((gpio_num_t)SEN_STA_PIN);
  rtc_gpio_pulldown_en((gpio_num_t)SEN_STA_PIN);
  esp_sleep_enable_ext0_wakeup((gpio_num_t)SEN_STA_PIN, 1);  // wake up when the bus becomes active
  esp_deep_sleep_start();
}

// -----------------------------------------------------------------------------
// Settings

void loadSettings() {
  preferences.begin("kbus", true);
  sleepAfter = preferences.getUInt("sleepAfter", DEFAULT_SLEEP_AFTER);
  webAwake = preferences.getUInt("webAwake", DEFAULT_WEB_AWAKE);
  keyFobLights = preferences.getBool("keyFob", true);
  preferences.end();
}

void saveSettings() {
  preferences.begin("kbus", false);
  preferences.putUInt("sleepAfter", sleepAfter);
  preferences.putUInt("webAwake", webAwake);
  preferences.putBool("keyFob", keyFobLights);
  preferences.end();
}

// -----------------------------------------------------------------------------
// Bus monitor

void logPacket(byte *packet) {
  byte size = packet[1] + 2;  // source + length byte + the bytes that follow
  if (size > sizeof(logData[0])) {
    size = sizeof(logData[0]);
  }
  memcpy(logData[logHead], packet, size);
  logLength[logHead] = size;
  logHead = (logHead + 1) % LOG_SIZE;
  if (logCount < LOG_SIZE) {
    logCount++;
  }
}

// -----------------------------------------------------------------------------
// Web interface

void sendOk() {
  server.send(200, "application/json", "{\"ok\":true}");
}

String toHex(byte value) {
  String hex = String(value, HEX);
  hex.toUpperCase();
  return value < 0x10 ? "0" + hex : hex;
}

void handleRoot() {
  touchWeb();
  server.send_P(200, "text/html", WEB_PAGE);
}

void handleCommands() {
  String json = "[";
  for (byte i = 0; i < webCommandCount; i++) {
    if (i > 0) json += ",";
    json += "{\"id\":" + String(i) + ",\"group\":\"" + webCommands[i].group + "\",\"name\":\"" + webCommands[i].name + "\"}";
  }
  json += "]";
  server.send(200, "application/json", json);
}

void handleState() {
  String json = "{\"busIdle\":" + String((millis() - lastBusTime) / 1000);
  json += ",\"sleepIn\":" + String(secondsUntilSleep());
  json += ",\"sleepAfter\":" + String(sleepAfter);
  json += ",\"webAwake\":" + String(webAwake);
  json += ",\"keyFob\":" + String(keyFobLights ? "true" : "false");
  json += ",\"log\":[";
  for (byte n = 0; n < logCount; n++) {
    byte index = (logHead + LOG_SIZE - 1 - n) % LOG_SIZE;  // newest message first
    if (n > 0) json += ",";
    json += "\"";
    for (byte i = 0; i < logLength[index]; i++) {
      if (i > 0) json += " ";
      json += toHex(logData[index][i]);
    }
    json += "\"";
  }
  json += "]}";
  server.send(200, "application/json", json);
}

void handleCommand() {
  int id = server.arg("id").toInt();
  if (!server.hasArg("id") || id < 0 || id >= webCommandCount) {
    server.send(400, "application/json", "{\"ok\":false}");
    return;
  }
  touchWeb();
  ibus.write(webCommands[id].frame, webCommands[id].size);
  sendOk();
}

void handleSettings() {
  touchWeb();
  if (server.hasArg("sleepAfter")) {
    sleepAfter = constrain(server.arg("sleepAfter").toInt(), 10, 3600);
  }
  if (server.hasArg("webAwake")) {
    webAwake = constrain(server.arg("webAwake").toInt(), 30, 3600);
  }
  if (server.hasArg("keyFob")) {
    keyFobLights = server.arg("keyFob").toInt() != 0;
  }
  saveSettings();
  sendOk();
}

void handleAwake() {
  touchWeb();
  sendOk();
}

void handleSleep() {
  sendOk();
  delay(200);  // give the answer time to reach the browser
  goToSleep();
}
