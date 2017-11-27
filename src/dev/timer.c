/*
 * timer.c
 *
 *  Created on: Nov 26, 2017
 *      Author: karacasoft
 */
#include "chip.h"

#include <stdint.h>
#include "timer.h"

#define FLAG_IS_FULL 0x01
#define FLAG_REPEAT 0x02

uint8_t is_sleeping = 0;

void unsleep() {
	is_sleeping = 0;
}

struct {
	uint32_t start_time;
	uint32_t time;
	timer_callback_t callback;
	uint8_t flags;
} timer_blocks[20];

void TIMER1_IRQHandler(void) {
	int i = 0;
	for(i = 0; i < 20; i++) {
		if((timer_blocks[i].flags & FLAG_IS_FULL) && !(--timer_blocks[i].time)) {
			timer_blocks[i].callback(timer_blocks[i].start_time);
			if(timer_blocks[i].flags & FLAG_REPEAT) {
				timer_blocks[i].time = timer_blocks[i].start_time;
			} else {
				timer_blocks[i].flags = 0;
			}
		}
	}
	TIMER1->IR = 0x1;
}

void init_timer() {
	TIMER1->IR = TIMER1->IR;
	TIMER1->TCR = 0x3;
	TIMER1->MCR = 0x3;
	TIMER1->PR = 60;

	TIMER1->MR0 = 1000; // microsec
	TIMER1->CCR = 0;
	TIMER1->EMR = 0;
	TIMER1->CTCR = 0;

	TIMER1->TCR = 0x1;

	NVIC_EnableIRQ(TIMER1_IRQn);
}

uint8_t set_timeout(uint32_t millis, timer_callback_t callback) {

	int i = 0 - 1;
	while(i < 20 && timer_blocks[++i].flags & FLAG_IS_FULL);

	timer_blocks[i].start_time = millis;
	timer_blocks[i].time = millis;
	timer_blocks[i].callback = callback;

	timer_blocks[i].flags = FLAG_IS_FULL;

	return i;
}

uint8_t set_interval(uint32_t millis, timer_callback_t callback) {
	int i = 0 - 1;
	while(i < 20 && timer_blocks[++i].flags & FLAG_IS_FULL);

	timer_blocks[i].start_time = millis;
	timer_blocks[i].time = millis;
	timer_blocks[i].callback = callback;

	timer_blocks[i].flags = FLAG_IS_FULL | FLAG_REPEAT;

	return i;
}

void clear_interval(uint8_t id) {
	timer_blocks[id].flags = 0;
}

void sleep(uint32_t millis) {
	is_sleeping = 1;
	set_timeout(millis, unsleep);
	while(is_sleeping) {
		__WFI();
	}
}
