/*
 * scrc_sds.cpp
 *
 *  Created on: 16-Oct-2020
 *      Author: BSRC-Sam
  *  last update :04-08-2023
 *      Author : SCRC-Sasidhar
 */

#include <SDS011.h>

#include "scrc_sds.h"

#define MIN_READING 0.1
#define MAX_READING 999.9

#define node_RX D5
#define node_TX D6

SDS011 my_sds;
float p25, p10;

void hw_setup_sds() {
	my_sds.begin(D5, D6);
}

void hw_read_sds(float *buf_pm25, float *buf_pm10) {
	float error = my_sds.read(&p25, &p10);

	*buf_pm25 = p25;
	//error = validate_pm25(p25);
	if (*buf_pm25 == 0 || *buf_pm25 < MIN_READING || *buf_pm25 > MAX_READING) {
		*buf_pm25 = NAN;
	}

	Serial.print("PM25 :");
	Serial.println(*buf_pm25);

	*buf_pm10 = p10;
	//error = validate_pm10(p10);
	if (*buf_pm10 == 0 || *buf_pm10 < MIN_READING || *buf_pm10 > MAX_READING) {
		*buf_pm10 = NAN;
	}
	Serial.print("PM10 :");
	Serial.println(*buf_pm10);

}
