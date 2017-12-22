/*
 * uart.h
 *
 *  Created on: Dec 20, 2017
 *      Author: karacasoft
 */

#ifndef DEV_UART_H_
#define DEV_UART_H_

#include <stdint.h>

#define UART_BUFFER_SIZE 2048

#define UART0_ADDRESS 0x4000C000
#define UART1_ADDRESS 0x40010000
#define UART2_ADDRESS 0x40088000
#define UART3_ADDRESS 0x4009C000
#define UART4_ADDRESS 0x400A4000

typedef struct {
	volatile uint32_t RBR_THR_DLL;
	volatile uint32_t DLM_IER;
	volatile uint32_t IIR_FCR;
	volatile uint32_t LCR;
	         uint32_t RESERVED0;
	volatile uint32_t LSR;
	         uint32_t RESERVED1;
	volatile uint32_t SCR_reg;
	volatile uint32_t ACR;
	         uint32_t RESERVED2;
	volatile uint32_t FDR;
	         uint32_t RESERVED3;
	volatile uint32_t TER;
			 uint32_t RESERVED4[6];
	volatile uint32_t RS485CTRL;
	volatile uint32_t RS485ADMATCH;
	volatile uint32_t RS485DLY;
} uart_regs_t;

typedef struct {
	uint8_t index;
	char receivedString[UART_BUFFER_SIZE];
	
	char sendBuffer[UART_BUFFER_SIZE];
	char receiveBuffer[UART_BUFFER_SIZE];

	uint8_t isSending;
	
	char *sendBufferWritePtr;
	char *sendBufferPtr;
	char *receiveBufferReadPtr;
	char *receiveBufferPtr;
	
	uart_regs_t *regs;

	char bufferUntil;
	void (*onBufferComplete)(char *);

} uart_context_t;

uart_context_t *uart_getUartContext(int index);

void uart_init(uart_context_t *context);
void uart_setBaudRate(uart_context_t *context, int rate);
void uart_send(uart_context_t *context, char ch);
void uart_sendString(uart_context_t *context, char *string, int len);

void uart_setBufferCompleteCallback(uart_context_t *context, void(*callback)(char *));
void uart_bufferUntil(uart_context_t *context, char bufferChar);

#endif /* DEV_UART_H_ */
