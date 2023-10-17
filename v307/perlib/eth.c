#include "eth.h"
#include "rcc.h"
#include "kestring.h"

static void eth_filter_init(void);
static void eth_control_init(void);
static void eth_flow_control_init(void);
static void eth_dma_init(void);
static void eth_rcc_init(void);
static void eth_irq_init(void);
static void eth_dmatxdesc_init(void);
static void eth_dmarxdesc_init(void);
static void eth_start(void);
static void eth_phy_init(void);
static void eth_reset(void);
static void eth_set_mac(const uint8_t *mac_addr);

enum {
	/*MACCR*/
	eth_wd_pos = 23,
	eth_wd_on = (1 << eth_wd_pos),
	eth_wd_off = (0 << eth_wd_pos),

	eth_jd_pos = 22,
	eth_jd_on = (1 << eth_jd_pos),
	eth_jd_off = (0 << eth_jd_pos),

	eth_ifg_pos = 17,
	eth_ifg_96bit = (0b000 << eth_ifg_pos),

	eth_ipco_pos = 10,
	eth_ipco_on = (1 << eth_ipco_pos),
	eth_ipco_off = (0 << eth_ipco_pos),

	eth_dm_pos = 11,
	eth_dm_on = (1 << eth_dm_pos),
	eth_dm_off = (0 << eth_dm_pos),

	eth_fes_pos = 14,
	eth_fes_10mbs = (0b00 << eth_fes_pos),
	eth_fes_100mbs = (0b01 << eth_fes_pos),
	eth_fes_1gbs = (0b10 << eth_fes_pos),

	eth_lm_pos = 12,
	eth_lm_on = (1 << eth_lm_pos),
	eth_lm_off = (0 << eth_lm_pos),

	eth_apcs_pos = 7,
	eth_apcs_on = (1 << eth_apcs_pos),
	eth_apcs_off = (0 << eth_apcs_pos),

	eth_te_pos = 3,
	eth_te_on = (1 << eth_te_pos),
	eth_te_off = (0 << eth_te_pos),

	eth_re_pos = 2,
	eth_re_on = (1 << eth_re_pos),
	eth_re_off = (0 << eth_re_pos),

	/*MACFFR*/
	eth_saf_pos = 9,
	eth_saf_on = (1 << eth_saf_pos),
	eth_saf_off = (0 << eth_saf_pos),

	eth_pcf_pos = 6,
	eth_pcf_off = (0b00 << eth_pcf_pos),
	eth_pcf_on = (0b10 << eth_pcf_pos),
	eth_pcf_filter = (0b11 << eth_pcf_pos),

	eth_bfd_pos = 5,
	eth_bfd_on = (1 << eth_bfd_pos),
	eth_bfd_off = (0 << eth_bfd_pos),

	eth_daif_pos = 3,
	eth_daif_on = (1 << eth_daif_pos),
	eth_daif_off = (0 << eth_daif_pos),

	eth_ra_pos = 31,
	eth_ra_on = (1 << eth_ra_pos),
	eth_ra_off = (0 << eth_ra_pos),

	eth_pm_pos = 0,
	eth_pm_on = (1 << eth_pm_pos),
	eth_pm_off = (0 << eth_pm_pos),

	eth_hu_pos = 1,
	eth_hu_off = (0 << eth_hu_pos),
	eth_hu_on = (1 << eth_hu_pos),

	eth_hm_pos = 2,
	eth_hm_off = (0 << eth_hm_pos),
	eth_hm_on = (1 << eth_hm_pos),

	/*MACFCR*/
	eth_zqpd_pos = 7,
	eth_zqpd_off = (0 << eth_zqpd_pos),
	eth_zqpd_on = (1 << eth_zqpd_pos),

	eth_pt_pos = 16,
	eth_pt = (0 << eth_pt_pos),

	eth_plt_pos = 4,
	eth_plt_4ts = (0b00 << eth_plt_pos),

	eth_upfd_pos = 3,
	eth_upfd_on = (1 << eth_upfd_pos),
	eth_upfd_off = (0 << eth_upfd_pos),

	eth_rfce_pos = 2,
	eth_rfce_on = (1 << eth_rfce_pos),
	eth_rfce_off = (0 << eth_rfce_pos),

	eth_tfce_pos = 1,
	eth_tfce_on = (1 << eth_tfce_pos),
	eth_tfce_off = (0 << eth_tfce_pos),

	/* DMABMR */
	eth_softreset_pos = 0,
	eth_softreset = (1 << eth_softreset_pos),
	
