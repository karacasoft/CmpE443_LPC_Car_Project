#ifndef __DEV_RGB_LED_H__
#define __DEV_RGB_LED_H__

#include "device.h"

#define RGB_LED_COMMAND_SET_RED_VALUE     0
#define RGB_LED_COMMAND_SET_GREEN_VALUE   1
#define RGB_LED_COMMAND_SET_BLUE_VALUE    2
#define RGB_LED_COMMAND_SET_RGB           3

device_info_t *getRGBLed(void);

#endif
