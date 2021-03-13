#include "sensors.h"

void init_gpios(){
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
}

bool sensor_read(std::vector<uint8_t> &sensor_data){
    sensor_data.push_back(255);
    sensor_data.push_back(128);
    return true;
}
