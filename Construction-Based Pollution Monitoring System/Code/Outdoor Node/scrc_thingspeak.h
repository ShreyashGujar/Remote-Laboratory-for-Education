/*
   scrc_thingspeak.h

    Created on: 16-Oct-2020
        Author: BSRC-Sam
*/

#include "scrc_constants.h"
#include "AirPOllution.h"

#ifndef INCLUDE_SCRC_THINGSPEAK_H_
#define INCLUDE_SCRC_THINGSPEAK_H_

void pub_setup_thingspeak();

struct sensors_data {
	uint64_t time_stamp;  // timestamp number

	// DHT22
	float temp;
	float rh;
	short int_padding_1; // padding is based on the no of parameters and sizeof each parameter
 // SDS011
	float pm25;
	float pm10;
};

int pub_thingspeak(const sensors_data *ptr_buf,
                   const short int latest_buf_idx);
int pub_10_min_thingspeak(const struct sensors_data *ptr_buf, int hist_start_idx,
                    const short int latest_buf_idx);
int HTTPPost( int numFields , String fieldData[], int i, String time_stamp  );
String getResponse();

#endif /* INCLUDE_SCRC_THINGSPEAK_H_ */
