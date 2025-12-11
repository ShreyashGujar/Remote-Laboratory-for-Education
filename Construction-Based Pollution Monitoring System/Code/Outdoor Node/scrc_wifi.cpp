/*
   scrc_wifi.cpp

    Created on: 16-Oct-2020
        Author: BSRC-Sam
         *  last update :04-08-2023
 *      Author : SCRC-Sasidhar
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "scrc_wifi.h"
#include "scrc_constants.h"
#include "scrc_time.h"

//#include "secrets.h"

static bool nw_connected = false;

void nw_setup() {
  //WiFi.mode(WIFI_OFF);   //Prevents reconnection issue (taking too long to connect)
  pinMode(led_pin,OUTPUT);
  WiFi.mode(WIFI_STA);                           //Only Station No AP, This line hides the viewing of ESP as wifi hotspot
}

// timeout in milliseconds
bool nw_start(unsigned long timeout, int retry_delay) {
  static unsigned long last_ms = millis();

  Serial.println("nw_start...\n");

  Serial.print("last_ms: " + String(last_ms));
  Serial.print(", timeout: " + String(timeout));
  Serial.println(", retry_delay: " + String(retry_delay));

  if (nw_connected) {
    //If already connected show IP address in serial monitor
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(MAIN_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(led_pin,HIGH);
    Serial.println("led ON");
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
    return nw_connected;
  }

  Serial.println("Establishing connection to WiFi...");
  // WiFi.mode(WIFI_OFF);                           //Prevents reconnection issue (taking too long to connect)
  WiFi.mode(WIFI_STA); //Only Station No AP, This line hides the viewing of ESP as wifi hotspot

  WiFi.begin(MAIN_SSID, MAIN_PASS);              //Connect to your WiFi router

  bool error = false;

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    if ((millis() - last_ms) > timeout) {
      Serial.println("last_ms: " + String(millis()));
      debug_info("timed out!!!\n");
      error = true;
      break;
    }
    Serial.print(".");
    delay(retry_delay);
  }

  if (error)
    nw_connected = false;
  else
    nw_connected = true;

  if (nw_connected) {
    //If connection successful show IP address in serial monitor
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(MAIN_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(led_pin,HIGH);
    Serial.println("led ON");
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");

    // sync_time();//case 1
    // Serial.println("case1111111111111111111111111");

  } else {
    Serial.println("");
    Serial.print("Unable to connect!!!");
  }

  return nw_connected;
}

void nw_stop() {
  if (nw_connected) {
    //TODO: use WiFi.forceSleepBegin instead of WiFi.disconnect
    //WiFi.disconnect();
    //WiFi.forceSleepBegin(sleepUs)
    nw_connected = false;
  }
}

bool nw_is_connected() {

  (WiFi.status() == WL_CONNECTED) ? nw_connected = true : nw_connected = false;

#ifdef DEBUG
  if (nw_connected) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(MAIN_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(led_pin,HIGH);
    Serial.println("led ON");
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
  }
#endif

  return nw_connected;
}
