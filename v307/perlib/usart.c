#include "usart.h"
#include "nvic.h"
#include "gpio.h"
#include "rcc.h"
#include "kestring.h"
#include "stdlib.h"

static void usart_gpio_init(const usart_t *usart_set);
static void usart_rcc_init(const usart_t *usart_set);
static void irq_init(const usart_t *usart_set);
static void tx_init(const usart_t *usart_set);
static void rx_init(const usart_t *usart_set);
static void baud_init(const usart_t *usart_set);
static rcc_periph_id_t switch_rcc_id(const usart_t *usart_set);
static USART_TypeDef *switch_usart(const usart_t *usart_set);
static nvic_irqn_t switch_irqn(const usart_t *usart_set);
static nvic_irq_prior_t switch_irq_prior(const usart_t *usart_set);
static rcc_bus_div_t switch_bus_div(const usart_t *usart_set);

void usart_init(const usart_t *usart_set)
{
	usart_gpio_init(usart_set);
	usart_rcc_init(usart_set);
	irq_init(usart_set);
	tx_init(usart_set);
	rx_init(usart_set);
	baud_init(usart_set);
	usart_enable(usart_set);
}

void usart_enable(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart->CTLR1 |= USART_CTLR1_UE;
}

void usart_disable(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart->CTLR1 &= ~USART_CTLR1_UE;
}

void usart_tx_enable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->tx_state = usart_tx_on;
	usart->CTLR1 |= USART_CTLR1_TE;
}

void usart_rx_enable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->rx_state = usart_rx_on;
	usart->CTLR1 |= USART_CTLR1_RE;
}

void usart_tx_disable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->tx_state = usart_tx_off;
	usart->CTLR1 &= ~USART_CTLR1_TE;
}

void usart_rx_disable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->rx_state = usart_rx_off;
	usart->CTLR1 &= ~USART_CTLR1_RE;
}

void usart_tx_irq_enable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->tx_irq_state = usart_tx_irq_on;
	usart->CTLR1 |= USART_CTLR1_TXEIE;
}

void usart_rx_irq_enable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->rx_irq_state = usart_rx_irq_on;
	usart->CTLR1 |= USART_CTLR1_RXNEIE;
}

void usart_tx_irq_disable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->tx_irq_state = usart_tx_irq_off;
	usart->CTLR1 &= ~USART_CTLR1_TXEIE;
}

void usart_rx_irq_disable(usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart_set->rx_irq_state = usart_rx_irq_off;
	usart->CTLR1 &= ~USART_CTLR1_RXNEIE;
}

void usart_put_char(const usart_t *usart_set, const char c)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	while(!(usart->STATR & USART_STATR_TXE)) {}
	usart->DATAR = c;
}

void usart_put_uint32(const usart_t *usart_set, const uint32_t data)
{
	char data_str[5];
	kememcpy(data_str, &data, sizeof(uint32_t));
	data_str[4] = '\0';
	usart_put_str(usart_set, data_str);
}


void usart_put_str(const usart_t *usart_set, const char *str)
{
	while(*str) {
		usart_put_char(usart_set, *str);
		str++;
	}
}

void usart_put_buff(const usart_t *usart_set, const void *buff, const uint32_t byte_num)
{
	uint8_t *buff_ptr = (uint8_t *)buff;
	int i;
	for(i = 0; i < byte_num; i++)
		usart_put_char(usart_set, buff_ptr[i]);
}

usart_rx_status_t usart_get_rx_status(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	if((usart->STATR & USART_STATR_RXNE))
		return usart_rx_rdy;
	else
		return usart_rx_nrdy;
}

usart_tx_status_t usart_get_tx_status(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	if((usart->STATR & USART_STATR_TXE))
		return usart_tx_rdy;
	else
		return usart_tx_nrdy;
}

void usart_get_char(const usart_t *usart_set, char *c)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	/*while(!(usart->SR & USART_SR_RXNE)) {}*/
	*c = usart->DATAR;
}

void usart_gpio_init(const usart_t *usart_set)
{
	gpio_t tx_pin, rx_pin;

	tx_pin.mode 	= gpio_mode_out50mhz;
	tx_pin.oconfig 	= gpio_oconfig_altpp;

	rx_pin.mode 	= gpio_mode_input;
	rx_pin.iconfig 	= gpio_iconfig_floating;

	switch(usart_set->usart) {
	case usart_num_1: 	tx_pin.port = usart1_gpio_port;
						tx_pin.pin 	= usart1_tx_pin; 
						rx_pin.port = usart1_gpio_port;
						rx_pin.pin 	= usart1_rx_pin; 
						break;
	case usart_num_2: 	tx_pin.port = usart2_gpio_port;
						tx_pin.pin 	= usart2_tx_pin;
						rx_pin.port = usart2_gpio_port;
						rx_pin.pin 	= usart2_rx_pin;
						break;
	case usart_num_3: 	tx_pin.port = usart3_gpio_port;
						tx_pin.pin 	= usart3_tx_pin;
						rx_pin.port = usart3_gpio_port;
						rx_pin.pin 	= usart3_rx_pin;
						break;
	default:			return;
	}

	gpio_init(&rx_pin);
	gpio_init(&tx_pin);
}

