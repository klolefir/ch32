#include "gpio.h"

int main(void)
{
	uint32_t i;
	SystemInit();
    SystemCoreClockUpdate();

	gpio_t led;
#if 1
	led.port = gpio_port_b;
	led.pin = gpio_pin_10;
#else
	led.port = gpio_port_c;
	led.pin = gpio_pin_0;
#endif
	led.mode = gpio_mode_out50mhz;
	led.oconfig = gpio_oconfig_pp;
	gpio_init(&led);

    while(1)
    {
		gpio_tgl(&led);
		for(i = 0; i < 1000000; i++)
		{}
    }
}
