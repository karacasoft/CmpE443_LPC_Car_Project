#include "peripherals.h"

uint8_t is_peripheral_active(uint32_t peripheral) {
	return (PCONP & peripheral);
}

void enable_peripheral(uint32_t peripheral) {
	PCONP |= peripheral;
}

 void disable_peripheral(uint32_t peripheral) {
	PCONP &= ~(peripheral);
}
