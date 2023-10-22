#include "gpio.h"
#include "tim.h"
#include "rcc.h"

int main(void)
{
	rcc_init();

	gpio_t led;
	led.port = gpio_port_b;
	led.pin = gpio_pin_10;
	led.mode = gpio_mode_out50mhz;
	led.oconfig = gpio_oconfig_pp;
	gpio_init(&led);
	gpio_clr(&led);

	tim_hold_init();

    while(1)
    {
		/*gpio_tgl(&led);*/
		tim_delay_ms(1000);
    }
}
