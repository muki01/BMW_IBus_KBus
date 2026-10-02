/* -----------------------------------------------------------------------------
E46_Codes.h - BMW I/K-Bus message frames (BMW E46)

FRAME FORMAT
example IBUS message:
50 04 68 32 11 1F (volume up button pressed on the steering wheel)
|  |  |  |  |  |
|  |  |  |  |  checksum (xorsum of all previous bytes)
|  |  |  |  one or more data fields
|  |  |  message type/command type
|  |  destination address
|  length of message (including destination address and checksum)
source address

CHECKSUM
The frames in this file are stored WITHOUT their checksum. By default the
library calculates and appends it automatically, so a frame is sent like this:

    ibus.write(Trunk_Open, sizeof(Trunk_Open));

If a frame already ends with its checksum, pass false as the third argument
and the library sends it unchanged:

    ibus.write(FrameWithChecksum, sizeof(FrameWithChecksum), false);

The arrays are declared without a fixed size so that sizeof() returns the
exact number of bytes in the frame, and they are stored in flash (PROGMEM).

At the bottom of this file you will find the same frames including their
checksum, exactly as they appear on the bus. If you want to verify a checksum
manually, you can use this tool:
https://www.scadacore.com/tools/programming-calculators/online-checksum-calculator/
Just click "Calculate" and then check the value shown under "CheckSum8 Xor".
-----------------------------------------------------------------------------*/

#ifndef E46_CODES_H
#define E46_CODES_H

#include <Arduino.h>
#include <BMW_IBus_KBus_Modules.h>  // module addresses (M_GM5, M_DIA, M_IKE, ...) from the BMW IBus KBus library

// -----------------------------------------------------------------------------
// GM5 input/output addresses
static const uint8_t GM5_SET_IO                       = 0x0C;  // "set IO" diagnostic command
static const uint8_t GM5_BTN_DOME_LIGHT               = 0x01;  // dome light button
static const uint8_t GM5_BTN_TRUNK_OPEN1              = 0x02;  // trunk unlock button (1st)
static const uint8_t GM5_BTN_CENTER_LOCK              = 0x03;  // center console lock/unlock button
static const uint8_t GM5_BTN_TRUNK_OPEN2              = 0x05;  // interior trunk unlock button (2nd)
static const uint8_t GM5_BTN_WINDOW_DRIVER_DOWN       = 0x0A;  // driver window down button
static const uint8_t GM5_BTN_WINDOW_DRIVER_UP         = 0x0B;  // driver window up button
static const uint8_t GM5_BTN_WINDOW_PASSENGER_DOWN    = 0x0C;  // passenger window down button
static const uint8_t GM5_BTN_WINDOW_PASSENGER_UP      = 0x0D;  // passenger window up button
static const uint8_t GM5_INTERIOR_DIM                 = 0x30;  // interior dimming
static const uint8_t GM5_DOOR_LOCK_KEY                = 0x34;  // door lock key
static const uint8_t GM5_WINDOW_REAR_DRIVER_OPEN      = 0x41;  // rear driver window (open)
static const uint8_t GM5_WINDOW_REAR_DRIVER_CLOSE     = 0x42;  // rear driver window (close)
static const uint8_t GM5_WINDOW_REAR_PASSENGER_CLOSE  = 0x43;  // rear passenger window (close)
static const uint8_t GM5_WINDOW_REAR_PASSENGER_OPEN   = 0x44;  // rear passenger window (open)
static const uint8_t GM5_DOORS_FUEL_TRUNK             = 0x46;  // doors + fuel cap hardlock, open trunk
static const uint8_t GM5_DRIVERS_DOOR_LOCK            = 0x47;  // driver's door lock
static const uint8_t GM5_FRONT_WIPER                  = 0x49;  // front wiper
static const uint8_t GM5_LED_ALARM_WARNING            = 0x4E;  // red LED under interior mirror ("clown nose")
static const uint8_t GM5_ALL_DOORS_LOCK               = 0x4F;  // all doors except driver's door lock
static const uint8_t GM5_WINDOW_FRONT_DRIVER_OPEN     = 0x52;  // front driver window (open)
static const uint8_t GM5_WINDOW_FRONT_DRIVER_CLOSE    = 0x53;  // front driver window (close)
static const uint8_t GM5_WINDOW_FRONT_PASSENGER_OPEN  = 0x54;  // front passenger window (open)
static const uint8_t GM5_WINDOW_FRONT_PASSENGER_CLOSE = 0x55;  // front passenger window (close)
static const uint8_t GM5_WASHER_SPRAY                 = 0x62;  // windshield washer front spraying
static const uint8_t GM5_INTERIOR_OFF_DIM             = 0x68;  // interior light off and dim
static const uint8_t GM5_Hazard_IKE_LCM               = 0x70;  // hazard lights (IKE + LCM) + interior light dim
static const uint8_t GM5_Hazard_LCM_3s                = 0x75;  // hazard lights (LCM) for 3 seconds
static const uint8_t GM5_Sunroof_Open_Piece           = 0x7E;  // sunroof open a piece
static const uint8_t GM5_Sunroof_Close_Piece          = 0x7F;  // sunroof close a piece
static const uint8_t GM5_Trunk_Open3                  = 0x95;  // open the trunk (again)
static const uint8_t GM5_Doors_HardLock               = 0x97;  // doors lock (hardlock)

