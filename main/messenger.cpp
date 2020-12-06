#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <esp_log.h>

#include "messenger.h"

static EventGroupHandle_t mqtt_event_group;
const static uint8_t MQTT_CONNECTED_BIT = BIT0;
const static uint8_t MQTT_ERROR_BIT = BIT1;

static esp_mqtt_client_handle_t mqtt_client = NULL;

bool mqtt_app_start(void){
    mqtt_event_group = xEventGroupCreate();
    const esp_mqtt_client_config_t mqtt_cfg = {
        .event_handle = mqtt_event_handler,
        .host = MQTT_BROKER_URL,
        .port = MQTT_BROKER_PORT,
        .username = MQTT_USER,
        .password = MQTT_PASSWD
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    // xEventGroupClearBits(mqtt_event_group, MQTT_CONNECTED_BIT | MQTT_ERROR_BIT);

    // ESP_LOGI(TAG, "set_uri");
    // esp_mqtt_client_set_uri(mqtt_client, MQTT_BROKER_URL);

    ESP_LOGD(TAG, "esp_mqtt_client_start");
    esp_mqtt_client_start(mqtt_client);
    ESP_LOGD(TAG, "Note free memory: %d bytes", esp_get_free_heap_size());
    EventBits_t bits =  xEventGroupWaitBits(mqtt_event_group, MQTT_CONNECTED_BIT | MQTT_ERROR_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    bool result = false;
    if (bits & MQTT_CONNECTED_BIT) {
        result = true;
    }

    return result;
}

void mqtt_stop_app(void){
    esp_mqtt_client_stop(mqtt_client);
}

bool mqtt_app_send(const uint16_t value){
    std::string payload = generate_payload(UBIDOTS_VARIABLE_LABEL, value);
    ESP_LOGD(TAG, payload.c_str());

    int result = esp_mqtt_client_publish(mqtt_client, UBIDOTS_MQTT_TOPIC, payload.c_str(), payload.size(), 0, 0);

    return result != -1;
}

std::string generate_payload(const std::string variable, const uint16_t value){
    // TODO: use JSON library

    std::string payload = "{\"";
    payload.append(variable);
    payload.append("\": {\"value\": ");
    payload.append(std::to_string(value));
    payload.append("}}");
    return payload;
}

esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event){
    switch (event->event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGD(TAG, "MQTT_EVENT_CONNECTED");
        xEventGroupSetBits(mqtt_event_group, MQTT_CONNECTED_BIT);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGD(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGW(TAG, "MQTT_EVENT_ERROR");
        xEventGroupSetBits(mqtt_event_group, MQTT_ERROR_BIT);
        break;
    case MQTT_EVENT_BEFORE_CONNECT:
        break;
    default:
        ESP_LOGW(TAG, "Other event id:%d", event->event_id);
        // xEventGroupSetBits(mqtt_event_group, MQTT_ERROR_BIT);
        break;
    }
    return ESP_OK;
}
