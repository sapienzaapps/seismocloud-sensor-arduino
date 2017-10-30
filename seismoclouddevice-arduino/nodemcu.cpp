
#include "nodemcu.h"
#ifdef IS_ESP

void NodeMCU::begin() {
  Debugln("Connecting to WiFi SeismoCloud");

  WiFiManager wifi;
  wifi.autoConnect("SeismoCloud");

#ifdef DEBUG
  memset(buffer, 0, BUFFER_SIZE);
  // if you get here you have connected to the WiFi
  Debugln("Wifi connected!");
  Debug("IP Address: ");
  IPAddress myIp = WiFi.localIP(); // NB. not from WiFiManager lib but ESP8266WiFi.h
  snprintf((char*)buffer, 16, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
  Debugln((char*)buffer);
#endif

  memset(buffer, 0, 16);
  // Getting MAC Address
  WiFi.macAddress(buffer);
  setMACAddress(buffer);
#ifdef DEBUG
  Debug("MAC Address: ");
  printMACAddress();
#endif
}

#endif
