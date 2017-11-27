#ifndef ULTRASONIC_H
#define ULTRASONIC_H

//#include "LPC407x_8x_177x_8x.h"

#include "ultrasonictimer.h"
#include "gpio/gpio.h"

//Write IOCON Register Address of Trigger Pin.
#define IOCON_TRIGGER_ADDRESS	0x4002C024 //P0.9
#define IOCON_TRIGGER	*((volatile uint32_t*)(IOCON_TRIGGER_ADDRESS))

//Write IOCON Register Address of Echo Pin.
#define IOCON_ECHO_ADDRESS	0x4002C05C //P0.23
#define IOCON_ECHO	*((volatile uint32_t*)(IOCON_ECHO_ADDRESS))
	
void Ultrasonic_Trigger_Timer_Init(void);
void Ultrasonic_Capture_Timer_Init(void);
void Ultrasonic_Start_Trigger(void);

extern uint8_t isUltrasonicSensorTriggerEnded;
extern uint8_t ultrasonicSensorEdgeCount;

extern uint32_t ultrasonicSensorRisingTime;
extern uint32_t ultrasonicSensorFallingTime;
extern uint32_t ultrasonicSensorDuration;
extern uint32_t ultrasonicSensorDistance;

#endif
