#include "gpio.h"
#include "rcc.h"

static GPIO_TypeDef *switch_port(const gpio_t *gpio_set);
static rcc_periph_id_t switch_rcc_id(const gpio_t *gpio_set);
static void gpio_rcc_init(const gpio_t *gpio_set);

void gpio_init(const gpio_t *gpio_set)
{
	volatile uint32_t *cfgr;
	uint8_t offset;
	gpio_pin_t pin = gpio_set->pin;
	GPIO_TypeDef *gpio = switch_port(gpio_set);;
	if(!gpio)
		return;

	gpio_rcc_init(gpio_set);

	if(pin <= gpio_pin_7)
		cfgr = &gpio->CFGLR;
	else if(pin <= gpio_pin_15)
		cfgr = &gpio->CFGHR;
	else
		return;

	offset = pin * 4;
	*cfgr &= ~(0b11 << offset);
	*cfgr |= (gpio_set->mode << offset);

	switch(gpio_set->mode) {
	case gpio_mode_input:		*cfgr |= (gpio_set->iconfig << (2 + pin * 4));
								break;
	case gpio_mode_out2mhz:
	case gpio_mode_out10mhz:
	case gpio_mode_out50mhz:	offset = 2 + pin * 4;
								*cfgr &= ~(0b11 << offset);
								*cfgr |= (gpio_set->oconfig << offset);
								break;
	}
}

void gpio_set(const gpio_t *gpio_set)
{
	GPIO_TypeDef *gpio = switch_port(gpio_set);
	if(!gpio)
		return;
	if(gpio_set->mode != gpio_mode_input)
		gpio->BSHR = (1 << gpio_set->pin);
}

void gpio_clr(const gpio_t *gpio_set)
{
	GPIO_TypeDef *gpio = switch_port(gpio_set);
	if(!gpio)
		return;
	if(gpio_set->mode != gpio_mode_input)
		gpio->BSHR = (1 << (16 + gpio_set->pin));
}

void gpio_tgl(const gpio_t *gpio_set)
{
	GPIO_TypeDef *gpio = switch_port(gpio_set);
	if(!gpio)
		return;
	if(gpio_set->mode != gpio_mode_input)
		gpio->OUTDR ^= (1 << gpio_set->pin);
}

GPIO_TypeDef *switch_port(const gpio_t *gpio_set)
{
	switch(gpio_set->port) {
	case gpio_port_a:	return GPIOA;
	case gpio_port_b:	return GPIOB;
	case gpio_port_c:	return GPIOC;
	case gpio_port_d:	return GPIOD;
	case gpio_port_e:	return GPIOE;
	default:			return 0;
	}
}

rcc_periph_id_t switch_rcc_id(const gpio_t *gpio_set)
{
	switch(gpio_set->port) {
	case gpio_port_a:		return rcc_gpioa_id;
	case gpio_port_b: 		return rcc_gpiob_id;
	case gpio_port_c: 		return rcc_gpioc_id;
	case gpio_port_d: 		return rcc_gpiod_id;
	case gpio_port_e: 		return rcc_gpioe_id;
	default:				return rcc_none_id;
	}
}

void gpio_rcc_init(const gpio_t *gpio_set)
{
	rcc_periph_id_t gpio_id = switch_rcc_id(gpio_set);
	rcc_periph_enable(gpio_id);
}
