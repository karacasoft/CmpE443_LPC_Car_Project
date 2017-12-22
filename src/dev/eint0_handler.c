/*
 * eint0_handler.c
 *
 *  Created on: Dec 22, 2017
 *      Author: bahadir
 */
#include "interrupt.h"


void EINT0_IRQHandler() {
	EXTINT |= 8;
}
