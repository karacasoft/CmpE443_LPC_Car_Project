/*
 * timer.h
 *
 *  Created on: Nov 25, 2017
 *      Author: karacasoft
 */

#ifndef DEV_TIMER_H_
#define DEV_TIMER_H_

#include <stdint.h>

#define TIMER1_BASE 0x40008000
#define TIMER1 ((timer_regs_t *) TIMER1_BASE)

typedef struct {
	volatile uint32_t IR;
	volatile uint32_t TCR;
	volatile uint32_t TC;
	volatile uint32_t PR;
	volatile uint32_t PC;
	volatile uint32_t MCR;
	volatile uint32_t MR0;
	volatile uint32_t MR1;
	volatile uint32_t MR2;
	volatile uint32_t MR3;
	volatile uint32_t CCR;
	volatile uint32_t CR0;
	volatile uint32_t CR1;
			 uint32_t RESERVED0[2];
	volatile uint32_t EMR;
			 uint32_t RESERVED1[12];
	volatile uint32_t CTCR;
} timer_regs_t;

typedef void (*timer_callback_t)(uint32_t time);

extern void init_timer();

extern uint8_t set_timeout(uint32_t millis, timer_callback_t callback);
extern uint8_t set_interval(uint32_t millis, timer_callback_t callback);
extern void clear_interval(uint8_t id);

extern void sleep(uint32_t millis);


#endif /* DEV_TIMER_H_ */
