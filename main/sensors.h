#ifndef __SOIL_MONITOR_SENSORS_H__
#define __SOIL_MONITOR_SENSORS_H__

#include <vector>
#include <driver/gpio.h>

#define SENSOR1_GPIO GPIO_NUM_4
#define SENSOR2_GPIO GPIO_NUM_5

void init_gpios();
bool sensor_read(std::vector<uint8_t> &sensor_data);

#endif // __SOIL_MONITOR_SENSORS_H__
