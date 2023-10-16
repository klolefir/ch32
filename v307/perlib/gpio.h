#ifndef GPIO_H_SENTRY
#define GPIO_H_SENTRY

#include "ch32v30x.h"

typedef enum {
	gpio_port_a,
	gpio_port_b,
	gpio_port_c,
	gpio_port_d,
	gpio_port_e
} gpio_port_t;

typedef enum {
	gpio_pin_0 = 0,
	gpio_pin_1,
	gpio_pin_2,
	gpio_pin_3,
	gpio_pin_4,
	gpio_pin_5,
	gpio_pin_6,
	gpio_pin_7,
	gpio_pin_8,
	gpio_pin_9,
	gpio_pin_10,
	gpio_pin_11,
	gpio_pin_12,
	gpio_pin_13,
	gpio_pin_14,
	gpio_pin_15,
} gpio_pin_t;

typedef enum {
	gpio_mode_input		= 0b00,
	gpio_mode_out10mhz	= 0b01,
	gpio_mode_out2mhz	= 0b10,
	gpio_mode_out50mhz 	= 0b11
} gpio_mode_t;

typedef enum {
	gpio_iconfig_analog		= 0b00,
	gpio_iconfig_floating	= 0b01,
	gpio_iconfig_pupd		= 0b10
} gpio_iconfig_t;

typedef enum {
	gpio_oconfig_pp 	= 0b00,
	gpio_oconfig_od		= 0b01,
	gpio_oconfig_altpp	= 0b10,
	gpio_oconfig_altod	= 0b11
} gpio_oconfig_t;

typedef struct {
	gpio_port_t 	port	: 3;
	gpio_pin_t 		pin		: 4;
	gpio_mode_t 	mode 	: 2;
	union {
		gpio_iconfig_t iconfig	: 2;
		gpio_oconfig_t oconfig	: 2;
	};
} gpio_t;

void gpio_init(const gpio_t *gpio_set);
void gpio_set(const gpio_t *gpio_set);
void gpio_clr(const gpio_t *gpio_set);
void gpio_tgl(const gpio_t *gpio_set);

#endif
