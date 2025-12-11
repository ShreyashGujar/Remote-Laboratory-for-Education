/*
 * scrc_sds.h
 *
 *  Created on: 16-Oct-2020
 *      Author: BSRC-Sam
  *  last update :04-08-2023
 *      Author : SCRC-Sasidhar
 */

#ifndef INCLUDE_SCRC_SDS_H_
#define INCLUDE_SCRC_SDS_H_

void hw_setup_sds();
void hw_read_sds(float *buf_pm25, float *buf_pm10);

#endif /* INCLUDE_SCRC_SDS_H_ */
