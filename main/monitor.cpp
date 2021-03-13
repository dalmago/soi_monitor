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

    if (!wifi_connect()){
        sensors_cleanup();
        monitor_sleep(30 * 60, "Error connecting to WiFi. Will try again later."); // 30 min
    }

    if (!mqtt_app_start()){
        sensors_cleanup();
        mqtt_stop_app();
        wifi_disconnect();
        monitor_sleep(30 * 60, "Error connecting to MQTT. Will try again later."); // 30 min
    }

    std::vector<uint16_t> values;
    if (!sensor_read(values)){
        sensors_cleanup();
        mqtt_stop_app();
        wifi_disconnect();
        monitor_sleep(30 * 60, "Error reading sensors! Will try again later."); // 30 min
    }

    if (mqtt_app_send(values))
        ESP_LOGD(TAG, "Done");
    else
        ESP_LOGE(TAG, "Error publishing");

    mqtt_stop_app();
    wifi_disconnect();
    sensors_cleanup();
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
