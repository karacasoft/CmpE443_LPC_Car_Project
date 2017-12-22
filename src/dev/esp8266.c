/*
 * esp8266.c
 *
 *  Created on: Dec 21, 2017
 *      Author: karacasoft
 */

#include "esp8266.h"

#include "iocon.h"
#include "../gpio/gpio.h"
#include "device.h"
#include "uart.h"
#include "timer.h"

esp_context_t esp;

uint8_t waitingReceive;


uint8_t resetPort = 1;
uint8_t resetPin = 5;

uint8_t chdpPort = 1;
uint8_t chdpPin = 6;

void uart3callback(char *response);

void ap_list_parser(char *responseLine);


void uart3callback(char *response) {
	if(esp.onResponseListener) {
		esp.onResponseListener(response);
	}
}

esp_context_t *esp8266_get_context() {
	esp.uart_context = uart_getUartContext(3);
	return &esp;
}

void esp8266_init(esp_context_t *context) {
	USING_PERIPHERAL(PR_UART3);
	
	uart_init(context->uart_context);
	uart_setBaudRate(context->uart_context, 115200);
	
	esp.onResponseListener = 0;
	
	SET_PIN_USED(resetPort, resetPin);
	SET_PIN_USED(chdpPort, chdpPin);
	
	IOCON_PORTS[resetPort][resetPin].fields.FUNC = 0;
	IOCON_PORTS[chdpPort][chdpPin].fields.FUNC = 0;
	
	pinMode(resetPort, resetPin, OUTPUT);
	pinMode(chdpPort, chdpPin, OUTPUT);
	
	writePin(resetPort, resetPin, HIGH);
	writePin(chdpPort, chdpPin, HIGH);
}

void esp8266_reset(esp_context_t *context) {
	writePin(resetPort, resetPin, LOW);
	sleep(15);
	writePin(resetPort, resetPin, HIGH);
}

void esp8266_setOnResponseListener(esp_context_t *context, void (*onResponseListener)(char *)) {
	uart_bufferUntil(context->uart_context, '\n');
	context->onResponseListener = onResponseListener;
	
	uart_setBufferCompleteCallback(context->uart_context, uart3callback);
}

// LOW LEVEL COMMANDS TO CONTACT WITH ESP
void esp8266_send(esp_context_t *context, char *command, int len) {
	uart_sendString(context->uart_context, command, len);
}

void esp8266_waitReceive(esp_context_t *context) {
	waitingReceive = 1;
	while(waitingReceive) {
		__WFI();
	}
}
