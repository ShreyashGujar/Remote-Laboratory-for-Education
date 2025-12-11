#include <WiFi.h>
#include "time.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "SD.h"
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_SGP40.h"
#include "Adafruit_SHT4x.h"
#include <HTTPClient.h>
#include <ThingSpeak.h>



//--------------------------------------WiFi & Time----------------------------------------------------

#define WIFI_SSID "JioFi3_1E6520"
#define WIFI_PASSWORD "pk5bs3bc87"



#define CHANNEL_ID 2237935
#define CHANNEL_API_KEY "WHVGYNVPOV9VFT41"

WiFiClient client;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 5 * 3600;
const int daylightOffset_sec = 1800;

int timestamp;

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println();
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

void time_setup(){
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  timeClient.begin();
}

void time_loop(){
  unsigned long epochTime = getTime();
  struct tm* timeinfo;
  time_t rawtime = (time_t)epochTime;
  timeinfo = localtime(&rawtime);
  char timestamp[20];
  strftime(timestamp, sizeof(timestamp), "%Y/%m/%d %H:%M:%S ", timeinfo);
  Serial.println(timestamp);
  File dataFile = SD.open("/DATA_Node3/data_Node3.csv", FILE_APPEND);
  if (dataFile) {
    dataFile.print(timestamp);
    dataFile.print(",");
    Serial.println("----------Time updated successfully!-------------");
  } else {
    Serial.println("--------------Error updating Time----------------");
  }
}

//---------------------------------------------------SD-Card-------------------------------------------------------------------------

void SD_setup() {
  if (!SD.begin()) {
    Serial.println();
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  if (!SD.exists("/DATA_Node3/data_Node3.csv")) {
    File dataFile = SD.open("/DATA_Node3/data_Node3.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.println("timeStamp,PM2.5,PM10,temp,rh,co2,raw_voc,voc_index,  ");
      dataFile.close();
      Serial.println("CSV file created successfully!");
    } else {
      Serial.println();
      Serial.println("Error creating CSV file");
    }
  } else {
    Serial.println();
    Serial.println("File exists, Appending to existing file");
  }
}

void SD_final() {
  File dataFile = SD.open("/DATA_Node3/data_Node3.csv", FILE_APPEND);
  if (dataFile) {
    dataFile.println("");
    Serial.println("----------updated successfully!-------------");
  } else {
    Serial.println("--------------Error updating ----------------");
  }
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}
//---------------------------------------------------CO2------------------------------------------------------
#define PIN 18  // PWM pin of sensor(PIN 4)

unsigned long duration, th, tl;
int ppm;

void CO2_Monitor() {

  th = pulseIn(PIN, HIGH, 2008000) / 1000;
  tl = 1004 - th;
  ppm = 2000 * (th - 2) / (th + tl - 4);

  Serial.print(" Co2 Concentration: ");
  Serial.println(ppm);
  // File dataFile = SD.open("/DATA_Node3/data_Node3.csv", FILE_APPEND);
  // if (dataFile) {
    // dataFile.print(ppm);
    // dataFile.print(",");
    ThingSpeak.setField(5, ppm);
  //   Serial.println("----------CO2 updated successfully!-------------");
  // } else {
  //   Serial.println("--------------Error updating CO2----------------");
  // }
}


//----------------------------------------SHT4X_SGp40_VOC----------------------------------------------------
Adafruit_SGP40 sgp;
Adafruit_SHT4x sht4 = Adafruit_SHT4x();

float t;
float h;


void sht_sgp_setup() {
  while (!Serial) { delay(10); }  // Wait for serial console to open!

  if (!sgp.begin()) {
    Serial.println("Sensor not found :(");
    while (1)
      ;
  }
  if (!sht4.begin()) {
    Serial.println("Couldn't find SHT4x");
    while (1) delay(1);
  }
}


void sgp_loop() {
  int32_t voc_index;
  voc_index = sgp.measureVocIndex(t, h);
  uint16_t raw;
  raw = sgp.measureRaw();
  Serial.print("raw_voc: ");
  Serial.print(raw);
  Serial.println(" ppb");
  Serial.print("voc_index: ");
  Serial.println(voc_index);
  // File dataFile = SD.open("/DATA_Node3/data_Node3.csv", FILE_APPEND);
  // if (dataFile) {
  //   dataFile.print(raw);
  //   dataFile.print(",");
  //   dataFile.print(voc_index);
  //   dataFile.print(",");
    ThingSpeak.setField(6, raw);
    ThingSpeak.setField(7, voc_index);

  //   Serial.println("----------SGP_VOC updated successfully!-------------");
  // } else {
  //   Serial.println("--------------Error updating SGP_VOC----------------");
  // }
}


void sht_loop() {
  sensors_event_t humidity, temp;
  sht4.getEvent(&humidity, &temp);  // populate temp and humidity objects with fresh data
  t = temp.temperature;
  h = humidity.relative_humidity;
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" °C\t");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("% rH");

  // File dataFile = SD.open("/DATA_Node3/data_Node3.csv", FILE_APPEND);
  // if (dataFile) {
  //   dataFile.print(t);
  //   dataFile.print(",");
  //   dataFile.print(h);
  //   dataFile.print(",");
    ThingSpeak.setField(3, t);
    ThingSpeak.setField(4, h);

  //   Serial.println("----------SHT_data updated successfully!-------------");
  // } else {
  //   Serial.println("--------------Error updating SHT_data----------------");
  // }
}

