#include "sensors.h"

void sensors_init(){
    // Set GPIOs as outputs
    gpio_config_t io_conf = {
        .pin_bit_mask = BIT(SENSOR1_GPIO) | BIT(SENSOR2_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    // initialize as 0
    gpio_config(&io_conf);
    gpio_set_level(SENSOR1_GPIO, 0);
    gpio_set_level(SENSOR2_GPIO, 0);

    // init adc
    adc_config_t adc_config = {
        .mode = ADC_READ_TOUT_MODE,
        .clk_div = 8 // ADC sample collection clock = 80MHz/clk_div = 10MHz
    };
    ESP_ERROR_CHECK(adc_init(&adc_config));
}

bool sensor_read(std::vector<uint16_t> &sensor_data){
    uint16_t adc_data;

    gpio_set_level(SENSOR1_GPIO, 1); // activate sensor 1
    vTaskDelay(500 / portTICK_RATE_MS); // wait 500ms

    if (adc_read(&adc_data) != ESP_OK){
        gpio_set_level(SENSOR1_GPIO, 0);
        return false;
    }

    sensor_data.push_back(adc_data);

    gpio_set_level(SENSOR1_GPIO, 0);
    // vTaskDelay(500 / portTICK_RATE_MS); // wait 500ms
    gpio_set_level(SENSOR2_GPIO, 1); // activate sensor 2
    vTaskDelay(500 / portTICK_RATE_MS); // wait 500ms

    if (adc_read(&adc_data) != ESP_OK){
        gpio_set_level(SENSOR1_GPIO, 0);
        return false;
    }

    sensor_data.push_back(adc_data);

    gpio_set_level(SENSOR2_GPIO, 0);
    return true;
}

void sensors_cleanup(){
    adc_deinit();
}
