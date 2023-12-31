#ifndef USART_H_SENTRY
#define USART_H_SENTRY

#include "ch32v30x.h"
#include "gpio.h"
#include "rcc.h"

enum {
	usart1_gpio_port 	= gpio_port_a,
	usart1_tx_pin 		= 9,
	usart1_rx_pin 		= 10,
	usart1_bus_div 		= 1/*rcc_apb2div*/,
	usart1_irq_prior	= 0
};

enum {
	usart2_gpio_port 	= gpio_port_a,
	usart2_tx_pin 		= 2,
	usart2_rx_pin 		= 3,
	usart2_bus_div 		= 1/*rcc_apb1div*/,
	usart2_irq_prior	= 0
};

enum {
	usart3_gpio_port 	= gpio_port_b,
	usart3_tx_pin 		= 10,
	usart3_rx_pin 		= 11,
	usart3_bus_div 		= 1,
	usart3_irq_prior	= 0
};

typedef enum {
	usart_num_1,
	usart_num_2,
	usart_num_3
} usart_num_t;

typedef enum {
	usart_baud_9600 	= 9600,
	usart_baud_115200 	= 115200
} usart_baud_t;

typedef enum {
	usart_rx_off 	= 0,
	usart_rx_on		= 1
} usart_rx_state_t;

typedef enum {
	usart_rx_rdy,
	usart_rx_nrdy
} usart_rx_status_t;

typedef enum {
	usart_tx_rdy,
	usart_tx_nrdy
} usart_tx_status_t;

typedef enum {
	usart_tx_off 	= 0,
	usart_tx_on 	= 1
} usart_tx_state_t;

typedef enum {
	usart_rx_irq_off 	= 0,
	usart_rx_irq_on		= 1
} usart_rx_irq_state_t;

typedef enum {
	usart_tx_irq_off	= 0,
	usart_tx_irq_on		= 1
} usart_tx_irq_state_t;

typedef enum {
	usart_tc_flag = USART_STATR_TC,
	usart_rxne_flag = USART_STATR_RXNE,
	usart_txe_flag = USART_STATR_TXE
} usart_flag_t;

typedef struct {
	usart_num_t 			usart			: 3;
	usart_baud_t			baud;
	usart_rx_state_t		rx_state 		: 1;
	usart_rx_state_t		tx_state 		: 1;
	usart_rx_irq_state_t	rx_irq_state 	: 1;
	usart_tx_irq_state_t	tx_irq_state	: 1;
} usart_t;

void usart_init(const usart_t *usart_set);

void usart_put_char(const usart_t *usart_set, const char c);
void usart_put_uint32(const usart_t *usart_set, const uint32_t data);
void usart_put_str(const usart_t *usart_set, const char *str);
void usart_put_buff(const usart_t *usart_set, const void *buff, const uint32_t byte_num);
void usart_get_char(const usart_t *usart_set, char *c);

void usart_enable(const usart_t *usart_set);
void usart_disable(const usart_t *usart_set);

void usart_rx_enable(usart_t *usart_set);
void usart_rx_disable(usart_t *usart_set);

void usart_tx_enable(usart_t *usart_set);
void usart_tx_disable(usart_t *usart_set);

void usart_rx_irq_enalbe(usart_t *usart_set);
void usart_rx_irq_disable(usart_t *usart_set);

void usart_tx_irq_enalbe(usart_t *usart_set);
void usart_tx_irq_disable(usart_t *usart_set);

/*void usart_clear_flag(usart_t *usart_set, usart_flag_t);*/
void usart_tx_flag_clear(const usart_t *usart_set);
void usart_rx_flag_clear(const usart_t *usart_set);

usart_rx_status_t usart_get_rx_status(const usart_t *usart_set);
usart_tx_status_t usart_get_tx_status(const usart_t *usart_set);

#endif
