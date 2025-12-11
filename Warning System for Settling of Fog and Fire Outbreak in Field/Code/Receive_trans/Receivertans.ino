
#include <SPI.h>   //To handle the communication interface with the modem.
#include "RF24.h"  //The library which helps us to control the radio modem.
String space = " ";
String sign = "&";

RF24 myRadio (9,10);  //( CE , CSN )  
byte addresses[][6] = {"00001","00005"};  //Address from which data to be received
int Soilsensor = A0;
int lig = 8;
struct package
{
    int rain = 0;
    int ldrStatus = 0;
    int dhtvalue = 0;
  
};
typedef struct package Package;
Package data;

void setup() 
{
  Serial.begin(9600); //Setting baudrate of Serial Port to 9600.
  delay(1000);
  pinMode(lig,OUTPUT);
   digitalWrite(lig,LOW);
  myRadio.begin();  //Activate the modem.
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ); 
//  myRadio.openReadingPipe(0, addresses[1]); //Sets the address of receiver from which program will receive the data.
  myRadio.openWritingPipe(addresses[0]);
}


void loop()  
{

 
     
  
  myRadio.stopListening();
  int rainValue = analogRead(Soilsensor);
   
    myRadio.write(&rainValue, sizeof(rainValue));
    Serial.println("Data sending ........................");
}
