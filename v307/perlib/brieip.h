#ifndef BRIEIP
#define BRIEIP

#include "eth.h"

enum {
	brieip_buff_size = eth_max_packet_size
};

enum {
	mac_packet_pos = 0,
	mac_packet_size = 14,
	arp_packet_pos = mac_packet_pos + mac_packet_size,
	arp_packet_size = 28,
	ip_packet_pos = mac_packet_pos + mac_packet_size,
	ip_packet_size = 20,
	icmp_packet_pos = ip_packet_pos + ip_packet_size,
	icmp_packet_size = 4
};

enum {
	mac_ethertype_ipv4 = 0x0800,
	mac_ethertype_arp = 0x0806
};

enum {
	arp_packet_bad,
	arp_packet_ok,
	ip_packet_bad,
	ip_packet_ok,
	icmp_packet_bad,
	icmp_packet_ok
};

typedef struct {
	uint8_t buff[brieip_buff_size];
	uint32_t cnt;
} brieip_buff_t;

typedef struct {
	uint8_t dst_addr[6];
	uint8_t src_addr[6];
	uint16_t ethertype;
} mac_packet_t;

enum {
	arp_htype_eth = 0x0001,
	arp_ptype_ipv4 = 0x0800,
	arp_opcode_request = 0x0001,
	arp_opcode_reply = 0x0002,
	arp_haddrlen_eth = 0x06,
	arp_paddrlen_ipv4 = 0x04
};

typedef struct {
	uint16_t htype;
	uint16_t ptype;
	uint8_t haddr_len;
	uint8_t paddr_len;
	uint16_t opcode;
	uint8_t sender_haddr[arp_haddrlen_eth];
	uint8_t sender_paddr[arp_paddrlen_ipv4];
	uint8_t target_haddr[arp_haddrlen_eth];
	uint8_t target_paddr[arp_paddrlen_ipv4];
} arp_packet_t;

enum {
	ip_ver_ipv4 = 0x04,
	ip_ttl_max = 0x40,
	ip_protocol_udp = 0x11,
	ip_protocol_tcp = 0x06,
	ip_protocol_icmp = 0x01
};

enum {
	ip_flags_df_pos = 1,
	ip_flags_df = (1 << ip_flags_df_pos),
	ip_flags_mf_pos = 2,
	ip_flags_mf = (1 << ip_flags_mf_pos)
};

typedef struct {
	uint8_t verihl;
	uint8_t dscp : 6;
	uint8_t ecn : 2;
	uint16_t len;
	uint16_t id;
	uint16_t flagsoff;
	uint8_t ttl;
	uint8_t protocol;
	uint16_t cs;
	uint8_t src_ip[4];
	uint8_t dst_ip[4];
} ip_packet_t;
#if 0
typedef struct {
	uint8_t ver : 4;
	uint8_t ihl : 4;
	uint8_t dscp : 6;
	uint8_t ecn : 2;
	uint16_t len;
	uint16_t id;
	uint8_t flags : 3;
	uint16_t offset : 13;
	uint8_t ttl;
	uint8_t protocol;
	uint16_t cs;
	uint8_t src_ip[4];
	uint8_t dst_ip[4];
} ip_packet_t;
#endif

enum {
	icmp_type_echoreq = 0x08,
	icmp_type_echorep = 0x00
};

typedef struct  {
	uint8_t type;
	uint8_t code;
	uint16_t cs;
} icmp_packet_t;

void brieip_init(const uint8_t *mac_addr, const uint8_t *ip_addr, const uint16_t port);
void brieip_process(void);

#endif
