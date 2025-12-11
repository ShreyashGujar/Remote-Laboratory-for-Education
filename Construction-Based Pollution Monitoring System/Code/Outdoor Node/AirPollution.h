// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _AirPollution_H_
#define _AirPollution_H_
#include "Arduino.h"
//add your includes for the project AirPollution here
//#include <Arduino.h>
#include "HardwareSerial.h"

#include <stdlib.h>
#include <stdio.h>

#include "scrc_constants.h"
#include "scrc_wifi.h"
#include "scrc_time.h"
#include "scrc_http.h"
#include "scrc_pub_onem2m.h"
#include "scrc_dht.h"
#include "scrc_sds.h"
#include "scrc_thingspeak.h"

//end of add your includes here
// struct sensors_data {
// 	uint64_t time_stamp;  // timestamp number

// 	// DHT22
// 	float temp;
// 	float rh;
// 	short int_padding_1; // padding is based on the no of parameters and sizeof each parameter
//  // SDS011
// 	float pm25;
// 	float pm10;
// };
	
//add your function definitions for the project AirPollution here

//Do not add code below this line
#endif /* _AirPollution_H_ */