// GM5 state groups
static const uint8_t GM5_INPUT_STATE_DIGITAL = 0x00;  // request digital IO states
static const uint8_t GM5_INPUT_STATE_ANALOG  = 0x01;  // request analog IO states

// -----------------------------------------------------------------------------
// Example: building a frame from the named constants above
const byte toggleDomeLight[] PROGMEM = {
  M_DIA,               // sender ID (diagnostic interface)
  0x05,                // length of the message (including destination ID and checksum)
  M_GM5,               // destination ID (body control module)
  GM5_SET_IO,          // the type of message (IO manipulation)
  GM5_BTN_DOME_LIGHT,  // the first parameter (the IO line that we want to manipulate)
  0x01                 // second parameter
  // don't worry about the checksum, the library automatically calculates it for you
};

// -----------------------------------------------------------------------------
// Lights

// Alternative variants
// const byte ParkLights_And_Signals[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06};
// const byte ParkLights_And_Signals_And_FogLights[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x06};
// const byte FollowMeHome[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x06};

// const byte ParkLights[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x02, 0x08, 0x00, 0x06};
// const byte ParkLights_And_Signals[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x62, 0x48, 0x0A, 0x06};
// const byte ParkLights_And_Signals_And_FogLights[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x62, 0x48, 0x0B, 0x06};

const byte TurnOffLights[] PROGMEM = {0x3F, 0x0F, 0xD0, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xE4, 0xFF, 0x00};
const byte ParkLights_And_Signals[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x7A, 0x48, 0x0A, 0x06};
const byte ParkLights_And_Signals_And_FogLights[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x7A, 0x48, 0x0B, 0x06};
const byte Low_Beams[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x02, 0x4E, 0x0A, 0x06};
const byte FollowMeHome[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x06};
const byte GoodbyeLights[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x62, 0x08, 0xA0, 0x06};

const byte Fog[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x06};                   // fog lights
const byte LeftTail[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x06};              // left tail light (+ A)
const byte RearLight[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x06};             // rear light (+ A)
const byte Brake_Above[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x06};           // brake light above
const byte Brake_Left[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x06};            // brake light left (+ A)
const byte Brake_Right[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x06};           // brake light right (+ A)
const byte FrontRightLowBeam[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x06};     // low beam, front right
const byte FrontLeftLowBeam[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x06};      // low beam, front left
const byte LowBeamDelayed[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06};        // low beam delayed front + rear
const byte MainBeamLeft[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x06};          // main beam left + low beams on both sides
const byte HighBeamRightLow[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x06};      // high beams on both sides of the right low beam
const byte IgnitionLowBeam[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x06};       // ignition at low beam + beam (IKE + LCM)
const byte LeftRearContin[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x06};        // left rear turn signal continuously (+ A)
const byte RightRearContin[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x06};       // right rear turn signal continuously (+ A)
const byte LeftFrontTurnContin[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x06};   // left front turn signal continuously (+ A)
const byte RightFrontTurnContin[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x06};  // right front turn signal continuously (+ A)
const byte LeftParking[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x06};           // left parking light
const byte RightParking[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x06};          // right parking light
const byte HazardLights[] PROGMEM = {0x3F, 0x0B, 0xBF, 0x0C, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06};          // hazard lights

