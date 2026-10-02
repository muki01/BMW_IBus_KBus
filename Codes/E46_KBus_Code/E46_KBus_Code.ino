// E46_KBus_Code - welcome lights, goodbye lights and follow-me-home from the
// remote key of a BMW E46 (Arduino Nano / Uno)
//
// Requires the "BMW IBus KBus" library:
// https://github.com/muki01/BMW_IBus_KBus_Library

#include <BMW_IBus_KBus.h>
#include <SoftwareSerial.h>
#include "E46_Codes.h"
SoftwareSerial debugSerial(7, 8);
BMW_IBus_KBus ibus;

byte source, length, destination, databytes[36];

static bool carLocked = false;
static bool goodByeLightsOn = false;
int top_Btn_Presed = 0;
int mid_Btn_Presed = 0;
static unsigned long goodByeLightsTime = 0;
static unsigned long lastTimeTopBtn = 0;
static unsigned long lastTimeMidBtn = 0;

void setup() {
  ibus.setIbusSerial(Serial);
  debugSerial.begin(9600);
  ibus.setIbusDebug(debugSerial);
  debugSerial.println(F("--IBUS reader--"));
  ibus.setIbusPacketHandler(packetHandler);
  ibus.sleepEnable(60);
  attachInterrupt(digitalPinToInterrupt(3), startTimer, CHANGE);
}
void loop() {
  ibus.run();

  if (goodByeLightsOn == true && millis() - goodByeLightsTime > 2000) {
    goodByeLightsOn = false;
    goodByeLightsTime = millis();
    ibus.write(TurnOffLights, sizeof(TurnOffLights));
  }
}

void packetHandler(byte *packet) {
  source = packet[0];
  length = packet[1];
  destination = packet[2];
  for (int i = 0, s = 3; i <= length - 3; i++, s++) {
    databytes[i] = packet[s];
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

void startTimer() {
  ibus.startTimer();
}