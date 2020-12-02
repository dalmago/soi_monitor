#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#include "network.h"

#define CONFIG_LOG_DEFAULT_LEVEL 4

#define TAG "soil_monitor"

extern "C" void app_main(){
    ESP_LOGD(TAG, "RUNNING!");

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    for (int i = 0; i < 3; i++){
        vTaskDelay(1000 / portTICK_RATE_MS);
        ESP_LOGD(TAG, ".");
    }

    if (!wifi_connect()) return;
    for (int i = 0; i < 5; i++){
        vTaskDelay(1000 / portTICK_RATE_MS);
        ESP_LOGD(TAG, ".");
    }

    wifi_setLowPower();
    ESP_LOGI(TAG, "Low power mode set");

    for (int i = 0; i < 5; i++){
        vTaskDelay(1000 / portTICK_RATE_MS);
        ESP_LOGD(TAG, ".");
    }

    wifi_restoreNormalPower();
    ESP_LOGI(TAG, "Normal mode set");

    while (true) {
        vTaskDelay(1000 / portTICK_RATE_MS);
        ESP_LOGD(TAG, ".");
    }
}
