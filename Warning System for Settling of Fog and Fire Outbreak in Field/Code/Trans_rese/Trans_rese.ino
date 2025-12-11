#include <SoftwareSerial.h>
//Including required Libraries
SoftwareSerial mySerial(3, 4);
int led = 2;
#include <dht.h>
#include <SPI.h>   //To handle the communication interface with the modem.
#include "RF24.h"  //The library which helps us to control the radio modem.
dht DHT;
#define dht_apin 4
String message;

RF24 myRadio (9, 10);  //( CE , CSN )     
byte addresses[][6] = {"00001","00005"}; //Address to which data to be transmitted. 
int rainsensor = A1;
const int ldr = A0; 
int rainValue;
int ldrStatus;
int dhtvalue;  
int rece = 0;
struct package   //Creating Structure which allows us to combine data of different types together.
{
    int rain = 0;
    int ldrStatus = 0;
    int dhtvalue = 0;
    int buttonState = 0;
};
typedef struct package Package;
Package data;


void setup()
{

  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing...");
  delay(1000);//Delay to let system boot
  
  mySerial.println("AT+CNMI=2,2,0,0,0"); 
  delay(1000);
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
  
  pinMode(ldr, INPUT);
  delay(1000);
  myRadio.begin(); // Activate the modem
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe( addresses[1]); //Sets the address of transmitter to which data will send.
  myRadio.openReadingPipe(1, addresses[0]);
 
  
  delay(100);
}

void loop()
{
DHT.read11(dht_apin);
    
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    data.dhtvalue = DHT.humidity;
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
     rainValue = analogRead(rainsensor);   
   Serial.println("Analog Value for rain sensor : ");
   Serial.println(rainValue);
   data.rain = rainValue;
     ldrStatus = analogRead(ldr);
     Serial.println("LDR Value is : ");
    Serial.println(ldrStatus);
    data.ldrStatus = ldrStatus;
    
    
    if (mySerial.available()>0){
  message = mySerial.readString();
  }
  if(message.indexOf("ON") > -1){
      Serial.println("LED ON");
      digitalWrite(led,HIGH);
      data.buttonState = 1;
    
//      myRadio.write(&data.buttonState, sizeof(data.buttonState)); //Sending the hole data to nrf24_server. 
//      Serial.println("Data Send.................");
      Serial.println(data.buttonState);
      delay(2000);
     }
    else if(message.indexOf("OFF") > -1){
      Serial.println("LED OFF");
      digitalWrite(led,LOW);
      data.buttonState = 0;
  
//      myRadio.write(&data.buttonState, sizeof(data.buttonState)); //Sending the hole data to nrf24_server. 
//      Serial.println("Data Send.................");
      Serial.println(data.buttonState);
      delay(2000);
    }
    delay(1000);
  myRadio.stopListening();
  myRadio.write(&data, sizeof(data)); //Sending the hole data to nrf24_server. 
  Serial.println("Data Send.................");
  delay(2000);
myRadio.startListening(); //Setting modem in Receiver mode.
  if ( myRadio.available()) 
  {    
  myRadio.read(&rece, sizeof(rece));
  Serial.println("Reseveing data ");
  Serial.println(rece);
     delay(2000);

  }}