// const byte Flashing_Warning[] PROGMEM = {0x08, 0x00, 0x04, 0xBF, 0x76};   // Flashing: 2 = warning lights, 4 = low beam, high beam 8
const byte Hazard_IKE_LCM[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x70, 0x01};  // hazard lights (IKE + LCM) + interior light dim
const byte Hazard_LCM_3s[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x75, 0x01};   // hazard lights (LCM) for 3 seconds

// -----------------------------------------------------------------------------
// Sunroof
// const byte Sunroof_Open_Unlock[] PROGMEM = {0xBF, 0x7D, 0x00, 0x05, 0x00, 0x30};   // open sunroof and possibly unlock
// const byte Sunroof_Open_Lock[] PROGMEM = {0xBF, 0x7D, 0x00, 0x05, 0x00, 0x20};     // open sunroof and lock
// const byte Sunroof_Close_Unlock[] PROGMEM = {0xBF, 0x7D, 0x00, 0x05, 0x00, 0x70};  // close sunroof and possibly unlock
// const byte Sunroof_Close_Lock[] PROGMEM = {0xBF, 0x7D, 0x00, 0x05, 0x00, 0x60};    // close sunroof and lock
// const byte Sliding_Lock[] PROGMEM = {0xBF, 0x7D, 0x00, 0x05, 0x00, 0x00};          // sliding lock
// const byte Sliding_Unlock[] PROGMEM = {0xBF, 0x7D, 0x00, 0x05, 0x00, 0x10};        // sliding unlock
const byte Sunroof_Open_Piece[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x7E, 0x01};       // sunroof open a piece
const byte Sunroof_Close_Piece[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x7F, 0x01};      // sunroof close a piece

// -----------------------------------------------------------------------------
// Windows
const byte Window_FrontDriver_Open[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x52, 0x01};      // front driver window open a piece
const byte Window_FrontDriver_Close[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x53, 0x01};     // front driver window close a piece
const byte Window_FrontPassenger_Open[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x54, 0x01};   // front passenger window open a piece
const byte Window_FrontPassenger_Close[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x55, 0x01};  // front passenger window close a piece
const byte Window_RearDriver_Open[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x41, 0x01};       // rear driver window open a piece
const byte Window_RearDriver_Close[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x42, 0x01};      // rear driver window close a piece
const byte Window_RearPassenger_Open[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x44, 0x01};    // rear passenger window open a piece
const byte Window_RearPassenger_Close[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x43, 0x01};   // rear passenger window close a piece

// -----------------------------------------------------------------------------
// Interior light
const byte Interior_Off[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x01, 0x00};     // interior light turn off
const byte Interior_On[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x01, 0x01};      // interior light turn on
const byte Interior_Dim2[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x30, 0x01};    // ? + interior light dimming
const byte Interior_On3s[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x60, 0x01};    // interior light turn on for 3 seconds (no fade)
const byte Interior_OffDim[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x68, 0x01};  // interior light off and dim

// -----------------------------------------------------------------------------
// Doors and trunk
const byte Doors_Unlock_Interior[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x03, 0x01};  // doors unlock (interior button pressed)
const byte Doors_Lock_Key[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x34, 0x01};         // doors lock (key pressed)
const byte Doors_Fuel_Trunk[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x46, 0x01};       // doors + fuel cap hardlock, open trunk
const byte DriverDoor_Lock[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x47, 0x01};        // driver's door lock
const byte AllExceptDriver_Lock[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x4F, 0x01};   // all doors except driver's door lock
const byte Doors_HardLock[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x97, 0x01};         // doors lock (hardlock)

const byte Trunk_Open[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x02, 0x01};   // trunk open
const byte Trunk_Open2[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x05, 0x01};  // open the trunk (again)
const byte Trunk_Open3[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x95, 0x01};  // open the trunk (again)

