#include "PMS.h"
#include "SoftwareSerial.h"
#include <SDS011.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

SoftwareSerial pmsSerial(16, 17); // RX, TX
 
PMS pms(pmsSerial);
PMS::DATA data;

float p10,p25;
int error;
SDS011 my_sds;

Adafruit_BME280 bme;
float temperature, humidity;
 
void setup()
{
  Serial.begin(9600);
  pmsSerial.begin(9600);
  my_sds.begin(14,12);
  bme.begin(0x76);
}
 
void loop()
{
  if (pms.read(data))
  {
    Serial.println("Dust Concentration");
    Serial.println("PM2.5 :" + String(data.PM_AE_UG_2_5) + "(ug/m3)");
    Serial.println("PM10  :" + String(data.PM_AE_UG_10_0) + "(ug/m3)");
    
    delay(1000);
  }
//  SDS011();
//    BME();
}

void SDS011()
{
  error = my_sds.read(&p25,&p10);
  if (! error) {
    Serial.println("P2.5: "+String(p25));
    Serial.println("P10:  "+String(p10));
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
