#include "brieip.h"
#include "rcc.h"
#include "tim.h"
#include "usart.h"

void ETH_IRQHandler(void)    __attribute__((interrupt(/*"WCH-Interrupt-fast"*/)));

void init(void);

const uint8_t mac_addr[] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x77 };
const uint8_t ip_addr[] = { 192, 168, 1, 200 };
const uint16_t port = 7777;

uint16_t buff_cnt;
uint8_t buff[2000];

usart_t usart;

int main(void)
{
	uint16_t val;
	__enable_irq();
	init();
	tim_delay_ms(1000);
	usart_put_str(&usart, "BRIEIP SNIPPET\r");
	eth_read_phy_reg(phy_addr, phy_physr_addr, &val);
    while(1) { 
		brieip_process();
	}
}

void ETH_IRQHandler()
{
	if(ETH->DMASR & eth_pls) {
		usart_put_str(&usart, "PHY LINK STATUS OK\r");
		ETH->DMASR |= eth_pls;
	}
	if(ETH->DMASR & eth_tus) {
		usart_put_str(&usart, "TRANSMIT UNDERFLOW\r");
		ETH->DMASR |= eth_tus;
	}
	if(ETH->DMASR & eth_tjts) {
		usart_put_str(&usart, "TRANSMIT JABBER\r");
		ETH->DMASR |= eth_tjts;
	}
	if(ETH->DMASR & eth_tbus) {
		usart_put_str(&usart, "TRANSMIT BUFFER UNAVAILABLE\r");
		ETH->DMASR |= eth_tbus;
	}
	if(ETH->DMASR & eth_tpss) {
		usart_put_str(&usart, "TRANSMIT STOP\r");
		ETH->DMASR |= eth_tpss;
	}
	if(ETH->DMASR & eth_ts) {
		usart_put_str(&usart, "TRANSMIT OK\r");
		ETH->DMASR |= eth_ts;
	}
	ETH->DMASR |= eth_ais | eth_nis;
#if 0
	if(ETH->DMASR & eth_rs) {
		usart_put_str(&usart, "RECEIVE OK\r");
		ETH->DMASR |= eth_rs;
	}
	if(ETH->DMASR & eth_rbus) {
		usart_put_str(&usart, "RECEIVE BUFFER UNAVAILABLE\r");
		ETH->DMASR |= eth_rbus;
	}
#endif
}

void init(void)
{
	rcc_init();
	tim_hold_init();

	usart_t usart;
	usart.usart = usart_num_1;
	usart.baud = usart_baud_115200;
	usart.rx_state = usart_rx_on;
	usart.tx_state = usart_tx_on;
	usart.tx_irq_state = usart_tx_irq_off;
	usart.rx_irq_state = usart_rx_irq_off;
	usart_init(&usart);

	brieip_init(mac_addr, ip_addr, port);
}