// -----------------------------------------------------------------------------
// Wipers
const byte Wipers_Front[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x49, 0x01};  // windshield wipers front
const byte Washer_Front[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x62, 0x01};  // windshield washer front spraying

// -----------------------------------------------------------------------------
// Instrument cluster (IKE)
const byte AvgSpeedDelete[] PROGMEM = {0x3B, 0x05, 0x80, 0x41, 0x10, 0x0A};  // average speed delete
const byte RequestMileage[] PROGMEM = {0xBF, 0x03, 0x80, 0x16};              // request mileage
// Speed limit set (beeps when exceeded). Replace 0xXX with the wanted value before enabling this frame.
// const byte SpeedLimitBeep[] PROGMEM = {0x3B, 0x06, 0x80, 0x40, 0x09, 0x00, 0xXX};
const byte SpeedLimitCurrent[] PROGMEM = {0x3B, 0x05, 0x80, 0x41, 0x09, 0x20};  // speed limit set on current speed
const byte SpeedLimitDisable[] PROGMEM = {0x3B, 0x05, 0x80, 0x41, 0x09, 0x08};  // disable adjusted speed limit
const byte REQUEST_TIME[] PROGMEM = {0x68, 0x05, 0x80, 0x41, 0x01, 0x01};       // request current time from IKE

// -----------------------------------------------------------------------------
// Remote control, key and ignition
const byte Remote_LockButton[] PROGMEM = {0x00, 0x04, 0xBF, 0x72, 0x16};     // pressed lock button on remote control
const byte Remote_UnlockButton[] PROGMEM = {0x00, 0x04, 0xBF, 0x72, 0x26};   // pressed unlock button on remote control
const byte Remote_ReleaseButton[] PROGMEM = {0x00, 0x04, 0xBF, 0x72, 0x06};  // released button on remote control

const byte KEY_IN[] PROGMEM = {0x44, 0x05, 0xBF, 0x74, 0x04, 0x00};   // ignition key in (last byte = key number)
const byte KEY_OUT[] PROGMEM = {0x44, 0x05, 0xBF, 0x74, 0x00, 0xFF};  // ignition key out
const byte IGNITION_OFF[] PROGMEM = {0x80, 0x04, 0xBF, 0x11, 0x00};   // ignition off
const byte IGNITION_POS1[] PROGMEM = {0x80, 0x04, 0xBF, 0x11, 0x01};  // ignition position 1
const byte IGNITION_POS2[] PROGMEM = {0x80, 0x04, 0xBF, 0x11, 0x03};  // ignition on position 2
const byte REMOTE_UNLOCK[] PROGMEM = {0x00, 0x04, 0xBF, 0x72, 0x22};  // remote control unlock
const byte REMOTE_LOCK[] PROGMEM = {0x00, 0x04, 0xBF, 0x72, 0x12};    // remote control lock

const byte CLOWN_FLASH[] PROGMEM = {0x3F, 0x05, 0x00, 0x0C, 0x4E, 0x01};  // turn on clown nose for 3 seconds

// -----------------------------------------------------------------------------
// Steering wheel
const byte MFL_VOL_UP[] PROGMEM = {0x50, 0x04, 0x68, 0x32, 0x11};          // steering wheel volume up
const byte MFL_VOL_DOWN[] PROGMEM = {0x50, 0x04, 0x68, 0x32, 0x10};        // steering wheel volume down
const byte MFL_TEL_VOL_UP[] PROGMEM = {0x50, 0x04, 0xC8, 0x32, 0x11};      // steering wheel volume up - telephone
const byte MFL_TEL_VOL_DOWN[] PROGMEM = {0x50, 0x04, 0xC8, 0x32, 0x10};    // steering wheel volume down - telephone
const byte MFL_SES_PRESS[] PROGMEM = {0x50, 0x04, 0xB0, 0x3B, 0x80};       // steering wheel press and hold phone button
const byte MFL_SEND_END_PRESS[] PROGMEM = {0x50, 0x04, 0xC8, 0x3B, 0x80};  // steering wheel send/end press
const byte MFL_RT_PRESS[] PROGMEM = {0x50, 0x04, 0x68, 0x3B, 0x02};        // steering wheel R/T press

