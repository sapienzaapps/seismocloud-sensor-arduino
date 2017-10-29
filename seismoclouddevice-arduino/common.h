#ifndef __COMMON_H
#define __COMMON_H

// ************************** PLATFORM AUTODETECTION ****************************
#include <Arduino.h>
// Arduino UNO
#ifdef ARDUINO_AVR_UNO
#define IS_ARDUINO
#define MODEL "uno"
#else

// NodeMCU 12E
#ifdef ESP8266
#define IS_ESP
#define MODEL "esp8266"
#else

// Arduino MEGA
#ifdef ARDUINO_AVR_MEGA2560
#define IS_ARDUINO
#define MODEL "mega2560"
#else

// Arduino Ethernet
#ifdef ARDUINO_AVR_ETHERNET
#define IS_ARDUINO
#define MODEL "uno-e"
#else

#error "Unsupported platform"
#endif
#endif
#endif
#endif

// ************************ BEGIN CONFIG **************************

// This flag will prevent updates - USE ONLY DURING DEVELOPMENT
// #define DONT_UPDATE

// On Arduino this flag has no effect if you use avr_boot bootloader (for self-update). You should switch to standard bootloader
// #define DEBUG

#ifdef IS_ARDUINO
// LED for Arduino
#define LED_RED     3
#define LED_YELLOW  2
#define LED_GREEN   5
#else
#ifdef IS_ESP
// LED for NodeMCU/ESP8266
#define LED_RED     D7
#define LED_YELLOW  D6
#define LED_GREEN   D5
#endif
#endif

// ************************ END CONFIG **************************

#include <SPI.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#define VERSION     "1.20"

// ******* DEBUG PART
#ifdef DEBUG
#define Debug(x) Serial.print(x)
#define Debugln(x) Serial.println(x)
#else
#define Debug(x)
#define Debugln(x)
#endif
// ******* /DEBUG PART

// ******* UPDATE
#ifndef DONT_UPDATE

#ifdef IS_ARDUINO
#include <PetitFS.h>
#define SS_SD_CARD   4
#define SS_ETHERNET 10
#endif

#include "update.h"
#endif
// ******* /UPDATE

#ifdef IS_ARDUINO
#include "AcceleroMMA7361.h"
#include <avr/wdt.h>
#define soft_restart()        \
apiDisconnect();            \
delay(5000);                \
do                          \
{                           \
    wdt_enable(WDTO_15MS);  \
    for(;;)                 \
    {                       \
    }                       \
} while(0)
#endif

#ifdef IS_ESP
#include <Wire.h>                // I2C
#include <ESP8266WiFi.h>         // ESP8266 Core WiFi Library
#include <DNSServer.h>           // Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>    // Local WebServer used to serve the configuration portal
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266httpUpdate.h>   // For OTA
#include "MPU6050.h"

#define min(X,Y) ((X)<(Y) ? (X) : (Y))
#define soft_restart()      \
apiDisconnect();            \
delay(5000);                \
ESP.restart()
#endif

#include "LED.h"
#include "seismometer.h"
#include "api.h"
#include "nodemcu.h"

#define BUFFER_SIZE 512
extern byte buffer[BUFFER_SIZE];
extern byte ethernetMac[6];

/**
 * Format EEPROM (you'll lose all data)
 */
void initEEPROM();

/**
 * Check if EEPROM is faulty or not
 */
void checkEEPROM();

/**
 * Check MAC Address - if not present, a new one will be generated
 */
void checkMACAddress();

/**
 * Force a MAC Address
 */
void setMACAddress(byte* mac);

/**
 * Load config from EEPROM if it's valid
 */
void loadConfig();

/**
 * Returns the device ID (as char array)
 */
void getDeviceId(char* dest);

/**
 * Returns the device ID (as byte array)
 */
void getDeviceId(byte* dest);

/**
 * Switch to SD Card
 */
void selectSD();

/**
 * Switch to Ethernet card
 */
void selectEthernet();

#ifdef DEBUG
void printMACAddress();
void printUNIXTime();
void setProbeSpeedStatistic(uint32_t);
uint32_t getProbeSpeedStatistic();
#endif

class MyRingBuffer
{
public:
  MyRingBuffer(unsigned int size);
  ~MyRingBuffer();

  void reset();
  void init();
  void push(char c);
  int getPos();
  bool endsWith(const char* str);

  unsigned int _size;
  char* ringBuf;
  char* ringBufEnd;
  char* ringBufP;

};

#endif
