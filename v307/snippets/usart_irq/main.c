#include "usart.h"

usart_t usart;

int main(void)
{
	SystemInit();
    SystemCoreClockUpdate();
	__enable_irq();

	usart.usart = usart_num_2;
	usart.baud = usart_baud_115200;
	usart.rx_state = usart_rx_on;
	usart.tx_state = usart_tx_on;
	usart.tx_irq_state = usart_tx_irq_off;
	usart.rx_irq_state = usart_rx_irq_on;
	usart_init(&usart);

    while(1)
    {}
}

void USART2_IRQHandler()
{
	char c;
	usart_get_char(&usart, &c);
	usart_put_char(&usart, c);
	usart_rx_flag_clear(&usart);
}