// -----------------------------------------------------------------------------
// CD changer
const byte CD_STOP[] PROGMEM = {0x68, 0x05, 0x18, 0x38, 0x01, 0x00};                                       // CD stop command
const byte CD_PLAY[] PROGMEM = {0x68, 0x05, 0x18, 0x38, 0x03, 0x00};                                       // CD play command
const byte CD_PAUSE[] PROGMEM = {0x68, 0x05, 0x18, 0x38, 0x02, 0x00};                                      // CD pause command
const byte CD_STOP_STATUS[] PROGMEM = {0x18, 0x0A, 0x68, 0x39, 0x00, 0x02, 0x00, 0x3F, 0x00, 0x07, 0x01};  // CD stop request
const byte CD_PLAY_STATUS[] PROGMEM = {0x18, 0x0A, 0x68, 0x39, 0x02, 0x09, 0x00, 0x3F, 0x00, 0x07, 0x01};  // CD play request

const byte BACK_ONE[] PROGMEM = {0x68, 0x05, 0x18, 0x38, 0x08, 0x00};      // back
const byte BACK_TWO[] PROGMEM = {0x68, 0x05, 0x18, 0x38, 0x08, 0x01};      // back
const byte LEFT[] PROGMEM = {0x68, 0x05, 0x18, 0x38, 0x0A, 0x01};          // left
const byte RIGHT[] PROGMEM = {0x68, 0x05, 0x18, 0x38, 0x0A, 0x00};         // right
const byte SELECT[] PROGMEM = {0x68, 0x05, 0x18, 0x38, 0x07, 0x01};        // select
const byte BUTTON_ONE[] PROGMEM = {0x68, 0x05, 0x18, 0x38, 0x06, 0x01};    // button 1
const byte BUTTON_TWO[] PROGMEM = {0x68, 0x05, 0x18, 0x38, 0x06, 0x02};    // button 2
const byte BUTTON_THREE[] PROGMEM = {0x68, 0x05, 0x18, 0x38, 0x06, 0x03};  // button 3
const byte BUTTON_FOUR[] PROGMEM = {0x68, 0x05, 0x18, 0x38, 0x06, 0x04};   // button 4
const byte BUTTON_FIVE[] PROGMEM = {0x68, 0x05, 0x18, 0x38, 0x06, 0x05};   // button 5
const byte BUTTON_SIX[] PROGMEM = {0x68, 0x05, 0x18, 0x38, 0x06, 0x06};    // button 6

const byte CDC_STATUS_REPLY_RST[] PROGMEM = {0x18, 0x04, 0xFF, 0x02, 0x01};                                                   // CDC status ready after reset to LOC
const byte CDC_STATUS_REQUEST[] PROGMEM = {0x68, 0x03, 0x18, 0x01};                                                           // CDC status request
const byte CDC_STATUS_REPLY[] PROGMEM = {0x18, 0x04, 0xFF, 0x02, 0x00};                                                       // CDC status reply
const byte CD_STATUS[] PROGMEM = {0x18, 0x0E, 0x68, 0x39, 0x00, 0x82, 0x00, 0x3F, 0x00, 0x07, 0x00, 0x00, 0x01, 0x01, 0x01};  // CD status

// -----------------------------------------------------------------------------
// Phone
const byte INCOMING_CALL[] PROGMEM = {0xC8, 0x04, 0xE7, 0x2C, 0x05};       // incoming phone call
const byte PHONE_ON[] PROGMEM = {0xC8, 0x04, 0xE7, 0x2C, 0x10};            // phone on
const byte HANDSFREE_PHONE_ON[] PROGMEM = {0xC8, 0x04, 0xE7, 0x2C, 0x11};  // hands free phone on
const byte ACTIVE_CALL[] PROGMEM = {0xC8, 0x04, 0xE7, 0x2C, 0x33};         // active phone call

