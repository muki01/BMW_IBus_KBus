// -----------------------------------------------------------------------------
// Commands.h - the buttons shown in the web interface
//
// Every entry is one button: the group it belongs to, the text on the button
// and the frame from E46_Codes.h that is sent when it is pressed. Add, remove
// or reorder the lines to change the web interface.
// -----------------------------------------------------------------------------

#ifndef COMMANDS_H
#define COMMANDS_H

#include "E46_Codes.h"

struct WebCommand {
  const char *group;
  const char *name;
  const byte *frame;
  byte size;
};

#define WEB_COMMAND(group, name, frame) \
  { group, name, frame, sizeof(frame) }

const WebCommand webCommands[] = {
  WEB_COMMAND("Lights", "Parking lights + signals", ParkLights_And_Signals),
  WEB_COMMAND("Lights", "Parking + signals + fog", ParkLights_And_Signals_And_FogLights),
  WEB_COMMAND("Lights", "Low beams", Low_Beams),
  WEB_COMMAND("Lights", "Fog lights", Fog),
  WEB_COMMAND("Lights", "Hazard lights", HazardLights),
  WEB_COMMAND("Lights", "Hazard lights 3 s", Hazard_LCM_3s),
  WEB_COMMAND("Lights", "Goodbye lights", GoodbyeLights),
  WEB_COMMAND("Lights", "Follow-me-home", FollowMeHome),
  WEB_COMMAND("Lights", "Lights off", TurnOffLights),

  WEB_COMMAND("Locks", "Lock / unlock", Doors_Unlock_Interior),
  WEB_COMMAND("Locks", "Lock doors", Doors_Lock_Key),
  WEB_COMMAND("Locks", "Open trunk", Trunk_Open),

  WEB_COMMAND("Windows", "Driver open", Window_FrontDriver_Open),
  WEB_COMMAND("Windows", "Driver close", Window_FrontDriver_Close),
  WEB_COMMAND("Windows", "Passenger open", Window_FrontPassenger_Open),
  WEB_COMMAND("Windows", "Passenger close", Window_FrontPassenger_Close),
  WEB_COMMAND("Windows", "Rear driver open", Window_RearDriver_Open),
  WEB_COMMAND("Windows", "Rear driver close", Window_RearDriver_Close),
  WEB_COMMAND("Windows", "Rear passenger open", Window_RearPassenger_Open),
  WEB_COMMAND("Windows", "Rear passenger close", Window_RearPassenger_Close),
  WEB_COMMAND("Windows", "Sunroof open", Sunroof_Open_Piece),
  WEB_COMMAND("Windows", "Sunroof close", Sunroof_Close_Piece),

  WEB_COMMAND("Interior", "Interior light on", Interior_On),
  WEB_COMMAND("Interior", "Interior light off", Interior_Off),
  WEB_COMMAND("Interior", "Flash alarm LED", CLOWN_FLASH),

  WEB_COMMAND("Wipers", "Front wipers", Wipers_Front),
  WEB_COMMAND("Wipers", "Front washer", Washer_Front),
};

const byte webCommandCount = sizeof(webCommands) / sizeof(webCommands[0]);

#endif
