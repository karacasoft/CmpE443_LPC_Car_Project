/*
 * interrupt.h
 *
 *  Created on: Dec 22, 2017
 *      Author: bahadir
 */

#ifndef DEV_INTERRUPT_H_
#define DEV_INTERRUPT_H_

#include <stdint.h>

#define EXTINT_BASE 0x400FC140
#define EXTINT *((volatile uint32_t*)(EXTINT_BASE))

#define EXTMODE_BASE 0x400FC148
#define EXTMODE *((volatile uint32_t*)(EXTMODE_BASE))

#define EXTPOLAR_BASE 0x400FC14C
#define EXTPOLAR *((volatile uint32_t*)(EXTPOLAR))


#endif /* DEV_INTERRUPT_H_ */
