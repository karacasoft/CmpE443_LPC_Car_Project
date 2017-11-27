#ifndef __PERIPHERALS_H__
#define __PERIPHERALS_H__

#include <stdint.h>

#define PCONP_BASE 0x400FC0C4
#define PCONP *((volatile uint32_t *) PCONP_BASE)

#define PC_LCD_BIT                       0x1         // 0
#define PC_TIMER0_BIT                    0x2         // 1
#define PC_TIMER1_BIT                    0x4         // 1
#define PC_UART0_BIT                     0x8         // 1
#define PC_UART1_BIT                     0x10        // 1
#define PC_PWM0_BIT                      0x20        // 0
#define PC_PWM1_BIT                      0x40        // 0
#define PC_I2C0_BIT                      0x80        // 1
#define PC_UART4_BIT                     0x100       // 0
#define PC_RTC_BIT                       0x200       // 1
#define PC_SSP_BIT                       0x400       // 0
#define PC_EMC_BIT                       0x800       // 0
#define PC_ADC_BIT                       0x1000       // 0
#define PC_CAN1_BIT                      0x2000      // 0
#define PC_CAN2_BIT                      0x4000      // 0
#define PC_GPIO_BIT                      0x8000      // 1
#define PC_SPIFI_BIT                     0x10000     // 0
#define PC_MCPWM_BIT                     0x20000     // 0
#define PC_QEI_BIT                       0x40000     // 0
#define PC_I2C1_BIT                      0x80000     // 1
#define PC_SSP2_BIT                      0x100000    // 0
#define PC_SSP0_BIT                      0x200000    // 0
#define PC_TIMER2_BIT                    0x400000    // 0
#define PC_TIMER3_BIT                    0x800000    // 0
#define PC_UART2_BIT                     0x1000000   // 0
#define PC_UART3_BIT                     0x2000000   // 0
#define PC_I2C2_BIT                      0x4000000   // 1
#define PC_I2S_BIT                       0x8000000   // 0
#define PC_SDCARD_BIT                    0x10000000  // 0
#define PC_GPDMA_BIT                     0x20000000  // 0
#define PC_ETHERNET_BIT                  0x40000000  // 0
#define PC_USB_BIT                       0x80000000  // 0

extern uint8_t is_peripheral_active(uint32_t peripheral);

extern void enable_peripheral(uint32_t peripheral);

extern void disable_peripheral(uint32_t peripheral);


#endif
