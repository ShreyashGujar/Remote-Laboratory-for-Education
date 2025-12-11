/*
 * scrc_wifi.h
 *
 *  Created on: 16-Oct-2020
 *      Author: BSRC-Sam
 *  last update :04-08-2023
 *      Author : SCRC-Sasidhar
 */

#ifndef INCLUDE_SCRC_WIFI_H_
#define INCLUDE_SCRC_WIFI_H_

void nw_setup();
bool nw_start(unsigned long timeout, int retry_delay);
void nw_stop();

bool nw_is_connected();

#endif /* INCLUDE_SCRC_WIFI_H_ */
