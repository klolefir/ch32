#include "brieip.h"
#include "kestring.h"

#if 0
static mac_packet_t *decode_mac_packet(void);
static arp_packet_t *decode_arp_packet(void);
static ip_packet_t *decode_ip_packet(void);
#endif
static void handle_packet(void);
static void handle_mac_packet(void);
static uint8_t handle_arp_packet(void);
static uint8_t handle_ip_packet(void);
static uint8_t handle_icmp_packet(void);
static void prepare_mac_packet(mac_packet_t *mac_packet);
static void prepare_arp_packet(arp_packet_t *arp_packet);
static void prepare_ip_packet(ip_packet_t *ip_packet);
static void prepare_icmp_packet(icmp_packet_t *icmp_packet, const uint16_t icmp_data_size);

static uint32_t htonl(const uint32_t hostlong);
static uint16_t htons(const uint16_t hostshort);
static uint16_t calculate_cs(const uint8_t *data, const uint16_t size);

static brieip_buff_t buff_set;
static uint8_t brieip_mac[6];
static uint8_t brieip_ip[6];

void brieip_init(const uint8_t *mac_addr, const uint8_t *ip_addr, const uint16_t port)
{
	eth_init(mac_addr);
	kememcpy(brieip_mac, mac_addr, sizeof(brieip_mac));
	kememcpy(brieip_ip, ip_addr, sizeof(brieip_ip));
}

void brieip_process()
{
	buff_set.cnt = eth_receive(buff_set.buff);
	buff_set.cnt = 0;

	handle_packet();

	if(buff_set.cnt)
		eth_transmit(buff_set.buff, buff_set.cnt);
}

void handle_packet()
{
	handle_mac_packet();
}

void handle_mac_packet()
{
	int8_t status;
	mac_packet_t *mac_packet;
	mac_packet = (mac_packet_t *)(buff_set.buff + mac_packet_pos);

	/* check mac accordance */
	/* in future replace it with mac filter */
#if 0
	status = kememcmp(mac_packet->dst_addr, brieip_mac, sizeof(mac_packet->dst_addr));
	if(status)
		return;
#endif

	switch((htons(mac_packet->ethertype))) {
	case mac_ethertype_ipv4:
		status = handle_ip_packet();
		break;
	case mac_ethertype_arp:
		status = handle_arp_packet();
		break;
	default:
		return;
	}

	if(status == ip_packet_ok || status == arp_packet_ok)
		prepare_mac_packet(mac_packet);
}

uint8_t handle_arp_packet()
{
	uint16_t htype, ptype;
	arp_packet_t *arp_packet;
	arp_packet = (arp_packet_t *)(buff_set.buff + arp_packet_pos);
	htype = htons(arp_htype_eth);
	ptype = htons(arp_ptype_ipv4);
	if((arp_packet->htype != htype) ||
		(arp_packet->ptype != ptype) ||
		(arp_packet->haddr_len != arp_haddrlen_eth) ||
		(arp_packet->paddr_len != arp_paddrlen_ipv4))
	{
		return arp_packet_bad;
	}
	prepare_arp_packet(arp_packet);
	return arp_packet_ok;
}

uint8_t handle_ip_packet()
{
	uint8_t status, packet_size;
	uint16_t cs;
	ip_packet_t *ip_packet;
	ip_packet = (ip_packet_t *)(buff_set.buff + ip_packet_pos);
	status = kememcmp(ip_packet->dst_ip, brieip_ip, sizeof(ip_packet->dst_ip));
	 /* packet size in words, convert words->bytes */
	packet_size = (ip_packet->verihl & 0x0F) << 2;
	 /* cs must be zero */
	cs = calculate_cs((uint8_t *)ip_packet, packet_size);
	if(((ip_packet->verihl & 0xF0) != 0x40/*ip_ver_ipv4*/) ||
		(ip_packet->flagsoff & htons(0x2000)/*ip_flags_mf*/) ||
		(cs) ||
		(status))
	{
		return ip_packet_bad;
	}

	switch(ip_packet->protocol) {
	case ip_protocol_icmp:
		status = handle_icmp_packet();
		break;
	}

	if(status != icmp_packet_ok)
		return ip_packet_bad;

	prepare_ip_packet(ip_packet);
	return ip_packet_ok;
}

