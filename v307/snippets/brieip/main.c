#include "brieip.h"
#include "rcc.h"

const uint8_t mac_addr[] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x77 };
const uint8_t ip_addr[] = { 0x88, 0x88, 0x88, 0x88 };
const uint16_t port = 7777;

int main(void)
{
	rcc_init();

	brieip_init(mac_addr, ip_addr, port);
    while(1) { 
		brieip_process();
	}
}