	/* DMAOMR */
	eth_dtcefd_pos = 26,
	eth_dtcefd_on = (1 << eth_dtcefd_pos),
	eth_dtcefd_off = (0 << eth_dtcefd_pos),

	eth_ftf_pos = 20,
	eth_ftf = (1 << eth_ftf_pos),

	eth_st_pos = 13,
	eth_st_on = (1 << eth_st_pos),
	eth_st_off = (0 << eth_st_pos),

	eth_sr_pos = 1,
	eth_sr_on = (1 << eth_sr_pos),
	eth_sr_off = (0 << eth_sr_pos),

	/* transmit store forward */
	eth_tsf_pos = 21,
	eth_tsf_on = (1 << eth_tsf_pos),
	eth_tsf_off = (0 << eth_tsf_pos),

	eth_fef_pos = 7,
	eth_fef_on = (1 << eth_fef_pos),
	eth_fef_off = (0 << eth_fef_pos),

	eth_fugf_pos = 6,
	eth_fugf_on = (1 << eth_fugf_pos),
	eth_fugf_off = (0 << eth_fugf_pos),

	/* MACHTHR */
	eth_hth = 0,

	/* MACHTLR */
	eth_htl = 0,

	eth_rgufm_pos = 17,
	eth_rgufm_on = (1 << eth_rgufm_pos),
	eth_rgufm_off = (0 << eth_rgufm_pos),

	eth_rfcem_pos = 5,
	eth_rfcem_on = (1 << eth_rfcem_pos),
	eth_rfcem_off = (0 << eth_rfcem_pos),

	/* MMCTIMR */
	
	eth_tgfm_pos = 5,
	eth_tgfm_on = (1 << eth_tgfm_pos),
	eth_tgfm_off = (0 << eth_tgfm_pos),

	/* MACMIIAR */

	eth_pa_pos = 11,
	eth_pa = (0b11111 << eth_pa_pos),
	eth_mr_pos = 6,
	eth_mr = (0b11111 << eth_mr_pos), 
	eth_cr_pos = 2,
	eth_cr_div42 = (0b00 << eth_cr_pos),
	eth_mw_pos = 1,
	eth_mw_read = (0 << eth_mw_pos),
	eth_mw_write = (1 << eth_mw_pos),
	eth_mb_pos = 0,
	eth_mb = (1 << eth_mb_pos),

};

typedef struct {
	union {
		uint32_t rdes0;
		uint32_t tdes0;
	};
	union {
		uint32_t rdes1;
		uint32_t tdes1;
	};
	union {
		uint32_t rdes2;
		uint32_t tdes2;
	};
	union {
		uint32_t rdes3;
		uint32_t tdes3;
	};
} eth_dmadesc_t;

enum {
	eth_dmarxdesc_own_pos = 31,
	eth_dmarxdesc_own = (1 << eth_dmarxdesc_own_pos),

	eth_dmarxdesc_es_pos = 15,
	eth_dmarxdesc_es = (1 << eth_dmarxdesc_es_pos),

	eth_dmarxdesc_fs_pos = 9,
	eth_dmarxdesc_fs = (1 << eth_dmarxdesc_fs_pos),

	eth_dmarxdesc_ls_pos = 9,
	eth_dmarxdesc_ls = (1 << eth_dmarxdesc_ls_pos),

	eth_dmarxdesc_fl_pos = 16,
	eth_dmarxdesc_fl = (0x3FFF << eth_dmarxdesc_fl_pos),

	eth_dmarxdesc_rer_pos = 15,
	eth_dmarxdesc_rer = (1 << eth_dmarxdesc_rer_pos),

	eth_dmarxdesc_rch_pos = 14,
	eth_dmarxdesc_rch = (1 << eth_dmarxdesc_rch_pos),
};

enum {
	eth_dmatxdesc_own_pos = 31,
	eth_dmatxdesc_own = (1 << eth_dmatxdesc_own_pos),

	eth_dmatxdesc_fs_pos = 28,
	eth_dmatxdesc_fs = (1 << eth_dmarxdesc_fs_pos),

	eth_dmatxdesc_ls_pos = 29,
	eth_dmatxdesc_ls = (1 << eth_dmatxdesc_ls_pos),

	eth_dmatxdesc_fl_pos = 16,
	eth_dmatxdesc_fl = (0x3FFF << eth_dmatxdesc_fl_pos),

	eth_dmatxdesc_tch_pos = 20,
	eth_dmatxdesc_tch = (1 << eth_dmatxdesc_tch_pos),

	eth_dmatxdesc_tbs1_pos = 0,
	eth_dmatxdesc_tbs1 = (0x1FFF << eth_dmatxdesc_tbs1_pos)
};

