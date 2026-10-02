// -----------------------------------------------------------------------------
// Config.h - settings you may want to change before uploading
// -----------------------------------------------------------------------------

#ifndef CONFIG_H
#define CONFIG_H

// ---- Wi-Fi access point -----------------------------------------------------
// The ESP32 creates its own Wi-Fi network. Connect your phone to it and open
// http://192.168.4.1
//
// Everybody who knows the password can unlock the car, so there is no default
// password: the sketch does not compile until you set your own (8+ characters).
#define WIFI_SSID "BMW-E46"
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD ""
#endif

// ---- Pins -------------------------------------------------------------------
#define BUS_RX_PIN 16   // transceiver TXD -> ESP32
#define BUS_TX_PIN 17   // ESP32 -> transceiver RXD
#define SEN_STA_PIN 4   // transceiver SEN/STA, also wakes the ESP32 from deep sleep (must be an RTC GPIO)
#define ENABLE_PIN 5    // transceiver EN
#define LED_PIN 2       // bus activity LED

// ---- Sleep ------------------------------------------------------------------
// Default values, both can be changed later in the web interface.
#define DEFAULT_SLEEP_AFTER 60   // seconds without bus traffic before going to sleep
#define DEFAULT_WEB_AWAKE 300    // seconds to stay awake after the last action in the web interface

#endif
