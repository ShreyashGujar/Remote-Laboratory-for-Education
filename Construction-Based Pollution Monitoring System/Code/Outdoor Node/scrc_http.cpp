// scrc_http.cpp

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include "scrc_http.h"
#include "scrc_wifi.h"
#include "scrc_constants.h"

WiFiClientSecure httpsClient;

void connect_setup_http() {
}

bool connect_http() {
  Serial.print("HTTP Connecting: http://");
  Serial.print(CSE_IP);
  Serial.print(":");
  Serial.print(CSE_PORT);
  Serial.print("/");
  
  WiFiClient client;
  client.setTimeout(2000); // 2 Seconds
  delay(1000);

  int r = 0; //retry counter
  while ((!client.connect(CSE_IP, CSE_PORT)) && (r < 30)) {
    delay(1000);
    debug_info(".");
    r++;
  }

  debug_info("requesting URL: ");
  debug_info(CSE_IP);

  if (r == 30) {
    debug_info(": Connection failed");
    // return error
    return false;
  } else {
    debug_info(": Connected to web");
    //return success
    return true;
  }
}

void disconnect_http() {
  HTTPClient http;
  Serial.print("HTTP disconnected.");
  http.end();
}

bool connect_https() {
  Serial.print("HTTPS Connecting: https://");
  Serial.print(CSE_IP);
  Serial.print(":");
  Serial.print(CSE_PORT);
  Serial.print("/");
  
  httpsClient.setInsecure(); // Allow connection to the server without verifying the SSL certificate
  httpsClient.setTimeout(2000); // 2 Seconds
  delay(1000);

  int r = 0; //retry counter
  while ((!httpsClient.connect(CSE_IP, CSE_PORT)) && (r < 30)) {
    delay(1000);
    debug_info(".");
    r++;
  }

  debug_info("requesting URL: ");
  debug_info(CSE_IP);

  if (r == 30) {
    debug_info(": Connection failed");
    // return error
    return false;
  } else {
    debug_info(": Connected to web");
    //return success
    return true;
  }
}

void disconnect_https() {
  Serial.print("HTTPS disconnected.");
  httpsClient.stop();
}

int post_request_http(String req) {
  if (!connect_http()) {
    debug_info("Connecting to onem2m failed!!!\n");
    return E_OM2M_CONNECTION;
  }

  WiFiClient client;

  Serial.print("Connecting to ");
  Serial.println(CSE_IP);

  if (!client.connect(CSE_IP, CSE_PORT)) {
    Serial.println("Connection failed to server");
    return E_OM2M_CONNECTION;
  }

  Serial.println(req + "\n");

  // Send the HTTP request
  client.print(req);

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return E_OM2M_NO_RESPONSE;
    }
  }

  // Wait until the client sends some data
  Serial.println("Reply was:");
  Serial.println("==========");
  String line;
  while (client.available()) {
    line = client.readStringUntil('\n'); //Read Line by Line
    Serial.println(line); //Print response
  }
  Serial.println("==========");
  Serial.println("Closing connection");

  return SUCCESS_OM2M;
}

int post_request_https(String req) {
  if (!connect_https()) {
    debug_info("Connecting to onem2m failed!!!\n");
    return E_OM2M_CONNECTION;
  }

  Serial.print("Connecting to ");
  Serial.println(CSE_IP);

  if (!httpsClient.connect(CSE_IP, CSE_PORT)) {
    Serial.println("Connection failed to server");
    return E_OM2M_CONNECTION;
  }

  Serial.println(req + "\n");

  // Send the HTTPS request
  httpsClient.print(req);

  unsigned long timeout = millis();
  while (httpsClient.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      httpsClient.stop();
      return E_OM2M_NO_RESPONSE;
    }
  }

  // Wait until the client sends some data
  Serial.println("Reply was:");
  Serial.println("==========");
  String line;
  while (httpsClient.available()) {
    line = httpsClient.readStringUntil('\n'); //Read Line by Line
    Serial.println(line); //Print response
  }
  Serial.println("==========");
  Serial.println("Closing connection");

  httpsClient.stop(); // Close the connection

  return SUCCESS_OM2M;
}


