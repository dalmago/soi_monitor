#ifndef __SOIL_MONITOR_SENSORS_H__
#define __SOIL_MONITOR_SENSORS_H__

#include <vector>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/adc.h"

#define SENSOR1_GPIO GPIO_NUM_4
#define SENSOR2_GPIO GPIO_NUM_5

void sensors_init();
bool sensor_read(std::vector<uint16_t> &sensor_data);
void sensors_cleanup();

#endif // __SOIL_MONITOR_SENSORS_H__
