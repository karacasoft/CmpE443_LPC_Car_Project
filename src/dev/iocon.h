#ifndef __IOCON_H__
#define __IOCON_H__

#include <stdint.h>

#define IOCON_BASE 0x4002C000

#define IOCON_P0 ((iocon_reg_t *) IOCON_BASE)
#define IOCON_P1 ((iocon_reg_t *) (IOCON_BASE + 0x80))
#define IOCON_P2 ((iocon_reg_t *) (IOCON_BASE + 0x100))
#define IOCON_P3 ((iocon_reg_t *) (IOCON_BASE + 0x180))
#define IOCON_P4 ((iocon_reg_t *) (IOCON_BASE + 0x200))
#define IOCON_P5 ((iocon_reg_t *) (IOCON_BASE + 0x280))

typedef union {
	struct {
		volatile uint32_t FUNC : 3;
		volatile uint32_t MODE : 2;
		volatile uint32_t HYS : 1;
		volatile uint32_t INV : 1;
		volatile uint32_t ADMODE : 1;
		volatile uint32_t FILTER : 1;
		volatile uint32_t SLEW : 1;
		volatile uint32_t OD : 1;
				 uint32_t RESERVED1 : 21;
	} fields;
	volatile uint32_t value;
} iocon_reg_t;

extern iocon_reg_t *IOCON_PORTS[6];

#endif
