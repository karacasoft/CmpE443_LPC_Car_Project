/*
 * gpio.h
 *
 *  Created on: Nov 24, 2017
 *      Author: karacasoft
 */

#ifndef GPIO_GPIO_H_
#define GPIO_GPIO_H_

#include <stdint.h>

#define INPUT 0
#define OUTPUT 1

#define LOW INPUT
#define HIGH 1

#define GPIO_BASE 0x20098000
#define PORT0_BASE (GPIO_BASE)
#define PORT1_BASE (GPIO_BASE + 0x20)
#define PORT2_BASE (GPIO_BASE + 0x40)
#define PORT3_BASE (GPIO_BASE + 0x60)
#define PORT4_BASE (GPIO_BASE + 0x80)
#define PORT5_BASE (GPIO_BASE + 0xA0)

typedef struct {
	volatile uint32_t DIR;
			 uint32_t RESERVED0[3];
	volatile uint32_t MASK;
	volatile uint32_t PIN;
	volatile uint32_t SET;
	volatile uint32_t CLR;
} gpio_t;

#define PORT0 ((gpio_t *) PORT0_BASE)
#define PORT1 ((gpio_t *) PORT1_BASE)
#define PORT2 ((gpio_t *) PORT2_BASE)
#define PORT3 ((gpio_t *) PORT3_BASE)
#define PORT4 ((gpio_t *) PORT4_BASE)
#define PORT5 ((gpio_t *) PORT5_BASE)

extern gpio_t *ports[6];

static inline void pinMode(uint8_t port, uint8_t pin, uint8_t mode) {

	if(port > 5) {
		// TODO dude? die pls.
		return;
	}
	gpio_t *theport = ports[port];

	if(mode == INPUT) {
		theport->DIR &= ~(1 << pin);
	} else if(mode == OUTPUT) {
		theport->DIR |= (1 << pin);
	}
}

static inline void writePin(uint8_t port, uint8_t pin, uint8_t val) {
	if(port > 5) {
		// TODO dude? die pls.
		return;
	}
	gpio_t *theport = ports[port];

	if(val == LOW) {
		theport->PIN &= ~(1 << pin);
	} else if(val == HIGH) {
		theport->PIN |= (1 << pin);
	}
}

static inline uint8_t readPin(uint8_t port, uint8_t pin) {
	if(port > 5) {
		// TODO dude? die pls.
		return 0;
	}
	gpio_t *theport = ports[port];

	return !(!(theport->PIN & (1 << pin)));

}

#endif /* GPIO_GPIO_H_ */
