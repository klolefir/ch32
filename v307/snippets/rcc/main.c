#include "rcc.h"
#include "gpio.h"
#include "systick.h"

int main(void)
{
#if 1
	SystemInit();
    SystemCoreClockUpdate();
#else
	rcc_init();
#endif
#if 1
	systick_init(1000);
#endif

	__enable_irq();
	gpio_t led;
	led.port = gpio_port_c;
	led.pin = gpio_pin_0;
	led.mode = gpio_mode_out50mhz;
	led.oconfig = gpio_oconfig_pp;
	gpio_init(&led);

    while(1)
    {
		gpio_tgl(&led);
		systick_delay_ms(500);
#if 0
		for(i = 0; i < 1000000; i++)
		{}
#endif
    }
}