uint8_t handle_icmp_packet()
{
	uint16_t icmp_data_size, cs, packet_size;
	ip_packet_t *ip_packet;
	icmp_packet_t *icmp_packet;
	ip_packet = (ip_packet_t *)(buff_set.buff + ip_packet_pos);
	icmp_packet = (icmp_packet_t *)(buff_set.buff + icmp_packet_pos);

	/* ip packet size */
	packet_size = (ip_packet->verihl & 0x0F) << 2;
	 /* ip + icmp packet size */
	icmp_data_size = htons(ip_packet->len);
	icmp_data_size -= packet_size;
	icmp_data_size -= icmp_packet_size;

	/* icmp packet size */
	packet_size = icmp_packet_size + icmp_data_size;

	cs = calculate_cs((uint8_t *)icmp_packet, packet_size);
	if(cs)
		return icmp_packet_bad;

	switch(icmp_packet->type) {
	case icmp_type_echoreq:
		prepare_icmp_packet(icmp_packet, icmp_data_size);
		break;
	case icmp_type_echorep:
		return icmp_packet_bad;
	}

	return icmp_packet_ok;
}

void prepare_mac_packet(mac_packet_t *mac_packet)
{
	kememcpy(mac_packet->dst_addr, mac_packet->src_addr, sizeof(mac_packet->dst_addr));
	kememcpy(mac_packet->src_addr, brieip_mac, sizeof(mac_packet->src_addr));
	buff_set.cnt += mac_packet_size;
}

void prepare_arp_packet(arp_packet_t *arp_packet)
{
	kememcpy(arp_packet->target_haddr, arp_packet->sender_haddr, sizeof(arp_packet->target_haddr));
	kememcpy(arp_packet->sender_haddr, brieip_mac, sizeof(arp_packet->sender_haddr));
	kememcpy(arp_packet->target_paddr, arp_packet->sender_paddr, sizeof(arp_packet->target_paddr));
	kememcpy(arp_packet->sender_paddr, brieip_ip, sizeof(arp_packet->target_paddr));
	switch(htons(arp_packet->opcode)) {
	case arp_opcode_request:
		arp_packet->opcode = htons(arp_opcode_reply);
		break;
	case arp_opcode_reply:
		arp_packet->opcode = htons(arp_opcode_request);
		break;
	}
	buff_set.cnt += arp_packet_size;
}

void prepare_ip_packet(ip_packet_t *ip_packet)
{
	uint16_t packet_size, data_size;
	data_size = htons(ip_packet->len);
	packet_size = (ip_packet->verihl & 0x0F) << 2;
	data_size -= packet_size;
	ip_packet->len =
		ip_packet_size +
		data_size;
	ip_packet->len = htons(ip_packet->len);
#if 0
	switch(ip_packet->protocol) {
	case ip_protocol_icmp:
		ip_packet->len += htons(46);
		break;
	}
#endif
	ip_packet->ttl = ip_ttl_max;
	kememcpy(ip_packet->dst_ip, ip_packet->src_ip, sizeof(ip_packet->dst_ip));
	kememcpy(ip_packet->src_ip, brieip_ip, sizeof(ip_packet->src_ip));

	ip_packet->cs = 0;
	packet_size = (ip_packet->verihl & 0x0F) << 2;
	ip_packet->cs = calculate_cs((uint8_t *)ip_packet, packet_size);
	ip_packet->cs = htons(ip_packet->cs);	
	buff_set.cnt += ip_packet_size;
}

void prepare_icmp_packet(icmp_packet_t *icmp_packet, const uint16_t icmp_data_size)
{
	uint16_t packet_size;
	switch(icmp_packet->type) {
	case icmp_type_echoreq:
		icmp_packet->type = icmp_type_echorep;
		break;
	case icmp_type_echorep:
		icmp_packet->type = icmp_type_echoreq;
		break;
	}

	packet_size = icmp_packet_size + icmp_data_size;
	icmp_packet->cs = 0;
	icmp_packet->cs = calculate_cs((uint8_t *)icmp_packet, packet_size);
	icmp_packet->cs = htons(icmp_packet->cs);

	buff_set.cnt += icmp_packet_size + icmp_data_size;
}

uint32_t htonl(const uint32_t hostlong)
{
	uint32_t num =
		((hostlong >> 24) & 0x000000FF) |
		((hostlong >> 8) & 0x0000FF00) |
		((hostlong << 24) & 0xFF000000) |
		((hostlong << 8) & 0x00FF0000);
	return num;
}

uint16_t htons(const uint16_t hostshort)
{
	uint16_t num =
		(hostshort >> 8) |
		(hostshort << 8);
	return num;
}

uint16_t calculate_cs(const uint8_t *data, const uint16_t size)
{
	uint32_t cs = 0;
	uint16_t i;
	for(i = 0; i < size; i += 2) {
		/*cs += htons((uint16_t)data[i]);*/
		cs += (data[i] << 8) | data[i + 1];
	}

	do {
		i = (cs >> 16);
		cs = i + (cs & 0xFFFF);
	} while(i);
#if 0
	while(i = (cs >> 16)) {
		cs = i + (cs & 0xFFFF);
	}
#endif
	cs = ~cs;
	return cs;
}
