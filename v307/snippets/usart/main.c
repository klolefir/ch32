#include "usart.h"
#include "tim.h"
#include "rcc.h"

int main(void)
{
	char c;
	usart_rx_status_t rx_status;
	rcc_init();

	usart_t usart;
	usart.usart = usart_num_1;
	usart.baud = usart_baud_115200;
	usart.rx_state = usart_rx_on;
	usart.tx_state = usart_tx_on;
	usart.tx_irq_state = usart_tx_irq_off;
	usart.rx_irq_state = usart_rx_irq_off;
	usart_init(&usart);

    while(1)
    {
		do {
			rx_status = usart_get_rx_status(&usart);
		} while(rx_status != usart_rx_rdy);
		usart_get_char(&usart, &c);
		usart_put_char(&usart, c);
    }
}
