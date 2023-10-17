#include "brieip.h"

#if 0
static void decode_arp_packet();
static void handle_arp_packet();
static void decode_ip_packet();
static void handle_ip_packet();
#endif

static brieip_buff_t brieip_buff;

void brieip_init(const uint8_t *mac_addr, const uint8_t *ip_addr, const uint16_t port)
{
	eth_init(mac_addr);
}

void brieip_process()
{
	brieip_buff.cnt = eth_receive(brieip_buff.buff);

	if(brieip_buff.cnt)
		eth_transmit(brieip_buff.buff, brieip_buff.cnt);
}
