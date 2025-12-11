// scrc_pub_onem2m.cpp

#include <Arduino.h>
#include "scrc_constants.h"
#include "scrc_pub_onem2m.h"
#include "scrc_http.h"
#include "scrc_wifi.h"
#include "scrc_time.h"

int publish_onem2m(const struct sensors_data *ptr_sensors_data, const bool is15sec);

int count_15 = 1;
int count_10 = 1;

int pub_15_sec_data(const struct sensors_data *ptr_buf, const short int latest_buf_idx, const short int hist_buf_idx, bool pub_hist) {
  int status = 0;
  int hw_time_interval = TIME_INTERVAL_15_SEC;
  unsigned long om2m_pub_interval = TIME_INTERVAL_1_MIN;

  debug_info("....sizeof(buffer): ");
  debug_info(sizeof(ptr_buf));
  debug_info(", sizeof(typeof(buffer)): ");
  debug_info(sizeof(typeof(ptr_buf[0])));
  debug_info("\n");

  int max = PRIMARY_BUF_COUNT;
  
  debug_info("Start pub 15 sec data...\n");

  // Check if WiFi is available
  if (!nw_is_connected()) {
    debug_info("No network, do nothing!!!\n");
    return -1;
  }

  int no_records = (om2m_pub_interval / hw_time_interval) - 1;

  int start_idx = latest_buf_idx + (max - no_records) + 1;
  int end_idx = max + latest_buf_idx;
  debug_info("no_records: " + String(no_records) + "\n");
  debug_info("start_idx: " + String(start_idx) + "\n");
  debug_info("end_idx: " + String(end_idx) + "\n");
  debug_info("max_record_count: " + String(max) + "\n");

  for (int i = start_idx; i <= end_idx; i++) {
    if (HTTPS == false) {
      if (!connect_http()) {
        debug_info("Connecting to onem2m failed!!!\n");
        return -2;
      }
    } else {
      if (!connect_https()) {
        debug_info("Connecting to onem2m failed!!!\n");
        return -2;
      }
    }

    Serial.print("Temp debug 1: ");
    float temp = ptr_buf[i % max].temp;
    Serial.println(ptr_buf[i % max].temp);

    status = publish_onem2m(&ptr_buf[i % max], true);

    Serial.print("count of 15 sec data: ");
    Serial.println(count_15++);
  }

  return status;
}

int pub_10_min_data(const struct sensors_data *ptr_buf, int hist_start_idx, const short int latest_buf_idx) {
  int status = 0;
  int hw_time_interval = TIME_INTERVAL_10_MIN;
  unsigned long om2m_pub_interval = TIME_INTERVAL_10_MIN;

  debug_info("....sizeof(buffer): ");
  debug_info(sizeof(ptr_buf));
  debug_info(", sizeof(typeof(buffer)): ");
  debug_info(sizeof(typeof(ptr_buf[0])));
  debug_info("\n");

  int max = SEC_BUF_COUNT;

  debug_info("Start pub 10 min data...\n");

  // Check if WiFi is available
  if (!nw_is_connected()) {
    debug_info("No network, do nothing!!!\n");
    return -1;
  }

  int no_records = latest_buf_idx - hist_start_idx;

  int start_idx = latest_buf_idx + (max - no_records);
  int end_idx = max + latest_buf_idx - 1;
  debug_info("no_records: " + String(no_records) + "\n");
  debug_info("start_idx: " + String(start_idx) + "\n");
  debug_info("end_idx: " + String(end_idx) + "\n");
  debug_info("max_record_count: " + String(max) + "\n");

  for (int i = start_idx; i <= end_idx; i++) {
    if (HTTPS == false) {
      if (!connect_http()) {
        debug_info("Connecting to onem2m failed!!!\n");
        return -2;
      }
    } else {
      if (!connect_https()) {
        debug_info("Connecting to onem2m failed!!!\n");
        return -2;
      }
    }

    debug_info("........i: " + String(i) + "\n");

    status = publish_onem2m(&ptr_buf[i % max], false);

    Serial.print("count of 10 min data: ");
    Serial.println(count_10++);
  }

  return status;
}

int publish_onem2m(const struct sensors_data *ptr_sensors_data, const bool is15sec) {
  char data[MAX_STRING_LEN];

  sprintf(data,
          "[%ld, %.2f, %.2f, %.2f, %.2f]",
          ptr_sensors_data->time_stamp,
          ptr_sensors_data->pm25, ptr_sensors_data->pm10,
          ptr_sensors_data->temp, ptr_sensors_data->rh);

  debug_info(data);
  debug_info("\n");

  String req_data = String() + "{\"m2m:cin\": {"
                              + "\"con\": \"" + data + "\","
                              + "\"lbl\": " + OM2M_DATA_LBL + ","
                              + "\"cnf\": \"text\""
                              + "}}";

  Serial.println(req_data);

  String link = String() + OM2M_MN + OM2M_AE + "/" + OM2M_DATA_CONT;
  Serial.print("onem2m ae/mn/node: ");
  Serial.println(link);

  String req = String() + "POST " + link + " HTTP/1.1\r\n" +
               "Host: " + CSE_IP + "\r\n" +
               "X-M2M-Origin:" + OM2M_ORGIN + "\r\n" +
               "Content-Type:application/json;ty=" + 4 + "\r\n" +
               "Content-Length: " + req_data.length() + "\r\n" +
               "Connection: close\r\n\n" +
               req_data;

  if (HTTPS == false) {
    return post_request_http(req);
  } else {
    return post_request_https(req);
  }
}


