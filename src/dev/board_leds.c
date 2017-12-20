/*
 * board_leds.c
 *
 *  Created on: Dec 20, 2017
 *      Author: bahadir
 */

#include "board_leds.h"

void init_board_leds() {
	LED1_Init();
	LED2_Init();
	LED3_Init();
	LED4_Init();
}

void leds_on() {
	LED1_On();
	LED2_On();
	LED3_On();
	LED4_On();
}

void leds_off() {
	LED1_Off();
	LED2_Off();
	LED3_Off();
	LED4_Off();
}

void LED1_Init() {
	writePin(LED1_PORT, LED1_PIN, 1);
}

void LED2_Init() {
	writePin(LED2_PORT, LED2_PIN, 1);
}

void LED3_Init() {
	writePin(LED3_PORT, LED3_PIN, 1);
}

void LED4_Init() {
	writePin(LED4_PORT, LED4_PIN, 1);
}

void LED1_Off() {
	writePin(LED1_PORT, LED1_PIN, 1);
}

void LED2_Off() {
	writePin(LED2_PORT, LED2_PIN, 1);
}

void LED3_Off() {
	writePin(LED3_PORT, LED3_PIN, 0);
}

void LED4_Off() {
	writePin(LED4_PORT, LED4_PIN, 0);
}

void LED1_On() {
	writePin(LED1_PORT, LED1_PIN, 0);
}

void LED2_On() {
	writePin(LED2_PORT, LED2_PIN, 0);
}

void LED3_On() {
	writePin(LED3_PORT, LED3_PIN, 1);
}

void LED4_On() {
	writePin(LED4_PORT, LED4_PIN, 1);
}



