/*
 * uart.c
 *
 *  Created on: Dec 20, 2017
 *      Author: karacasoft
 */
#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include "device.h"
#include "uart.h"
#include "iocon.h"

#include <stdint.h>


void uart_setBaudRate(uart_context_t *context, int rate);
void uart_onSend(uart_context_t *context);


uart_context_t uart_context[5] = {0};

uint32_t getUartAddress(int index) {
	switch(index) {
	case 0:
		return UART0_ADDRESS;
	case 1:
		return UART1_ADDRESS;
	case 2:
		return UART2_ADDRESS;
	case 3:
		return UART3_ADDRESS;
	case 4:
		return UART4_ADDRESS;
	default:
		return 0;
	}
}

void enableUartPins(int index) {
	switch(index) {
	case 0:
		SET_PIN_USED(0, 2);
		SET_PIN_USED(0, 3);
		
		IOCON_PORTS[0][2].fields.FUNC = 0x1;
		IOCON_PORTS[0][3].fields.FUNC = 0x1;
		
		//IOCON_PORTS[0][2].fields.MODE = 0x0;
		IOCON_PORTS[0][3].fields.MODE = 0x0;

		break;
	case 3:
		SET_PIN_USED(4, 29);
		SET_PIN_USED(4, 28);
		
		IOCON_PORTS[4][29].fields.FUNC = 0x2;
		IOCON_PORTS[4][28].fields.FUNC = 0x2;
		
		IOCON_PORTS[4][29].fields.MODE = 0x0;
		//IOCON_PORTS[4][28].fields.MODE = 0x0;
		
		break;
	default:
		break;
	}
}

void enableUartInterrupt(int index) {
	switch(index) {
		case 0:
			NVIC_EnableIRQ(UART0_IRQn);
			NVIC_SetPriority(UART0_IRQn, 9);
			break;
		case 1:
			NVIC_EnableIRQ(UART1_IRQn);
			NVIC_SetPriority(UART1_IRQn, 6);
			break;
		case 2:
			NVIC_EnableIRQ(UART2_IRQn);
			NVIC_SetPriority(UART2_IRQn, 7);
			break;
		case 3:
			NVIC_EnableIRQ(UART3_IRQn);
			NVIC_SetPriority(UART3_IRQn, 5);
			break;
		case 4:
			NVIC_EnableIRQ(UART4_IRQn);
			NVIC_SetPriority(UART4_IRQn, 9);
			break;
		default:
			break;
		}
}

uart_context_t *uart_getUartContext(int index) {
	uart_context_t context = uart_context[index];
	context.regs = ((uart_regs_t *) getUartAddress(index));
	context.index = index;
	uart_context[index] = context;
	return &uart_context[index];
}

void uart_init(uart_context_t *context) {
	context->regs->LCR = 0x3;
	context->regs->DLM_IER = 0x3;

	context->sendBufferPtr = context->sendBuffer;
	context->sendBufferWritePtr = context->sendBuffer;
	
	context->receiveBufferPtr = context->receiveBuffer;
	context->receiveBufferReadPtr = context->receiveBuffer;
	
	context->isSending = 0;
	
	context->bufferUntil = 0;
	context->onBufferComplete = 0;
	
	enableUartPins(context->index);
	
	enableUartInterrupt(context->index);
	
}

void uart_calculateBaudRateValues(int baudrate, uint16_t *dl, uint8_t *divAdd, uint8_t *mul) {
	uint32_t pclk = 60000000;
	
	uint16_t dlVal = pclk / (16 * baudrate);
	uint8_t divAddVal = 0;
	uint8_t mulVal = 1;
	
	// Reference: https://os.mbed.com/users/Baycken/code/mbed-src/raw-diff/3bc89ef62ce7/vendor/NXP/LPC4088/hal/serial_api.c/
	if(pclk % (16 * baudrate) != 0) {
		uint8_t dav;
		uint8_t mv;
		
		float error = baudrate;
		
		uint8_t hit = 0;
		
		uint16_t dlv;
		uint16_t dlMax = dlVal;
		for(dlv = dlMax / 2; dlv <= dlMax && !hit; dlv++) {
			for(mv = 1; mv <=15; mv++) {
				for(dav = 0; dav < mv; dav++) {
					float ratio = 1.0f + ((float) dav / (float) mv);
					float calculatedBaud = (pclk / (16 * dlv * ratio));
					float err = (((float) baudrate - calculatedBaud) / (float) baudrate);
					if(err < 0) err = -err;
					if(err < error) {
						dlVal = dlv;
						divAddVal = dav;
						mulVal = mv;
						error = err;
						if(err < 0.001f) {
							hit = 1;
						}
					}
				}
			}
		}
	}
	
	
	*dl = dlVal;
	*divAdd = divAddVal;
	*mul = mulVal;
	
}

