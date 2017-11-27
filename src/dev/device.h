#ifndef __DEV_DEVICE_H__
#define __DEV_DEVICE_H__

#include <stdint.h>
#include "../peripherals/peripherals.h"

// pin usage macros
#define IS_PIN_AVAILABLE(port, pin) (!(pin_usage_bitmap[port] & (1 << pin)))
#define SET_PIN_USED(port, pin) if(IS_PIN_AVAILABLE(port, pin)) (pin_usage_bitmap[port] |= (1 << pin)); else pin_error_state()
#define SET_PIN_CLEARED(port, pin) (pin_usage_bitmap[port] &= ~(1 << pin))


#define PR_LCD                           0x0         // 0
#define PR_TIMER0                        0x1         // 1
#define PR_TIMER1                        0x2         // 1
#define PR_UART0                         0x3         // 1
#define PR_UART1                         0x4         // 1
#define PR_PWM0                          0x5         // 0
#define PR_PWM1                          0x6         // 0
#define PR_I2C0                          0x7         // 1
#define PR_UART4                         0x8         // 0
#define PR_RTC                           0x9         // 1
#define PR_SSP                           0xA         // 0
#define PR_EMC                           0xB         // 0
#define PR_ADC                           0xC         // 0
#define PR_CAN1                          0xD         // 0
#define PR_CAN2                          0xE         // 0
#define PR_GPIO                          0xF         // 1
#define PR_SPIFI                         0x10        // 0
#define PR_MCPWM                         0x11        // 0
#define PR_QEI                           0x12        // 0
#define PR_I2C1                          0x13        // 1
#define PR_SSP2                          0x14        // 0
#define PR_SSP0                          0x15        // 0
#define PR_TIMER2                        0x16        // 0
#define PR_TIMER3                        0x17        // 0
#define PR_UART2                         0x18        // 0
#define PR_UART3                         0x19        // 0
#define PR_I2C2                          0x1A        // 1
#define PR_I2S                           0x1B        // 0
#define PR_SDCARD                        0x1C        // 0
#define PR_GPDMA                         0x1D        // 0
#define PR_ETHERNET                      0x1E        // 0
#define PR_USB                           0x1F        // 0

#define USING_PERIPHERAL(pr) if(!(peripheral_usage[pr]++)) enable_peripheral(1 << pr)
#define END_USING_PERIPHERAL(pr) if(!(--peripheral_usage[pr])) disable_peripheral(1 << pr)



#define MAX_DEVICES 25
#define GET_DEVICE(info_ptr) (&(devices[(info_ptr)->system_device_index]))

#define DEVICE_STATE_AVAILABLE           0x01
#define DEVICE_STATE_INITIALIZED         0x02
#define DEVICE_STATE_WAITING             0x04
#define DEVICE_STATE_ERROR               0x08
// Other device state bits are reserved for future use.

#define IS_DEVICE_AVAILABLE(info_ptr)   ((info_ptr)->state & DEVICE_STATE_AVAILABLE)
#define IS_DEVICE_INITIALIZED(info_ptr) ((info_ptr)->state & DEVICE_STATE_INITIALIZED)


typedef struct {
	void (*execute)(uint8_t, ...);
} command_t;

typedef struct {
	uint32_t state;

	void (*init)(void);
	void (*start)(void);
	void (*end)(void);

	command_t *commands;

	uint8_t system_device_index;
} device_info_t;

typedef struct {
	uint8_t dependency_count;
	device_info_t *dependency_list[10];

	uint8_t depending_device_count;
	uint8_t started_by_user;

	device_info_t *info;

} device_t;

extern uint32_t pin_usage_bitmap[6];
extern uint8_t peripheral_usage[32];

extern void pin_error_state();

extern uint8_t register_device(device_info_t *device);

extern void add_dependency(device_info_t *device, device_info_t *dependency);

extern void start_device(device_info_t *device);
extern void stop_device(device_info_t *device);

#endif
