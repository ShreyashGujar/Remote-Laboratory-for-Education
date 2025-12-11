
#include "PMS.h"
#include <WiFi.h>
#include "ThingSpeak.h"
#include "SoftwareSerial.h"
#include "time.h"
#include <SDS011.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//#include <TinyGPS++.h>
#include <SoftwareSerial.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 19800;//GMT+5:30
#define tim;

//int RXPin = 3;
//int TXPin = 1;
//int GPSBaud = 9600;
//TinyGPSPlus gps;
//SoftwareSerial gpsSerial(RXPin, TXPin);


const char* ssid = "JioFi3_259D0E";
const char* password = "k61m12m0u5";

WiFiClient  client;

unsigned long myChannelNumber = 236;
const char * myWriteAPIKey = "VS67LK8NZJFHQD36";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 20000;

SoftwareSerial pmsSerial(16, 17); // RX, TX
PMS pms(pmsSerial);
PMS::DATA data;
float PM, PMV;
float p25_c, p10_c;

//float lan = atof(pdop.value());
//float lon = atof(hdop.value());
//
//float LAN = lan/100;
//float LON = lon/100;

float p10, p25;
int error;
SDS011 my_sds;

Adafruit_BME280 bme;
float temperature, humidity;

void setup()
{
  Serial.begin(9600);
  pmsSerial.begin(9600);
//  ss.begin(GPSBaud);
  my_sds.begin(14, 12);
  bme.begin(0x76);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop()
{
  if ((millis() - lastTime) > timerDelay) {

    // Connect or reconnect to WiFi
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("Attempting to connect");
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        delay(5000);
      }
      Serial.println("\nConnected.");
    }

    if (pms.read(data))
    {
      Serial.println("");
      printLocalTime();
      Serial.println("");
 
      PM = data.PM_AE_UG_2_5;
      PMV = data.PM_AE_UG_10_0;
      Serial.println("Dust Concentration");
      Serial.println("PM2.5 :" + String(PM) + "(ug/m3)");
      Serial.println("PM10  :" + String(PMV) + "(ug/m3)");
      Serial.println("");
//      delay(1000);

      temperature = bme.readTemperature();
      humidity = bme.readHumidity();
      Serial.print("Temperature: ");
      Serial.println(temperature);
      Serial.print("Humidity: ");
      Serial.println(humidity);
      Serial.println("");
//      delay(1000);

      error = my_sds.read(&p25, &p10);
      if (! error) {
        Serial.println("P2.5: " + String(p25));
        Serial.println("P10:  " + String(p10));
        Serial.println("");
      }
      ThingSpeak.setField(1, PM);
      ThingSpeak.setField(2, PMV);
      ThingSpeak.setField(3, temperature);
      ThingSpeak.setField(4, humidity);
      ThingSpeak.setField(5, p25);
      ThingSpeak.setField(6, p10);

//      ThingSpeak.setField(7, LAN);
//      ThingSpeak.setField(8, LON);

      int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      if (x == 200) {
        Serial.println("Channel update successful.");
      }
      else {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
      }
      lastTime = millis();
    }
  }
 }
 
//void GPS()
//{
//  Serial.print(F("LAT=")); Serial.print(LAN,7);
//  Serial.print(F("\tLON=")); Serial.println(LON,7);
//  delay(100);
//  gps.encode(ss.read());
//  Serial.println();
//}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%Y-%m-%d %H:%M:%S");

}
