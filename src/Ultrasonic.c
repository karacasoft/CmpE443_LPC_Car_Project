#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include "Ultrasonic.h"
#include "dev/iocon.h"
#include "dev/device.h"

void Ultrasonic_Trigger_Timer_Init() {
	//Give the Correct Function Values to IOCON_TRIGGER
	IOCON_P0[9].fields.FUNC = 3;
	//Enable Timer2.
	USING_PERIPHERAL(PR_TIMER2);
	//This code changes the mode of Timer2 to Timer Mode.
	TIMER2->CTCR = 0x00;
	
	//This code disables Timer Counter and Prescale Counter for Timer2.
	TIMER2->TCR &= ~(1 << 0);
	
	//This code resets Timer Counter and Prescale Counter for Timer2.
	TIMER2->TCR |= (1 << 1);
	
	//Change PR Register value for 1 microsecond incrementing
	TIMER2->PR = 60;
	//Write the Correct Configuration for EMR (Toggle Output Value and Initial value is HIGH)
	TIMER2->EMR |= (1 << 3 | 3 << 10);
	//Enable TIMER2_IRQn (Interrupt Request).
	NVIC_EnableIRQ(TIMER2_IRQn);
	//Set Priority Timer2 IRQ as 5.
	NVIC_SetPriority(TIMER2_IRQn, 5);
	//Clear pendings for Timer2.
	NVIC_ClearPendingIRQ(TIMER2_IRQn);
}

void Ultrasonic_Capture_Timer_Init() {
	//Give the Correct Function Values to IOCON_ECHO
	IOCON_P0[23].fields.FUNC = 3;
	//Enable Timer3
	USING_PERIPHERAL(PR_TIMER3);
	//This code changes the mode of Timer3 to Timer Mode.
	TIMER3->CTCR = 0x00;
	
	//This code disables Timer Counter and Prescale Counter for Timer3.
	TIMER3->TCR &= ~(1 << 0);
	
	//This code resets Timer Counter and Prescale Counter for Timer3.
	TIMER3->TCR |= (1 << 1);
	
	//Change PR Register value for 1 microsecond incrementing
	TIMER3->PR = 60;
	//Write the Correct Value for Getting Interrupt when Rising Edge Occur
	TIMER3->CCR = (1<<0 | 1<<2);
	//Remove the reset on counters of Timer3.
	TIMER3->TCR &= ~(1 << 1);
	//Enable Timer3 Counter and Prescale Counter for counting.
	TIMER3->TCR |= (1 << 0);
}

void Ulrasonic_Start_Trigger() {
	//Give correct value to MR3 Register for 10 microsecond
	TIMER2->MR3 = 10;
	//Enable interrupt for MR3 register, if MR3 register matches the TC.
	TIMER2->MCR |= (1<<9);
	//Remove the reset on counters of Timer2.
	TIMER2->TCR &= ~(1 << 1);
	//Enable Timer2 Counter and Prescale Counter for counting.
	TIMER2->TCR |= (1 << 0);
}

uint8_t isUltrasonicSensorTriggerEnded = 0;
uint8_t ultrasonicSensorEdgeCount = 0;

void TIMER2_IRQHandler() {
	if(isUltrasonicSensorTriggerEnded == 0) {
		//Change MR3 Register Value for Suggested Waiting
		TIMER2->MR3 = 60000;
		
		isUltrasonicSensorTriggerEnded = 1;
		
		ultrasonicSensorEdgeCount = 0;
		
		//Clear pendings for Timer3.
		NVIC_ClearPendingIRQ(TIMER3_IRQn);
		
		//Enable TIMER3_IRQn (Interrupt Request).
		NVIC_EnableIRQ(TIMER3_IRQn);
	}
	else {
		TIMER2->MR3 = 10;
		isUltrasonicSensorTriggerEnded = 0;
	}
	
	//Clear IR Register Flag for Corresponding Interrupt
	TIMER2->IR = (1<<3);
	
	TIMER2->TC = 0;
}

uint32_t ultrasonicSensorRisingTime = 0;
uint32_t ultrasonicSensorFallingTime = 0;
uint32_t ultrasonicSensorDuration = 0;
uint32_t ultrasonicSensorDistance = 0;

void TIMER3_IRQHandler() {
	if(ultrasonicSensorEdgeCount == 0) {
		
		//Store the rising time into ultrasonicSensorRisingTime variable
		ultrasonicSensorRisingTime = TIMER3->CR0;
		
		TIMER3->CCR = (1 << 1) | (1 << 2);
		ultrasonicSensorEdgeCount = 1;
	}
	else if(ultrasonicSensorEdgeCount == 1){
		
		//Store the falling time into ultrasonicSensorFallingTime variable
		ultrasonicSensorFallingTime = TIMER3->CR0;
		
		TIMER3->CCR = (1 << 0) | (1 << 2);
		
		ultrasonicSensorEdgeCount = 2;
		
		ultrasonicSensorDistance = (ultrasonicSensorFallingTime - ultrasonicSensorRisingTime) / 58;

		//Clear pendings for Timer3.
		NVIC_ClearPendingIRQ(TIMER3_IRQn);
		//Disable TIMER3_IRQn (Interrupt Request).
		NVIC_DisableIRQ(TIMER3_IRQn);
	}
	
	TIMER3->IR = 1 << 4;
}

