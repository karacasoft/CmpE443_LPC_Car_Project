/*
 * board_leds.h
 *
 *  Created on: Dec 20, 2017
 *      Author: bahadir
 */

#ifndef DEV_BOARD_LEDS_H_
#define DEV_BOARD_LEDS_H_

#define LED1_PORT			1
#define LED1_PIN			18

#define LED2_PORT			0
#define LED2_PIN			13

#define LED3_PORT			1
#define LED3_PIN			13

#define LED4_PORT			2
#define LED4_PIN			19

void init_board_leds(void);
void leds_on(void);
void leds_off(void);

void LED1_Off(void);
void LED2_Off(void);
void LED3_Off(void);
void LED4_Off(void);

void LED1_On(void);
void LED2_On(void);
void LED3_On(void);
void LED4_On(void);

#endif /* DEV_BOARD_LEDS_H_ */
