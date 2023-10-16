#include "gpio.h"
#include "eth.h"
#include "rcc.h"

const uint8_t mac_addr[] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x77 };

int main(void)
{
	uint16_t val;
	rcc_init();
    SystemCoreClockUpdate();

	eth_init(mac_addr);
	eth_read_phy_reg(eth_phy_addr, eth_phy_bmsr_addr, &val);

    while(1)
    { }
}
