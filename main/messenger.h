#ifndef __SOIL_MONITOR_MESSENGER_H__
#define __SOIL_MONITOR_MESSENGER_H__

#include <string>
#include "mqtt_client.h"

#include "configs.h"

#define UBIDOTS_DEVICE_LABEL "soil_monitor"
#define UBIDOTS_VARIABLE_LABEL "humidity1"

#define MQTT_BROKER_URL "industrial.api.ubidots.com"
// #define MQTT_BROKER_PORT 8883

#define MQTT_USER CONFIG_UBIDOTS_API_TOKEN
#define MQTT_PASSWD "" // any character - not required -

#define UBIDOTS_MQTT_TOPIC "/v1.6/devices/" UBIDOTS_DEVICE_LABEL

bool mqtt_app_start(void);
void mqtt_stop_app(void);
bool mqtt_app_send(const uint16_t value);
std::string generate_payload(const std::string variable, const uint16_t value);
esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event);

#endif
