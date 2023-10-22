#include "usart.h"
#include "tim.h"
#include "rcc.h"

int main(void)
{
	char c = '8';
	rcc_init();

	usart_t usart;
	usart.usart = usart_num_1;
	usart.baud = usart_baud_115200;
	usart.rx_state = usart_rx_off;
	usart.tx_state = usart_tx_on;
	usart.tx_irq_state = usart_tx_irq_off;
	usart.rx_irq_state = usart_rx_irq_on;
	usart_init(&usart);

	tim_hold_init();

    while(1)
    {
		tim_delay_ms(1000);
		usart_put_char(&usart, c);
    }
}
