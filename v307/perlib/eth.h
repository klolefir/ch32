#ifndef ETH_H_SENTRY
#define ETH_H_SENTRY

#include "ch32v30x.h"

enum {
	eth_max_packet_size = 1536,

	eth_rxbuff_num = 4,
	eth_rxbuff_size = eth_max_packet_size,

	eth_txbuff_num = 2,
	eth_txbuff_size = eth_max_packet_size,

	eth_irq_priority = 0,

	phy_addr = 1
};

enum {
	/* MMCRIMR */
	phy_bmcr_addr = 0x00,
	phy_bmsr_addr = 0x01,
	phy_anlpar_addr = 0x05,
	phy_physr_addr = 0x10,
	phy_mdix_addr = 0x1e,

	/* BMCR */
	phy_reset_pos = 15,
	phy_reset = (1 << phy_reset_pos),

	phy_autoneg_pos = 12,
	phy_autoneg = (1 << phy_autoneg_pos),

	phy_resautoneg_pos = 9,
	phy_resautoneg = (1 << phy_resautoneg_pos),

	phy_loopback_pos = 14,
	phy_loopback_on = (1 << phy_loopback_pos),
	phy_loopback_off = (0 << phy_loopback_pos),

	/* BMSR */

	phy_autonegcmpl_pos = 5,
	phy_autonegcmpl = (1 << phy_autonegcmpl_pos),

	phy_link_pos = 2,
	phy_link = (1 << phy_link_pos),
	
	/* MDIX */

	phy_tr_pos = 0,
	phy_tr_auto = (0b00 << phy_tr_pos),
	phy_tr_mdix = (0b01 << phy_tr_pos),
	phy_tr_mdi = (0b10 << phy_tr_pos),

	phy_polarity_pos = 2,
	phy_polarity_normal = (0b00 << phy_polarity_pos),
	phy_polarity_reversed = (0b01 << phy_polarity_pos),
};

enum {
	/* DMASR */
	eth_pls_pos = 31,
	eth_pls = (1 << eth_pls_pos),

	eth_nis_pos = 16,
	eth_nis = (1 << eth_nis_pos),

	eth_ais_pos = 15,
	eth_ais = (1 << eth_ais_pos),

	eth_rbus_pos = 7,
	eth_rbus = (1 << eth_rbus_pos),

	eth_rs_pos = 6,
	eth_rs = (1 << eth_rs_pos),

	eth_tus_pos = 5,
	eth_tus = (1 << eth_tus_pos),

	eth_tjts_pos = 3,
	eth_tjts = (1 << eth_tjts_pos),

	eth_tbus_pos = 2,
	eth_tbus = (1 << eth_tbus_pos),

	eth_tpss_pos = 1,
	eth_tpss = (1 << eth_tpss_pos),

	eth_ts_pos = 0,
	eth_ts = (1 << eth_ts_pos),

};

void eth_init(const uint8_t *mac_addr);
uint32_t eth_receive(uint8_t *recv_data);
void eth_transmit(const uint8_t *send_data, const uint32_t size);
void eth_read_phy_reg(const uint16_t phy_addr, const uint16_t reg, uint16_t *val);
void eth_write_phy_reg(const uint16_t phy_addr, const uint16_t reg, const uint16_t val);

#endif
