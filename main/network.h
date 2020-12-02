#ifndef __SOIL_MONITOR_NETWORK_H__
#define __SOIL_MONITOR_NETWORK_H__

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <esp_event_base.h>
#include <esp_event.h>

#define TAG "soil_monitor"

#define ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define ESP_MAXIMUM_RETRY  CONFIG_ESP_MAXIMUM_RETRY

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

bool wifi_connect();
void wifi_setLowPower();
void wifi_restoreNormalPower();

#endif
