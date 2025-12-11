/*
 * scrc_constant.h
 *
 *  Created on: 16-Oct-2020
 *      Author: BSRC-Sam
 *  last update :04-08-2023
 *      Author : SCRC-Sasidhar
 */

#ifndef INCLUDE_SCRC_CONSTANT_H_
#define INCLUDE_SCRC_CONSTANT_H_

/*
 * uncomment to print debug statements
 */
#define DEBUG false

/*
 * node type
 */

#define NT_POLLUTION
//#define NT_WATER
#define led_pin D7
/*
 * Controller type
 */
#define CT_ESP_8266
//#define CT_ESP_32

/*
 * uncomment to publish to om2m
 */
#define PUB_OM2M

/*
 * uncomment to publish to ThingSpeak
 */
#define PUB_THINGSPEAK

/*
 * time interval in milliseconds
 */
#define TIME_INTERVAL_15_SEC  15000

#define TIME_INTERVAL_1_HR    3600000             //1 * 60* 60 * 1000

#define TIME_INTERVAL_10_MIN  600000             //10*60 * 1000
#define TIME_INTERVAL_1_MIN  60000             //10*60 * 1000

// count of sensor reading in 10 min @ 15 sec, i.e., 4 reading per min, (40)
#define PRIMARY_BUF_COUNT  40

// count of 10 min avg sensor reading in 1 days, i.e., 6 reading per 1 hour, (144 reading)
#define SEC_BUF_COUNT  100

#define NODE_1     //GuestHouse 
// #define NODE_2     //Main gate
// #define NODE_3     //Behind KRB
// #define NODE_4     //Pump House -3
// #define NODE_5     //Library
// #define NODE_6     //Palash Nivas
// #define NODE_7     //Food Ball Ground
// #define NODE_8     //Khdhamaba Nivas
// #define NODE_9     //Anand Nivas
// #define NODE_10    //Bakul


/*
 * WiFi credentials
 * Thingspeak Credentials
 * Https  define 
//  */

#ifdef NODE_1 // Guest house

  // #define MAIN_SSID "SAHANA-1"  
  // #define MAIN_PASS "0BEEF6590BEEF6590BEEF65926"
  
  #define MAIN_SSID "IIIT-Guest"       
  #define MAIN_PASS "I%GR#*S@!"

  #define HTTPS     false
  #define OM2M_NODE_ID   "AQ-SN00-00"
  #define OM2M_DATA_CONT  "AQ-\SN00-00/Data"
  #define OM2M_DATA_LBL   "[\"AE-AQ\", \"V3.0.02\", \"AQ-SN00-00\", \"AQ-V3.0.02\"]"

  #define myChannel 885762;
  #define writeAPIKey  "VWMXNW6NJOQVVVI3"

#endif

#ifdef NODE_2 // Main Gate 

  #define MAIN_SSID "TP-Link_0EC8"  
  #define MAIN_PASS "78726598" 

  #define HTTPS     false
  #define OM2M_NODE_ID   "AQ-MG00-00"
  #define OM2M_DATA_CONT  "AQ-\MG00-00/Data"
  #define OM2M_DATA_LBL   "[\"AE-AQ\", \"V3.0.02\", \"AQ-MG00-00\", \"AQ-V3.0.02\"]"

  #define myChannel 939888;
  #define writeAPIKey  "WDVVTDSX00XL5V05"

#endif

#ifdef NODE_3  //Behind KRB

  #define MAIN_SSID "IIIT-Guest"       
  #define MAIN_PASS "I%GR#*S@!"

  #define HTTPS     false
  #define OM2M_NODE_ID   "AQ-KH00-00"
  #define OM2M_DATA_CONT  "AQ-\KH00-00/Data"
  #define OM2M_DATA_LBL   "[\"AE-AQ\", \"V3.0.02\", \"AQ-KH00-00\", \"AQ-V3.0.02\"]"

  #define myChannel 947311;
  #define writeAPIKey  "1CLG8H9WWR7Y7HGF"

#endif

#ifdef NODE_4  //Pump House -3

  #define MAIN_SSID "JioFi3_B51E5B" 
  #define MAIN_PASS "P1m9h0u53@3"

  #define HTTPS     false
  #define OM2M_NODE_ID   "AQ-PH03-00"
  #define OM2M_DATA_CONT  "AQ-\PH03-00/Data"
  #define OM2M_DATA_LBL   "[\"AE-AQ\", \"V3.0.02\", \"AQ-PH03-00\", \"AQ-V3.0.02\"]"

  #define myChannel 1099442;
  #define writeAPIKey  "UUESWH1NUKNQXM47"

#endif

#ifdef NODE_5 //Library

  #define MAIN_SSID "esw-m19@iiith"         
  #define MAIN_PASS "e5W-eMai@3!20hOct"

  #define HTTPS     false
  #define OM2M_NODE_ID   "AQ-VN90-00"
  #define OM2M_DATA_CONT  "AQ-\VN90-00/Data"
  #define OM2M_DATA_LBL   "[\"AE-AQ\", \"V3.0.02\", \"AQ-VN90-00\", \"AQ-V3.0.02\"]"

  #define myChannel 944545;
  #define writeAPIKey  "LH08QHJZ82YEXWUL"

#endif