__attribute__((__aligned__(4)))
static eth_dmadesc_t rx_descriptor[eth_rxbuff_num];
static eth_dmadesc_t *rx_descriptor_ptr;

__attribute__((__aligned__(4)))
static eth_dmadesc_t tx_descriptor[eth_txbuff_num];
static eth_dmadesc_t *tx_descriptor_ptr;

__attribute__((__aligned__(4))) 
static uint8_t rx_buff[eth_rxbuff_num * eth_rxbuff_size];
__attribute__((__aligned__(4)))
static uint8_t tx_buff[eth_txbuff_num * eth_txbuff_size];

void eth_init(const uint8_t *mac_addr)
{
	eth_rcc_init();
	/* enable internal phy */
	EXTEN->EXTEN_CTR |= EXTEN_ETH_10M_EN;

	eth_reset();

	eth_control_init();
	eth_filter_init();
	eth_dma_init();
	eth_irq_init();

	eth_set_mac(mac_addr);

	ETH->MACCR &= ~eth_dm_on; /* strange(check auto neg) */
	ETH->MACCR &= ~(eth_fes_100mbs | eth_fes_1gbs);

	eth_start();
	eth_phy_init();
}

uint32_t eth_receive(uint8_t *recv_data)
{
	uint32_t size;

	if((rx_descriptor_ptr->rdes0 & eth_dmarxdesc_own) && /* is owned */
		!(rx_descriptor_ptr->rdes0 & eth_dmarxdesc_es) &&  /*is error*/
		(rx_descriptor_ptr->rdes0 & eth_dmarxdesc_ls) && /*is last desc*/ 
		(rx_descriptor_ptr->rdes0 & eth_dmarxdesc_fs)) /*is first desc*/
	{
		size = ((rx_descriptor_ptr->rdes0 & eth_dmarxdesc_fl) >> eth_dmarxdesc_fl_pos) - 4;
		kememcpy(recv_data, &(rx_descriptor_ptr->rdes2), size);
	}
	else
	{
		return 0;
	}

	rx_descriptor_ptr->rdes0 = eth_dmarxdesc_own;
	rx_descriptor_ptr = (eth_dmadesc_t *)(rx_descriptor_ptr->rdes3);

	return size;
}

void eth_transmit(const uint8_t *send_data, const uint32_t size)
{
	if(tx_descriptor_ptr->rdes0 & eth_dmatxdesc_own)
		return;

	kememcpy(&(tx_descriptor_ptr->rdes2), send_data, size);
	tx_descriptor_ptr->rdes1 = (size & (eth_dmatxdesc_tbs1 >> eth_dmatxdesc_tbs1_pos));
	tx_descriptor_ptr->rdes0 |=
		eth_dmatxdesc_ls |
		eth_dmatxdesc_fs |
		eth_dmatxdesc_own;

	tx_descriptor_ptr = (eth_dmadesc_t *)(tx_descriptor_ptr->rdes2)	;
}

void eth_rcc_init()
{
	rcc_periph_enable(rcc_eth_id);
	rcc_periph_enable(rcc_ethrx_id);
	rcc_periph_enable(rcc_ethtx_id);
}

void eth_start()
{
	ETH->MACCR |= eth_te_on; /* enable tx */
	ETH->DMAOMR |= eth_ftf; /* reset fifo */
	ETH->MACCR |= eth_re_on; /* enable rx */
	ETH->DMAOMR |=eth_st_on; /* enable tx dma */
	ETH->DMAOMR |= eth_sr_on; /* enable rx dma */
}

void eth_read_phy_reg(const uint16_t phy_addr, const uint16_t reg, uint16_t *val)
{
	ETH->MACMIIAR |=
		((phy_addr << eth_pa_pos) & eth_pa) |
		((reg << eth_mr_pos) & eth_mr) |
		eth_mw_read |
		eth_mb;

	while(ETH->MACMIIAR & eth_mb)
	{}

	*val = (uint16_t)ETH->MACMIIDR;
}

void eth_write_phy_reg(const uint16_t phy_addr, const uint16_t reg, const uint16_t val)
{
	ETH->MACMIIDR = val;
	ETH->MACMIIAR |=
		((phy_addr << eth_pa_pos) & eth_pa) |
		((reg << eth_mr_pos) & eth_mr) |
		eth_mw_write |
		eth_mb;

	while(ETH->MACMIIAR & eth_mb)
	{}
}

