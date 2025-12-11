
#include <SPI.h>   //To handle the communication interface with the modem.
#include "RF24.h"  //The library which helps us to control the radio modem.
String space = " ";
String sign = "&";

RF24 myRadio (9,10);  //( CE , CSN )  
byte addresses[][6] = {"00001","00005"};  //Address from which data to be received

int lig = 8;
struct package
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
  Serial.begin(9600); //Setting baudrate of Serial Port to 9600.
  delay(1000);
  pinMode(lig,OUTPUT);
   digitalWrite(lig,LOW);
  myRadio.begin();  //Activate the modem.
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ); 
  myRadio.openReadingPipe(0, addresses[1]); //Sets the address of receiver from which program will receive the data.
}


void loop()  
{

  myRadio.startListening(); //Setting modem in Receiver mode.
  if ( myRadio.available()) 
  {    
    myRadio.read( &data, sizeof(data)); //Read the received data and store in data.
     String values = data.buttonState + space + data.rain + space + data.ldrStatus + space + data.dhtvalue + space + sign;
     Serial.println("Receving Data");
     Serial.println(values);
     Serial.println(data.buttonState);
     Serial.println(data.rain);
     Serial.println(data.ldrStatus);
     Serial.println(data.dhtvalue);
      
     delay(100); 

     if (data.buttonState == 0)
     {
       digitalWrite(lig,LOW);
     }

          else if (data.buttonState == 1)
     {
       digitalWrite(lig,HIGH);
     }
     
  }
}