void uart_setBaudRate(uart_context_t *context, int rate) {
	context->regs->IIR_FCR = 0x1;
	
	context->regs->LCR = 0x83;
	
	
	uint16_t dl;
	uint8_t divAddVal;
	uint8_t mulVal;
	
	uart_calculateBaudRateValues(rate, &dl, &divAddVal, &mulVal);
	
	context->regs->DLM_IER = (dl >> 8) & 0xFF;
	context->regs->RBR_THR_DLL = dl & 0xFF;
	context->regs->FDR = (mulVal << 4) | divAddVal;
	
	context->regs->LCR = 0x3;
}

void uart_send(uart_context_t *context, char ch) {
	*context->sendBufferWritePtr++ = ch;
	if(!context->isSending) {
		context->isSending = 1;
		uart_onSend(context);
	}
	if(context->sendBufferWritePtr - context->sendBuffer > UART_BUFFER_SIZE) {
		context->sendBufferWritePtr = context->sendBuffer;
	}
}

void uart_sendString(uart_context_t *context, char *string, int len) {
	int i;
	for (i = 0; i < len && string[i] != 0; ++i) {
		uart_send(context, string[i]);
	}
}

void uart_setBufferCompleteCallback(uart_context_t *context, void(*callback)(char *)) {
	context->onBufferComplete = callback;
}

void uart_bufferUntil(uart_context_t *context, char bufferChar) {
	context->bufferUntil = bufferChar;
}

void uart_onReceive(uart_context_t *context) {
	if(context->receiveBufferPtr - context->receiveBuffer > UART_BUFFER_SIZE) {
		context->receiveBufferPtr = context->receiveBuffer;
	}
	
	char receivedChar = context->regs->RBR_THR_DLL;
	
	if(context->onBufferComplete) {
		*context->receiveBufferPtr++ = receivedChar;
		
		if(context->bufferUntil) {
			if(receivedChar == context->bufferUntil) {
				char *recStringPtr = context->receivedString;
				while(context->receiveBufferReadPtr != context->receiveBufferPtr) {
					*recStringPtr++ = *context->receiveBufferReadPtr++;
					if(context->receiveBufferReadPtr - context->receiveBuffer > UART_BUFFER_SIZE) {
						context->receiveBufferReadPtr = context->receiveBuffer;
					}
				}
				*recStringPtr = 0;
				
				context->onBufferComplete(context->receivedString);
				
			}
		} else {
			*context->receivedString = *context->receiveBuffer;
			context->receivedString[1] = 0;
			context->onBufferComplete(context->receivedString);
			context->receiveBufferPtr = context->receiveBuffer;
		}
	}
}

void uart_onSend(uart_context_t *context) {
	if(context->sendBufferPtr != context->sendBufferWritePtr) {
		context->regs->RBR_THR_DLL = *context->sendBufferPtr++;
	} else {
		context->isSending = 0;
	}
	if(context->sendBufferPtr - context->sendBuffer > UART_BUFFER_SIZE) {
		context->sendBufferPtr = context->sendBuffer;
	}
}

void uart_int_handler(uart_context_t *context) {
	uint32_t intStatus = context->regs->IIR_FCR;
	uint8_t intId = (intStatus >> 1) & 0x7;
	if(intId == 0x02) {
		uart_onReceive(context);
	} else if(intId == 0x01) {
		uart_onSend(context);
	} else if(intId == 0x06) {
		uint8_t wow = intId;
	} else if(intId == 0x03){
		uint8_t wow = intId;
	}
}


void UART0_IRQHandler(void) {
	uart_context_t *context = uart_getUartContext(0); 
	uart_int_handler(context);
}

void UART1_IRQHandler(void) {
	uart_context_t *context = uart_getUartContext(1); 
	uart_int_handler(context);
}

void UART2_IRQHandler(void) {
	uart_context_t *context = uart_getUartContext(2); 
	uart_int_handler(context);
}

void UART3_IRQHandler(void) {
	uart_context_t *context = uart_getUartContext(3); 
	uart_int_handler(context);
}

void UART4_IRQHandler(void) {
	uart_context_t *context = uart_getUartContext(4); 
	uart_int_handler(context);
}