void eth_filter_init(void)
{
	ETH->MACFFR |=
		/* only forwards frames that have passed the filter */
		eth_ra_off |
		/* marks frames that do not pass the source address filtering */
		eth_saf_off |
		/* does not forward any flow control frames */
		eth_pcf_off |
		/* receive broadcast frames */
		eth_bfd_on |
		/* normally filter result */
		eth_daif_off |
		/* disable promiscuous mode(mark filter result) */
		eth_pm_off |
		/* perfect multicast frames address filtering */
		eth_hm_off |
		/* perfect unicast frames address filtering */
		eth_hu_off;

	/* hash list registers */
	ETH->MACHTHR = eth_hth;
	ETH->MACHTLR = eth_htl;
}

void eth_control_init(void)
{
	ETH->MACCR |=
		/* watchdog enable(frame max size 2048 bytes) */
		eth_wd_off |
		/* jabber enable(close the transmitter if frame len more then 2048 bytes */
		eth_jd_off |
		/* frame gap */
		eth_ifg_96bit |
		/* speed */
		eth_fes_10mbs |
		/* self loop mode disable */
		eth_lm_off |
		/* full duplex mode */
		eth_dm_on |
		/* ipv4 checksum enable */
		eth_ipco_on |
		/* crc auto-stripping disable */
		eth_apcs_off;

	eth_flow_control_init();
}

static void eth_flow_control_init(void)
{
	ETH->MACFCR |=
		/* pause interval */ 
		eth_pt |
		/* disable auto-generation zero-value pause control frames */
		eth_zqpd_on |
		eth_plt_4ts |
		eth_upfd_off |
		/* does not parse the pause frames */
		eth_rfce_off |
		/* does not send the pause frames */
		eth_tfce_off;
}

void eth_dma_init(void)
{

	ETH->DMAOMR |=
		/* discard tcp/ip checksum error frames */
		eth_dtcefd_off |
		/* sending process will start after the complete frame is written into the FIFO */
		eth_tsf_on |
		/* discard error frames */
		eth_fef_on |
		/* forward short frames */
		eth_fugf_on;

	eth_dmatxdesc_init();
	eth_dmarxdesc_init();
}

void eth_dmarxdesc_init()
{
	uint32_t i;
	eth_dmadesc_t *rxdesc_ptr;
	for(i = 0; i < eth_rxbuff_num; i++) {
		rxdesc_ptr = rx_descriptor + i;
		rxdesc_ptr->rdes0 = eth_dmarxdesc_own;
		rxdesc_ptr->rdes1 =
			eth_dmarxdesc_rch |
			eth_rxbuff_size;
		rxdesc_ptr->rdes2 = (uint32_t)&(rx_buff[i * eth_rxbuff_size]); 
		if(i < (eth_rxbuff_num - 1)) {
			rxdesc_ptr->rdes3 = (uint32_t)(rx_descriptor + i + 1);
		} else {
			rxdesc_ptr->rdes3 = (uint32_t)rx_descriptor;
		}
	}

	rx_descriptor_ptr = rx_descriptor;
	ETH->DMARDLAR = (uint32_t)rx_descriptor;
}

void eth_dmatxdesc_init()
{
	uint32_t i;
	eth_dmadesc_t *txdesc_ptr;
	for(i = 0; i < eth_txbuff_num; i++) {
		txdesc_ptr = tx_descriptor + i;
		txdesc_ptr->rdes0 = eth_dmatxdesc_tch;
		txdesc_ptr->rdes2 = (uint32_t)&(tx_buff[i * eth_txbuff_size]); 

		if(i < (eth_txbuff_num - 1)) {
			txdesc_ptr->rdes3 = (uint32_t)(tx_descriptor + i + 1);
		} else {
			txdesc_ptr->rdes3 = (uint32_t)tx_descriptor;
		}
	}

	tx_descriptor_ptr = tx_descriptor;
	ETH->DMARDLAR = (uint32_t)tx_descriptor;
}


void eth_set_mac(const uint8_t *mac_addr)
{
	ETH->MACA0HR = 
		(uint32_t)(
		(mac_addr[4]) |
		(mac_addr[5] << 8));
	ETH->MACA0LR = 
		(uint32_t)(
		(mac_addr[0]) | 
		(mac_addr[1] << 8) | 
		(mac_addr[2] << 16) | 
		(mac_addr[3] << 24));
}

void eth_irq_init(void)
{
	ETH->MMCTIMR =
		eth_tgfm_on;
	ETH->MMCRIMR =
		eth_rgufm_on |
		eth_rfcem_on;

}

void eth_reset(void)
{
	ETH->DMABMR |= eth_softreset;
	while(ETH->DMABMR & eth_softreset)
	{}
}

void eth_phy_init(void)
{
	eth_write_phy_reg(eth_phy_addr, eth_phy_bmcr_addr, eth_phy_reset); 
}
