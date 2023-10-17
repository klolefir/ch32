#ifndef ETH_H_SENTRY
#define ETH_H_SENTRY

#include "ch32v30x.h"

enum {
	eth_max_packet_size = 1536,

	eth_rxbuff_num = 4,
	eth_rxbuff_size = eth_max_packet_size,

	eth_txbuff_num = 2,
	eth_txbuff_size = eth_max_packet_size,

	eth_phy_addr = 1
};

enum {
	/* MMCRIMR */
	eth_phy_bmcr_addr = 0x00,
	eth_phy_bmsr_addr = 0x01,
	eth_phy_anlpar_addr = 0x05,
	eth_phy_sr_addr = 0x10,
	eth_phy_mdix_addr = 0x1e,

	/* BMCR */
	eth_phy_reset_pos = 15,
	eth_phy_reset = (1 << eth_phy_reset_pos),

	phy_autoneg_pos = 12,
	phy_autoneg_on = (1 << phy_autoneg_pos),
	phy_autoneg_off = (0 << phy_autoneg_pos),

	phy_loopback_pos = 14,
	phy_loopback_on = (1 << phy_loopback_pos),
	phy_loopback_off = (0 << phy_loopback_pos),
};

void eth_init(const uint8_t *mac_addr);
uint32_t eth_receive(uint8_t *recv_data);
void eth_transmit(const uint8_t *send_data, const uint32_t size);
void eth_read_phy_reg(const uint16_t phy_addr, const uint16_t reg, uint16_t *val);
void eth_write_phy_reg(const uint16_t phy_addr, const uint16_t reg, const uint16_t val);

#endif