void usart_rcc_init(const usart_t *usart_set)
{
	rcc_periph_id_t usart_id = switch_rcc_id(usart_set);
	rcc_periph_enable(usart_id);
}

void usart_tx_flag_clear(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart->STATR &= ~usart_txe_flag;
}

void usart_rx_flag_clear(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	usart->STATR &= ~usart_rxne_flag;
}

void irq_init(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	nvic_irqn_t usart_irqn = switch_irqn(usart_set);
	nvic_irq_prior_t usart_irq_prior = switch_irq_prior(usart_set);

	nvic_enable_irq(usart_irqn, usart_irq_prior);

	switch(usart_set->tx_irq_state) {
	case usart_tx_irq_off:	usart->CTLR1 &= ~USART_CTLR1_TXEIE;
							break;
	case usart_tx_irq_on:	usart->CTLR1 |= USART_CTLR1_TXEIE;
							break;
	}

	switch(usart_set->rx_irq_state) {
	case usart_rx_irq_off:	usart->CTLR1 &= ~USART_CTLR1_RXNEIE;
							break;
	case usart_rx_irq_on:	usart->CTLR1 |= USART_CTLR1_RXNEIE;
							break;
	}
}

rcc_periph_id_t switch_rcc_id(const usart_t *usart_set)
{
	switch(usart_set->usart) {
	case usart_num_1: 	return rcc_usart1_id;
	case usart_num_2: 	return rcc_usart2_id;
	case usart_num_3: 	return rcc_usart3_id;
	default:			return rcc_none_id;	
	}
}

nvic_irqn_t switch_irqn(const usart_t *usart_set)
{
	switch(usart_set->usart) {
	case usart_num_1: 	return nvic_usart1_irqn;
	case usart_num_2: 	return nvic_usart2_irqn;
	case usart_num_3: 	return nvic_usart3_irqn;
	default:			return nvic_none_irqn;	
	}
}

nvic_irq_prior_t switch_irq_prior(const usart_t *usart_set)
{
	switch(usart_set->usart) {
	case usart_num_1: 	return usart1_irq_prior;
	case usart_num_2: 	return usart2_irq_prior;
	case usart_num_3: 	return usart3_irq_prior;
	default:			return nvic_none_irq_prior;
	}
}

rcc_bus_div_t switch_bus_div(const usart_t *usart_set)
{
	switch(usart_set->usart) {
	case usart_num_1: 	return usart1_bus_div;
	case usart_num_2: 	return usart2_bus_div;
	case usart_num_3: 	return usart3_bus_div;
	default:			return 1;
	}
}

void tx_init(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	switch(usart_set->tx_state) {
	case usart_tx_off:	usart->CTLR1 &= ~USART_CTLR1_TE;
						break;
	case usart_tx_on:	usart->CTLR1 |= USART_CTLR1_TE;
						break;
	}
}

void rx_init(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	switch(usart_set->rx_state) {
	case usart_rx_off:	usart->CTLR1 &= ~USART_CTLR1_RE;
						break;
	case usart_rx_on:	usart->CTLR1 |= USART_CTLR1_RE;
						break;
	}
}

void baud_init(const usart_t *usart_set)
{
	USART_TypeDef *usart = switch_usart(usart_set);
	uint32_t clk = rcc_get_system_clk();
	rcc_bus_div_t usart_bus_div = switch_bus_div(usart_set);

#if 0
	uint32_t integerdivider, fractionaldivider, baud;
	integerdivider = ((25 * (clk / usart_bus_div)) / (4 * (usart_set->baud)));
    fractionaldivider = integerdivider - (100 * (usart_set->baud >> 4));
    baud = (integerdivider / 100) << 4;
	baud |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
	usart->BRR = (uint16_t)baud;
#else
	usart->BRR = (clk / usart_bus_div) / (usart_set->baud);
#endif
}

USART_TypeDef *switch_usart(const usart_t *usart_set)
{
	switch(usart_set->usart) {
	case usart_num_1: 	return USART1;
	case usart_num_2: 	return USART2;
	default:			return 0;
	}
}