// -----------------------------------------------------------------------------
// Radio and DSP
const byte DSP_STATUS_REQUEST[] PROGMEM = {0x68, 0x03, 0x6A, 0x01};          // DSP status request
const byte DSP_STATUS_REPLY[] PROGMEM = {0x6A, 0x04, 0xFF, 0x02, 0x00};      // DSP status reply
const byte DSP_STATUS_REPLY_RST[] PROGMEM = {0x6A, 0x04, 0xFF, 0x02, 0x01};  // DSP status ready after reset to LOC
const byte DSP_VOL_UP_1[] PROGMEM = {0x68, 0x04, 0x6A, 0x32, 0x11};          // rotary volume up 1 step
const byte DSP_VOL_UP_2[] PROGMEM = {0x68, 0x04, 0x6A, 0x32, 0x21};          // rotary volume up 2 step
const byte DSP_VOL_UP_3[] PROGMEM = {0x68, 0x04, 0x6A, 0x32, 0x31};          // rotary volume up 3 step
const byte DSP_VOL_DOWN_1[] PROGMEM = {0x68, 0x04, 0x6A, 0x32, 0x10};        // rotary volume down 1 step
const byte DSP_VOL_DOWN_2[] PROGMEM = {0x68, 0x04, 0x6A, 0x32, 0x20};        // rotary volume down 2 step
const byte DSP_VOL_DOWN_3[] PROGMEM = {0x68, 0x04, 0x6A, 0x32, 0x30};        // rotary volume down 3 step
const byte DSP_FUNC_0[] PROGMEM = {0x68, 0x04, 0x6A, 0x36, 0x30};            // DSP function 0
const byte DSP_FUNC_1[] PROGMEM = {0x68, 0x04, 0x6A, 0x36, 0xE1};            // DSP function 1
const byte DSP_SRCE_OFF[] PROGMEM = {0x68, 0x04, 0x6A, 0x36, 0xAF};          // DSP source = OFF
const byte DSP_SRCE_CD[] PROGMEM = {0x68, 0x04, 0x6A, 0x36, 0xA0};           // DSP source = CD
const byte DSP_SRCE_TUNER[] PROGMEM = {0x68, 0x04, 0x6A, 0x36, 0xA1};        // DSP source = tuner

const byte GO_TO_RADIO[] PROGMEM = {0x68, 0x04, 0xFF, 0x3B, 0x00};     // go to radio - I think
const byte BUTTON_PRESSED[] PROGMEM = {0x68, 0x04, 0xFF, 0x3B, 0x00};  // radio/telephone control, no buttons pressed

// Volume increments (lookup table, not a frame)
const byte VOL_INCREMENT[] PROGMEM = {
    0,   68,  70,  72,  74,  76,  78,  80,  82,  84,  86,  88,  90,  92,  94,  96,  98,  100, 102, 104, 106, 108,
    110, 112, 114, 116, 118, 120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152,
    154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190, 192};