//-----------------------------------------------PM Sensor-------------------------------------------------------------
int pm2 = 0;
int pm10 = 0;

unsigned long previous_loop, previous_10, previous_25, prev_time;

byte command_frame[9] = { 0xAA, 0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x67, 0xBB };
byte received_data[9];
int sum = 0;
void send_command(byte command) {
  command_frame[1] = command;
  int sum = command_frame[0] + command_frame[1] + command_frame[2] + command_frame[3] + command_frame[4] + command_frame[5] + command_frame[8];
  int rem = sum % 256;
  command_frame[6] = (sum - rem) / 256;
  command_frame[7] = rem;
  delay(1000);
  Serial.write(command_frame, 9);
}

bool checksum() {
  sum = int(received_data[0]) + int(received_data[1]) + int(received_data[2]) + int(received_data[3]) + int(received_data[4]) + int(received_data[5]) + int(received_data[8]);
  if (sum == ((int(received_data[6]) * 256) + int(received_data[7]))) {
    return true;
  } else
    return false;
}
void calculate_pm() {
  pm2 = int(received_data[4]) * 256 + int(received_data[5]);
  delay(2000);
  pm10 = int(received_data[2]) * 256 + int(received_data[3]);
  Serial.print("PM2.5: ");
  Serial.println(pm2);
  Serial.print("PM10: ");
  Serial.println(pm10);
  // File dataFile = SD.open("/DATA_Node3/data_Node3.csv", FILE_APPEND);
  // if (dataFile) {
  //   dataFile.print(pm2);
  //   dataFile.print(",");
  //   dataFile.print(pm10);
  //   dataFile.print(",");
    ThingSpeak.setField(1, pm2);
    ThingSpeak.setField(2, pm10);
  //   Serial.println("----------PM updated successfully!-------------");
  // } else {
  //   Serial.println("--------------Error updating PM----------------");
  // }
}

void PM_setup() {
  send_command(0x01);
}

void PM_Reading() {
  delay(5000);
  if (millis() - prev_time > 5000) {
    send_command(0x02);
    prev_time = millis();
  }
  if (Serial.available()) {
    Serial.readBytes(received_data, 9);
    if (checksum()) {
      calculate_pm();
    }
  }
}





//------------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  initWiFi();
  //time_setup();
  PM_setup();
  //SD_setup();
  sht_sgp_setup();
  ThingSpeak.begin(client);
  delay(30000);
}

void loop() {
  //time_loop();
  PM_Reading();
  sht_loop();
  CO2_Monitor();
  sgp_loop();
  //SD_final();
  ThingSpeak.writeFields(CHANNEL_ID, CHANNEL_API_KEY);
  delay(30000);
}

