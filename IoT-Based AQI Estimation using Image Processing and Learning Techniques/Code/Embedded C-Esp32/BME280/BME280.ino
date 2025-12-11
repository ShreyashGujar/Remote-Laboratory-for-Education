#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
Adafruit_BME280 bme;

float temperature, humidity;             
 
void setup() {
  Serial.begin(9600);
  delay(100);
  bme.begin(0x76);   
}

void loop() {
  
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();

  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  delay(2000);
  
}
