/*
 * scrc_http.h
 *
 *  Created on: 16-Oct-2020
 *      Author: BSRC-Sam
  *  last update :04-08-2023
 *      Author : SCRC-Sasidhar
 */

#ifndef INCLUDE_SCRC_HTTP_H_
#define INCLUDE_SCRC_HTTP_H_

void connect_setup_http();
bool connect_http();
void disconnect_http();

bool connect_https();
void disconnect_https();

int post_request_http(String req); // Add this line for post_request_http
int post_request_https(String req); // Add this line for post_request_https


#endif /* INCLUDE_SCRC_HTTP_H_ */
