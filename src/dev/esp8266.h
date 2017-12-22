/*
 * esp8266.h
 *
 *  Created on: Dec 21, 2017
 *      Author: karacasoft
 */

#ifndef __ESP8266_H__
#define __ESP8266_H__

#include <stdint.h>

#include "uart.h"

typedef struct {
	uart_context_t *uart_context;
	
	void (*onResponseListener)(char *);
} esp_context_t;

esp_context_t *esp8266_get_context();

void esp8266_init(esp_context_t *context);

void esp8266_setOnResponseListener(esp_context_t *context, void (*onResponseListener)(char *));

void esp8266_reset(esp_context_t *context);

// LOW LEVEL COMMANDS TO COMMUNICATE WITH ESP
void esp8266_send(esp_context_t *context, char *command, int length);




#endif /* __ESP8266_H__ */
