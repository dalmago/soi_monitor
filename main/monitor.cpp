#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_sleep.h>

#include "configs.h"
#include "network.h"
#include "messenger.h"

void monitor_sleep(uint32_t seconds, const char* const message = nullptr);

extern "C" void app_main(){
    ESP_LOGD(TAG, "RUNNING!");

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    if (!wifi_connect()){
        monitor_sleep(30 * 60, "Error connecting to WiFi. Will try again later."); // 30 min
    }

    if (!mqtt_app_start()){
        monitor_sleep(30 * 60, "Error connecting to MQTT. Will try again later."); // 30 min
    }

    uint16_t humidity1 = 255;
    if (mqtt_app_send(humidity1))
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
        ESP_LOGD(TAG, ".");
    }
}

void monitor_sleep(uint32_t seconds, const char* const message){
    if (message != nullptr){
        ESP_LOGE(TAG, message);
    }

    esp_deep_sleep(seconds * 1e6);
}