#ifdef NODE_6 //Palash Nivas

  #define MAIN_SSID "TP-Link_3934"  
  #define MAIN_PASS "wdsdtumt9r"

  #define HTTPS     false
  #define OM2M_NODE_ID   "AQ-PL00-00"
  #define OM2M_DATA_CONT  "AQ-\PL00-00/Data"
  #define OM2M_DATA_LBL   "[\"AE-AQ\", \"V3.0.02\", \"AQ-PL00-00\", \"AQ-V3.0.02\"]"

  #define myChannel 938513;
  #define writeAPIKey  "SP2M6BRIC92GPIM8"

#endif

#ifdef NODE_7 //Food Ball Ground

  #define MAIN_SSID "esw-m19@iiith"         
  #define MAIN_PASS "e5W-eMai@3!20hOct"

  #define HTTPS     false
  #define OM2M_NODE_ID   "AQ-FG00-00"
  #define OM2M_DATA_CONT  "AQ-\FG00-00/Data"
  #define OM2M_DATA_LBL   "[\"AE-AQ\", \"V3.0.02\", \"AQ-FG00-00\", \"AQ-V3.0.02\"]"

  #define myChannel 945028;
  #define writeAPIKey  "4PCMYWJ8XUHUSHUW"

#endif

#ifdef NODE_8 //Khdhamaba Nivas

  #define MAIN_SSID "TP-Link_5674"
  #define MAIN_PASS "49508680"

  #define HTTPS     false
  #define OM2M_NODE_ID   "AQ-KN00-00"
  #define OM2M_DATA_CONT  "AQ-\KN00-00/Data"
  #define OM2M_DATA_LBL   "[\"AE-AQ\", \"V3.0.02\", \"AQ-KN00-00\", \"AQ-V3.0.02\"]"

  #define myChannel 937750;
  #define writeAPIKey  "W9L6N34IC1MNE7HN"

#endif

#ifdef NODE_9//Anand Nivas

  #define MAIN_SSID "CHD"
  #define MAIN_PASS ""  

  #define HTTPS     false   
  #define OM2M_NODE_ID   "AQ-AN00-00"
  #define OM2M_DATA_CONT  "AQ-\AN00-00/Data"
  #define OM2M_DATA_LBL   "[\"AE-AQ\", \"V3.0.02\", \"AQ-AN00-00\", \"AQ-V3.0.02\"]"

  #define myChannel 944798;
  #define writeAPIKey  "IGN8L3ACV8PWB9YK"

#endif

#ifdef NODE_10  //Bakul

  #define MAIN_SSID "JioFi3_1E6520"  
  #define MAIN_PASS "pk5bs3bc87"

  #define HTTPS     true
  #define OM2M_NODE_ID   "AQ-BN00-00"
  #define OM2M_DATA_CONT  "AQ-\BN00-00/Data"
  #define OM2M_DATA_LBL   "[\"AE-AQ\", \"V3.0.02\", \"AQ-BN00-00\", \"AQ-V3.0.02\"]"

  #define myChannel 928486;
  #define writeAPIKey  "PSKWJ8GP0HS55WL6"

#endif


#define RETRY_WIFI_INTERVAL 60000  // in ms (1 min)
#define RETRY_WIFI_FACTOR 2     //multiplication factor user to increment the retry interval

#define STARTUP_WIFI_TIMEOUT  60000   //WiFi connection Timeout
#define STARTUP_WIFI_RETRY_DELAY 100  // WiFi reconnection delay

#define Timeoffset 19800

#define Reading_10min 2

#define MAX_STRING_LEN 255

/*
 * OneM2M connection details
 */
#define CSE_IP      "onem2m.iiit.ac.in"
#define OM2M_ORGIN    "AirPoll@20:22uHt@Sas"
#define CSE_PORT    443
#define FINGER_PRINT  "10 3D D5 4E B1 47 DB 4B 5C B0 89 08 41 A7 A4 14 87 10 7F E8"
#define OM2M_MN       "/~/in-cse/in-name/"
#define OM2M_AE       "AE-AQ"
// #define OM2M_NODE_ID   "AQ-SN00-00"
// #define OM2M_DATA_CONT  "AQ-\SN00-00/Data"
// #define OM2M_DATA_LBL   "[\"AE-AQ\", \"V3.0.02\", \"AQ-SN00-00\", \"AQ-V3.0.02\"]"
// dev-server
// #define CSE_IP      "dev-onem2m.iiit.ac.in"
// #define OM2M_ORGIN    "Tue_20_12_22:Tue_20_12_22"

/*
 Debug Function
 */
#if DEBUG
#define debug_info(fmt, ...) Serial.print(fmt, ##__VA_ARGS__)
#define debug_err(fmt, ...) Serial.print(fmt, ##__VA_ARGS__)
#else
  #define debug_info(fmt, ...) ((void)0)
  #define debug_err(fmt, ...) ((void)0)
#endif

/*
 * Error Handling Codes
 */
#define E_OM2M_NW -101
#define E_OM2M_CONNECT -102
#define E_OM2M_CONNECTION -103
#define E_OM2M_NO_RESPONSE -104
#define E_OM2M_EMPTY_RESPONSE -105

#define E_THINGSPEAK_NW -201
#define E_THINGSPEAK_CONNECT -202
#define E_THINGSPEAK_CONNECTION -203
#define E_THINGSPEAK_NO_RESPONSE -204
#define E_THINGSPEAK_EMPTY_RESPONSE -205

/*
 * Success Codes
 */
#define SUCCESS_OM2M 400
#define SUCCESS_THINGSPEAK 800

#endif /* INCLUDE_SCRC_CONSTANT_H_ */
