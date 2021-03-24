#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_sleep.h>
#include <vector>

#include "configs.h"
#include "network.h"
#include "messenger.h"
#include "sensors.h"

void monitor_sleep(uint32_t seconds, const char* const message = nullptr);

extern "C" void app_main(){
    ESP_LOGD(TAG, "RUNNING!");
    sensors_init();

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    std::vector<uint16_t> avg_values;
    for (unsigned int i = 0; i < 8; i++){
        std::vector<uint16_t> values;
        if (!sensor_read(values)){
            sensors_cleanup();
            mqtt_stop_app();
            wifi_disconnect();
            monitor_sleep(30 * 60, "Error reading sensors! Will try again later."); // 30 min
        }

        if (avg_values.empty()){
            avg_values = std::vector<uint16_t>(values.size(), 0); // initialize with zeros
        }

        for (unsigned int j = 0; j < values.size(); j++){
            avg_values[j] += values[j];
        }
    }
    sensors_cleanup();

    for (unsigned int j = 0; j < avg_values.size(); j++){
        avg_values[j] >>= 3; // value / 8
    }

    if (!wifi_connect()){
        monitor_sleep(30 * 60, "Error connecting to WiFi. Will try again later."); // 30 min
    }

    if (!mqtt_app_start()){
        mqtt_stop_app();
        wifi_disconnect();
        monitor_sleep(30 * 60, "Error connecting to MQTT. Will try again later."); // 30 min
    }

    if (mqtt_app_send(avg_values))
        ESP_LOGD(TAG, "Done");
    else
        ESP_LOGE(TAG, "Error publishing");

    mqtt_stop_app();
    wifi_disconnect();
    ESP_LOGI(TAG, "WiFi disconnected");

    monitor_sleep(2 * 3600); // 2 hours

    // This will never run
    while (true) {
        vTaskDelay(1000 / portTICK_RATE_MS);
        ESP_LOGE(TAG, ".");
    }
}

void monitor_sleep(uint32_t seconds, const char* const message){
    if (message != nullptr){
        ESP_LOGE(TAG, message);
    }

    if (seconds > 1800) // more than 30min
        seconds *= 1.04; /// + 4% for timer imprecision

    esp_deep_sleep(seconds * 1e6);
}
