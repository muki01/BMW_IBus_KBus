// Basic_Code - prints every message on the I/K-Bus to the debug port
// (Arduino Nano / Uno)
//
// Requires the "BMW IBus KBus" library:
// https://github.com/muki01/BMW_IBus_KBus_Library

#include <BMW_IBus_KBus.h>
#include <SoftwareSerial.h>
SoftwareSerial debugSerial(7, 8);
BMW_IBus_KBus ibus;

uint8_t source, length, destination, databytes[36];

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
}

void packetHandler(byte *packet) {
  source = packet[0];
  length = packet[1];
  destination = packet[2];
  for (int i = 0, s = 3; i <= length - 3; i++, s++) {
    databytes[i] = packet[s];
  }
}

void startTimer() {
  ibus.startTimer();
}