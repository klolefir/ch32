#include "gpio.h"
#include "tim.h"
#include "rcc.h"

#if 0
void TIM7_IRQHandler(void)    __attribute__((interrupt(/*"WCH-Interrupt-fast"*/)));
void TIM6_IRQHandler(void)    __attribute__((interrupt(/*"WCH-Interrupt-fast"*/)));
#endif
tim_t tim6;

int main(void)
{
	uint32_t ticks;
#if 0
	SystemInit();
    SystemCoreClockUpdate();
#endif
	rcc_init();

	gpio_t led;
	led.port = gpio_port_b;
	led.pin = gpio_pin_10;
	led.mode = gpio_mode_out50mhz;
	led.oconfig = gpio_oconfig_pp;
	gpio_init(&led);

	tim6.tim = tim_num_6;
	tim6.irq_state = tim_irq_on;
	tim6.psc = 144000000 / 10000;
	tim6.arr = 10;
	tim_init(&tim6);

    while(1)
    {
		gpio_tgl(&led);
		do {
			ticks = tim_get_ticks(&tim6);
		} while(ticks != 1000);
		tim_set_ticks(&tim6, 0);
    }
}

#if 0
void TIM6_IRQHandler()
{
	uint32_t ticks;
	TIM_TypeDef *tim = TIM6;
	if(tim->INTFR & TIM_UIF) {
		ticks = tim_get_ticks(&tim6);
		tim_set_ticks(&tim6, ticks + 1);
	}
	tim->INTFR &= ~TIM_UIF;
}
#endif
