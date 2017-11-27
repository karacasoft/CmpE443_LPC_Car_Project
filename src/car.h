/*
 * car.h
 *
 *  Created on: Nov 26, 2017
 *      Author: karacasoft
 */

#ifndef CAR_H_
#define CAR_H_

#include <stdint.h>

#define CAR_STATE_EVADE_OBSTACLE 0
#define CAR_STATE_FOLLOW_LIGHT 1
#define CAR_STATE_GO_FORWARD 2
#define CAR_STATE_STOP 3

extern uint8_t state;

extern void car_init();
extern void car_run();

#endif /* CAR_H_ */
