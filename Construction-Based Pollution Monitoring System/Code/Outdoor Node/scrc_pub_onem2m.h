/*
 * scrc_pub_onem2m.h
 *
 *  Created on: 16-Oct-2020
 *      Author: BSRC-Sam
  *  last update :04-08-2023
 *      Author : SCRC-Sasidhar
 */

#ifndef INCLUDE_SCRC_PUB_ONEM2M_H_
#define INCLUDE_SCRC_PUB_ONEM2M_H_

#include "AirPollution.h"

int pub_15_sec_data(const struct sensors_data *ptr_buf, const short int latest_buf_idx, const short int hist_buf_idx, bool pub_hist);
int pub_10_min_data(const struct sensors_data *ptr_buf,int hist_start_idx,
                    const short int latest_buf_idx);

                    
#endif /* INCLUDE_SCRC_PUB_ONEM2M_H_ */
