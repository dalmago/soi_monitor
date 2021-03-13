#ifndef __SOIL_MONITOR_SENSORS_H__
#define __SOIL_MONITOR_SENSORS_H__

#include <driver/gpio.h>

#define SENSOR1_GPIO GPIO_NUM_4
#define SENSOR2_GPIO GPIO_NUM_5

void init_gpios();

#endif // __SOIL_MONITOR_SENSORS_H__
