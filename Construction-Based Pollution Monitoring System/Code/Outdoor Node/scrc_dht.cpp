/*
   scrc_dht.c

    Created on: 12-May-2022
        Author: Ayu Parmar
    last update :04-08-2023
 *      Author : SCRC-Sasidhar
*/
#include <Arduino.h>
#include <HardwareSerial.h>


#include <Adafruit_AHTX0.h>


Adafruit_AHTX0 aht;   // aht 10

#define MIN_HUM_READING 0               //minimum value of humidity sensor
#define MAX_HUM_READING 100             // max value of temperature sensor

#define MIN_TEMP_READING -40            //minimum value of temperature sensor
#define MAX_TEMP_READING 85             //maximum value of temperature sensor


void hw_setup_aht() {
  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
  }
  Serial.println("AHT10 or AHT20 found");
}

void hw_read_aht(float *buf_temp, float *buf_rh) {
  Serial.println("...hw_read_aht");
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data

 // float temp_1 = temp.temperature;
  *buf_temp = temp.temperature;
  // Error validation
  if (isnan(
        *buf_temp) || *buf_temp < MIN_TEMP_READING || *buf_temp > MAX_TEMP_READING) {
    *buf_temp = NAN;
  }

  Serial.print("......Temp :");
  Serial.println(*buf_temp);

  //zxzfloat rh_1 = humidity.relative_humidity;

  *buf_rh = humidity.relative_humidity;
  if (isnan(*buf_rh) || *buf_rh < MIN_HUM_READING || *buf_rh > MAX_HUM_READING) {
    *buf_rh = NAN;
  }
  Serial.print("......Hum :");
  Serial.println(*buf_rh);

}
