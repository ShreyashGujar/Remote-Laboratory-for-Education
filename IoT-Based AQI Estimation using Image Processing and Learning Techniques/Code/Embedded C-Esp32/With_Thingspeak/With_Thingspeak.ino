#include <WiFi.h>
#include "ThingSpeak.h"
#include "PMS.h"
#include "SoftwareSerial.h"
#include <SDS011.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

const char* ssid = "Nilesh";   // your network SSID (name)
const char* password = "Nilesh_net";   // your network password

WiFiClient  client;

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "TIZ53EUD1JTU2C6B";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

int RXPin = 3;
int TXPin = 1;
int GPSBaud = 9600;
float lati, longi;

TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);

SoftwareSerial pmsSerial(16, 17); // RX, TX
PMS pms(pmsSerial);
PMS::DATA data;
float PM, PMV;

float p10, p25;
int error;
SDS011 my_sds;

Adafruit_BME280 bme;
float temperature, humidity;

void setup()
{
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  pmsSerial.begin(9600);
  Serial.begin(9600);
  my_sds.begin(14, 12);
  bme.begin(0x76);
  gpsSerial.begin(GPSBaud);
}

void loop()
{
  if ((millis() - lastTime) > timerDelay)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.print("Attempting to connect");
      while (WiFi.status() != WL_CONNECTED)
      {
        WiFi.begin(ssid, password);
        delay(5000);
      }
      Serial.println("\nConnected.");
    }
//    while (gpsSerial.available() > 0)
//      if (gps.encode(gpsSerial.read()))
//        GPS();
    PMS7003();
//    SDS011();
//    BME();

    ThingSpeak.setField(1, PM);
    ThingSpeak.setField(2, PMV);
    ThingSpeak.setField(3, temperature);
    ThingSpeak.setField(4, humidity);
    ThingSpeak.setField(5, p25);
    ThingSpeak.setField(6, p10);
    ThingSpeak.setField(7, lati);
    ThingSpeak.setField(8, longi);

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

void PMS7003()
{

  if (pms.read(data))
  {
    temperature = bme.readTemperature();
    humidity = bme.readHumidity();
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
    delay(1000);
    
    PM = data.PM_AE_UG_2_5;
    PMV = data.PM_AE_UG_10_0;
    Serial.println("Dust Concentration");
    Serial.println("PM2.5 :" + String(PM) + "(ug/m3)");
    Serial.println("PM10  :" + String(PMV) + "(ug/m3)");
    delay(1000);
  }
}

void SDS011()
{
  error = my_sds.read(&p25, &p10);
  if (! error) {
    Serial.println("P2.5: " + String(p25));
    Serial.println("P10:  " + String(p10));
  }
  delay(1000);
}

void BME()
{
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();

  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  delay(1000);
}

void GPS()
{
  if (gps.location.isValid())
  {
    lati = gps.location.lat();
    longi = gps.location.lng();
    Serial.print("Latitude: ");
    Serial.println(lati, 6);
    Serial.print("Longitude: ");
    Serial.println(longi, 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  }
  else
  {
    Serial.println("Location: Not Available");
  }

  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}