// -----------------------------------------------------------------------------
// Reference: the frames above including their checksum, as they appear on the bus.
// Use it to recognise frames in the debug output. A frame stored in this form
// must be sent with ibus.write(frame, sizeof(frame), false).
//
//   toggleDomeLight                       3F 05 00 0C 01 01 36
//   TurnOffLights                         3F 0F D0 0C 00 00 00 00 00 00 00 00 40 E4 FF 00 B7
//   ParkLights_And_Signals                3F 0B BF 0C 00 00 00 00 7A 48 0A 06 B9
//   ParkLights_And_Signals_And_FogLights  3F 0B BF 0C 00 00 00 00 7A 48 0B 06 B8
//   Low_Beams                             3F 0B BF 0C 00 00 00 00 02 4E 0A 06 C7
//   FollowMeHome                          3F 0B BF 0C 00 00 80 00 00 00 00 06 01
//   GoodbyeLights                         3F 0B BF 0C 00 00 00 00 62 08 A0 06 4B
//   Fog                                   3F 0B BF 0C 00 00 00 00 00 00 01 06 80
//   LeftTail                              3F 0B BF 0C 00 00 00 00 00 40 00 06 C1
//   RearLight                             3F 0B BF 0C 00 00 00 00 00 00 08 06 89
//   Brake_Above                           3F 0B BF 0C 00 00 00 00 01 00 00 06 80
//   Brake_Left                            3F 0B BF 0C 00 00 00 00 08 00 00 06 89
//   Brake_Right                           3F 0B BF 0C 00 00 00 00 10 00 00 06 91
//   FrontRightLowBeam                     3F 0B BF 0C 00 00 00 00 00 02 00 06 83
//   FrontLeftLowBeam                      3F 0B BF 0C 00 00 00 00 00 04 00 06 85
//   LowBeamDelayed                        3F 0B BF 0C 10 00 00 00 00 00 00 06 91
//   MainBeamLeft                          3F 0B BF 0C 00 00 00 00 00 10 00 06 91
//   HighBeamRightLow                      3F 0B BF 0C 00 00 00 00 00 20 00 06 A1
//   IgnitionLowBeam                       3F 0B BF 0C 00 00 80 00 00 00 00 06 01
//   LeftRearContin                        3F 0B BF 0C 00 00 00 00 00 00 20 06 A1
//   RightRearContin                       3F 0B BF 0C 00 00 00 00 00 00 80 06 01
//   LeftFrontTurnContin                   3F 0B BF 0C 00 00 00 00 20 00 00 06 A1
//   RightFrontTurnContin                  3F 0B BF 0C 00 00 00 00 40 00 00 06 C1
//   LeftParking                           3F 0B BF 0C 00 00 00 40 00 00 00 06 C1
//   RightParking                          3F 0B BF 0C 00 00 00 80 00 00 00 06 01
//   HazardLights                          3F 0B BF 0C 20 00 00 00 00 00 00 06 A1
//   Hazard_IKE_LCM                        3F 05 00 0C 70 01 47
//   Hazard_LCM_3s                         3F 05 00 0C 75 01 42
//   Sunroof_Open_Piece                    3F 05 00 0C 7E 01 49
//   Sunroof_Close_Piece                   3F 05 00 0C 7F 01 48
//   Window_FrontDriver_Open               3F 05 00 0C 52 01 65
//   Window_FrontDriver_Close              3F 05 00 0C 53 01 64
//   Window_FrontPassenger_Open            3F 05 00 0C 54 01 63
//   Window_FrontPassenger_Close           3F 05 00 0C 55 01 62
//   Window_RearDriver_Open                3F 05 00 0C 41 01 76
//   Window_RearDriver_Close               3F 05 00 0C 42 01 75
//   Window_RearPassenger_Open             3F 05 00 0C 44 01 73
//   Window_RearPassenger_Close            3F 05 00 0C 43 01 74
//   Interior_Off                          3F 05 00 0C 01 00 37
//   Interior_On                           3F 05 00 0C 01 01 36
//   Interior_Dim2                         3F 05 00 0C 30 01 07
//   Interior_On3s                         3F 05 00 0C 60 01 57
//   Interior_OffDim                       3F 05 00 0C 68 01 5F
//   Doors_Unlock_Interior                 3F 05 00 0C 03 01 34
//   Doors_Lock_Key                        3F 05 00 0C 34 01 03
//   Doors_Fuel_Trunk                      3F 05 00 0C 46 01 71
//   DriverDoor_Lock                       3F 05 00 0C 47 01 70
//   AllExceptDriver_Lock                  3F 05 00 0C 4F 01 78
//   Doors_HardLock                        3F 05 00 0C 97 01 A0
//   Trunk_Open                            3F 05 00 0C 02 01 35
//   Trunk_Open2                           3F 05 00 0C 05 01 32
//   Trunk_Open3                           3F 05 00 0C 95 01 A2
//   Wipers_Front                          3F 05 00 0C 49 01 7E
//   Washer_Front                          3F 05 00 0C 62 01 55
//   AvgSpeedDelete                        3B 05 80 41 10 0A E5
//   RequestMileage                        BF 03 80 16 2A
//   SpeedLimitCurrent                     3B 05 80 41 09 20 D6
//   SpeedLimitDisable                     3B 05 80 41 09 08 FE
//   REQUEST_TIME                          68 05 80 41 01 01 AC
//   Remote_LockButton                     00 04 BF 72 16 DF
//   Remote_UnlockButton                   00 04 BF 72 26 EF
//   Remote_ReleaseButton                  00 04 BF 72 06 CF
//   KEY_IN                                44 05 BF 74 04 00 8E
//   KEY_OUT                               44 05 BF 74 00 FF 75
//   IGNITION_OFF                          80 04 BF 11 00 2A
//   IGNITION_POS1                         80 04 BF 11 01 2B
//   IGNITION_POS2                         80 04 BF 11 03 29
//   REMOTE_UNLOCK                         00 04 BF 72 22 EB
//   REMOTE_LOCK                           00 04 BF 72 12 DB
//   CLOWN_FLASH                           3F 05 00 0C 4E 01 79
//   MFL_VOL_UP                            50 04 68 32 11 1F
//   MFL_VOL_DOWN                          50 04 68 32 10 1E
//   MFL_TEL_VOL_UP                        50 04 C8 32 11 BF
//   MFL_TEL_VOL_DOWN                      50 04 C8 32 10 BE
//   MFL_SES_PRESS                         50 04 B0 3B 80 5F
//   MFL_SEND_END_PRESS                    50 04 C8 3B 80 27
//   MFL_RT_PRESS                          50 04 68 3B 02 05
//   CD_STOP                               68 05 18 38 01 00 4C
//   CD_PLAY                               68 05 18 38 03 00 4E
//   CD_PAUSE                              68 05 18 38 02 00 4F
//   CD_STOP_STATUS                        18 0A 68 39 00 02 00 3F 00 07 01 78
//   CD_PLAY_STATUS                        18 0A 68 39 02 09 00 3F 00 07 01 71
//   BACK_ONE                              68 05 18 38 08 00 45
//   BACK_TWO                              68 05 18 38 08 01 44
//   LEFT                                  68 05 18 38 0A 01 46
//   RIGHT                                 68 05 18 38 0A 00 47
//   SELECT                                68 05 18 38 07 01 4B
//   BUTTON_ONE                            68 05 18 38 06 01 4A
//   BUTTON_TWO                            68 05 18 38 06 02 49
//   BUTTON_THREE                          68 05 18 38 06 03 48
//   BUTTON_FOUR                           68 05 18 38 06 04 4F
//   BUTTON_FIVE                           68 05 18 38 06 05 4E
//   BUTTON_SIX                            68 05 18 38 06 06 4D
//   CDC_STATUS_REPLY_RST                  18 04 FF 02 01 E0
//   CDC_STATUS_REQUEST                    68 03 18 01 72
//   CDC_STATUS_REPLY                      18 04 FF 02 00 E1
//   CD_STATUS                             18 0E 68 39 00 82 00 3F 00 07 00 00 01 01 01 FC
//   INCOMING_CALL                         C8 04 E7 2C 05 02
//   PHONE_ON                              C8 04 E7 2C 10 17
//   HANDSFREE_PHONE_ON                    C8 04 E7 2C 11 16
//   ACTIVE_CALL                           C8 04 E7 2C 33 34
//   DSP_STATUS_REQUEST                    68 03 6A 01 00
//   DSP_STATUS_REPLY                      6A 04 FF 02 00 93
//   DSP_STATUS_REPLY_RST                  6A 04 FF 02 01 92
//   DSP_VOL_UP_1                          68 04 6A 32 11 25
//   DSP_VOL_UP_2                          68 04 6A 32 21 15
//   DSP_VOL_UP_3                          68 04 6A 32 31 05
//   DSP_VOL_DOWN_1                        68 04 6A 32 10 24
//   DSP_VOL_DOWN_2                        68 04 6A 32 20 14
//   DSP_VOL_DOWN_3                        68 04 6A 32 30 04
//   DSP_FUNC_0                            68 04 6A 36 30 00
//   DSP_FUNC_1                            68 04 6A 36 E1 D1
//   DSP_SRCE_OFF                          68 04 6A 36 AF 9F
//   DSP_SRCE_CD                           68 04 6A 36 A0 90
//   DSP_SRCE_TUNER                        68 04 6A 36 A1 91
//   GO_TO_RADIO                           68 04 FF 3B 00 A8
//   BUTTON_PRESSED                        68 04 FF 3B 00 A8

#endif  // E46_CODES_H
