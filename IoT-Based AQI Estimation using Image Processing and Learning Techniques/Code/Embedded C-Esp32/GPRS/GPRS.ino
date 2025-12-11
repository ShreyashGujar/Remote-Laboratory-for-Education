#include <String.h>//string
//gsm
#define BOARD_USART3_TX_PIN PB10
#define BOARD_USART3_RX_PIN PB11


void setup()
{
//  startTime = millis();
  Serial3.begin(9600);               // the GPRS baud rate
  Serial.begin(9600);
}



void loop()
{

}

void send_Data_gsm(float median, float temp_corr)
{
  if (Serial3.available())
    Serial.write(Serial3.read());
  Serial3.println("AT");
  delay(1000);
  Serial3.println("AT+CPIN?");
  delay(1000);
  Serial3.println("AT+CREG?");
  delay(1000);
  Serial3.println("AT+CGATT?");
  delay(1000);
  Serial3.println("AT+CIPSHUT");
  delay(1000);
  Serial3.println("AT+CIPSTATUS");
  delay(2000);
  Serial3.println("AT+CIPMUX=0");
  delay(2000);
  ShowSerialData();
  Serial3.println("AT+CSTT=\"cmnet\"");//start task and setting the APN,
  delay(1000);
  ShowSerialData();
  Serial3.println("AT+CIICR");//bring up wireless connection
  delay(3000);
  ShowSerialData();
  Serial3.println("AT+CIFSR");//get local IP adress
  delay(2000);
  ShowSerialData();
  Serial3.println("AT+CIPSPRT=0");
  delay(3000);
  ShowSerialData();
  Serial3.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
  ShowSerialData();
  Serial3.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
  String str = "GET https://api.thingspeak.com/update?api_key=ZSPEL4SS4NPDF1S8&field6=" + String(median) + "&field7=" + String(temp_corr);
  Serial.println(str);
  Serial3.println(str);//begin send data to remote server
  delay(4000);
  ShowSerialData();
  Serial3.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet
  Serial3.println();
  ShowSerialData();
  Serial3.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
}

void ShowSerialData()
{
  while (Serial3.available() != 0)
    Serial.write(Serial3.read());
  delay(5000);
}
