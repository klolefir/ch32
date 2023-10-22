#include "eth.h"
#include "rcc.h"
#include "nvic.h"
#include "kestring.h"
#include "tim.h"

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
	eth_wd = eth_wd_on,

	eth_jd_pos = 22,
	eth_jd_on = (1 << eth_jd_pos),
	eth_jd_off = (0 << eth_jd_pos),
	eth_jd = eth_jd_on,

	eth_pr_pos = 20,
	eth_pr_on = (1 << eth_pr_pos),
	eth_pr_off = (0 << eth_pr_pos),
	eth_pr = eth_pr_on,

	eth_ifg_pos = 17,
	eth_ifg_96bit = (0b000 << eth_ifg_pos),

	eth_ipco_pos = 10,
	eth_ipco_on = (1 << eth_ipco_pos),
	eth_ipco_off = (0 << eth_ipco_pos),
	eth_ipco = eth_ipco_on,

	eth_dm_pos = 11,
	eth_dm_on = (1 << eth_dm_pos),
	eth_dm_off = (0 << eth_dm_pos),
	eth_dm = eth_dm_on,

	eth_fes_pos = 14,
	eth_fes_10mbs = (0b00 << eth_fes_pos),
	eth_fes_100mbs = (0b01 << eth_fes_pos),
	eth_fes_1gbs = (0b10 << eth_fes_pos),
	eth_fes = (0b11 << eth_fes_pos),

	eth_lm_pos = 12,
	eth_lm_on = (1 << eth_lm_pos),
	eth_lm_off = (0 << eth_lm_pos),
	eth_lm = eth_lm_on,

	eth_apcs_pos = 7,
	eth_apcs_on = (1 << eth_apcs_pos),
	eth_apcs_off = (0 << eth_apcs_pos),
	eth_apcs = eth_apcs_on,

	eth_te_pos = 3,
	eth_te_on = (1 << eth_te_pos),
	eth_te_off = (0 << eth_te_pos),
	eth_te = eth_te_on,

	eth_re_pos = 2,
	eth_re_on = (1 << eth_re_pos),
	eth_re_off = (0 << eth_re_pos),
	eth_re = eth_re_on,

	/*MACFFR*/
	eth_saf_pos = 9,
	eth_saf_on = (1 << eth_saf_pos),
	eth_saf_off = (0 << eth_saf_pos),
	eth_saf = eth_saf_on,

	eth_pcf_pos = 6,
	eth_pcf_off = (0b00 << eth_pcf_pos),
	eth_pcf_on = (0b10 << eth_pcf_pos),
	eth_pcf_filter = (0b11 << eth_pcf_pos),
	eth_pcf = (0b11 << eth_pcf_pos),

	eth_bfd_pos = 5,
	eth_bfd_on = (1 << eth_bfd_pos),
	eth_bfd_off = (0 << eth_bfd_pos),
	eth_bfd = eth_bfd_on,

	eth_daif_pos = 3,
	eth_daif_on = (1 << eth_daif_pos),
	eth_daif_off = (0 << eth_daif_pos),
	eth_daif = eth_daif_on,

	eth_ra_pos = 31,
	eth_ra_on = (1 << eth_ra_pos),
	eth_ra_off = (0 << eth_ra_pos),
	eth_ra = eth_ra_on,

	eth_pm_pos = 0,
	eth_pm_on = (1 << eth_pm_pos),
	eth_pm_off = (0 << eth_pm_pos),
	eth_pm = eth_pm_on,

	eth_hu_pos = 1,
	eth_hu_on = (1 << eth_hu_pos),
	eth_hu_off = (0 << eth_hu_pos),
	eth_hu = eth_hu_on,

	eth_hm_pos = 2,
	eth_hm_on = (1 << eth_hm_pos),
	eth_hm_off = (0 << eth_hm_pos),
	eth_hm = eth_hm_on,

	/*MACFCR*/
	eth_zqpd_pos = 7,
	eth_zqpd_on = (1 << eth_zqpd_pos),
	eth_zqpd_off = (0 << eth_zqpd_pos),
	eth_zqpd = eth_zqpd_on,

	eth_pt_pos = 16,
	eth_pt = (0xFFFF << eth_pt_pos),

	eth_plt_pos = 4,
	eth_plt_4ts = (0b00 << eth_plt_pos),
	eth_plt = (0b11 << eth_plt_pos),

	eth_upfd_pos = 3,
	eth_upfd_on = (1 << eth_upfd_pos),
	eth_upfd_off = (0 << eth_upfd_pos),
	eth_upfd = eth_upfd_on,

	eth_rfce_pos = 2,
	eth_rfce_on = (1 << eth_rfce_pos),
	eth_rfce_off = (0 << eth_rfce_pos),
	eth_rfce = eth_rfce_on,

	eth_tfce_pos = 1,
	eth_tfce_on = (1 << eth_tfce_pos),
	eth_tfce_off = (0 << eth_tfce_pos),
	eth_tfce = eth_tfce_on,

	/* DMAIER */
	eth_ple_pos = 31,
	eth_ple_on = (1 << eth_ple_pos),
	eth_ple_off = (0 << eth_ple_pos),
	eth_ple = eth_ple_on,

	eth_nise_pos = 16,
	eth_nise_on = (1 << eth_nise_pos),
	eth_nise_off = (0 << eth_nise_pos),
	eth_nise = eth_nise_on,

	eth_aise_pos = 15,
	eth_aise_on = (1 << eth_aise_pos),
	eth_aise_off = (0 << eth_aise_pos),
	eth_aise = eth_aise_on,

	eth_rbuie_pos = 7,
	eth_rbuie_on = (1 << eth_rbuie_pos),
	eth_rbuie_off = (0 << eth_rbuie_pos),
	eth_rbuie = eth_rbuie_on,

	eth_rie_pos = 6,
	eth_rie_on = (1 << eth_rie_pos),
	eth_rie_off = (0 << eth_rie_pos),
	eth_rie = eth_rie_on,

	eth_tuie_pos = 5,
	eth_tuie = (1 << eth_tuie_pos),

	eth_tjtie_pos = 3,
	eth_tjtie = (1 << eth_tjtie_pos),

	eth_tbuie_pos = 2,
	eth_tbuie_on = (1 << eth_tbuie_pos),	
	eth_tbuie_off = (0 << eth_tbuie_pos),	
	eth_tbuie = eth_tbuie_on,

	eth_tpsie_pos = 1,
	eth_tpsie = (1 << eth_tpsie_pos),

	eth_tie_pos = 0,
	eth_tie_on = (1 << eth_tie_pos),
	eth_tie_off = (0 << eth_tie_pos),
	eth_tie = eth_tie_on,

	/* DMABMR */
	eth_softreset_pos = 0,
	eth_softreset = (1 << eth_softreset_pos),
	
	/* DMAOMR */
	eth_dtcefd_pos = 26,
	eth_dtcefd_on = (1 << eth_dtcefd_pos),
	eth_dtcefd_off = (0 << eth_dtcefd_pos),
	eth_dtcefd = eth_dtcefd_on,

	eth_ftf_pos = 20,
	eth_ftf = (1 << eth_ftf_pos),

	eth_st_pos = 13,
	eth_st_on = (1 << eth_st_pos),
	eth_st_off = (0 << eth_st_pos),
	eth_st = eth_st_on,

	eth_sr_pos = 1,
	eth_sr_on = (1 << eth_sr_pos),
	eth_sr_off = (0 << eth_sr_pos),
	eth_sr = eth_sr_on,

	/* transmit store forward */
	eth_tsf_pos = 21,
	eth_tsf_on = (1 << eth_tsf_pos),
	eth_tsf_off = (0 << eth_tsf_pos),
	eth_tsf = eth_tsf_on,

	eth_fef_pos = 7,
	eth_fef_on = (1 << eth_fef_pos),
	eth_fef_off = (0 << eth_fef_pos),
	eth_fef = eth_fef_on,

	eth_fugf_pos = 6,
	eth_fugf_on = (1 << eth_fugf_pos),
	eth_fugf_off = (0 << eth_fugf_pos),
	eth_fugf = eth_fugf_on,

	/* MACHTHR */
	eth_hth = 0,

	/* MACHTLR */
	eth_htl = 0,

	eth_rgufm_pos = 17,
	eth_rgufm_on = (1 << eth_rgufm_pos),
	eth_rgufm_off = (0 << eth_rgufm_pos),
	eth_rgufm = eth_rgufm_on,

	eth_rfcem_pos = 5,
	eth_rfcem_on = (1 << eth_rfcem_pos),
	eth_rfcem_off = (0 << eth_rfcem_pos),
	eth_rfcem = eth_rfcem_on,

	/* MMCTIMR */
	
	eth_tgfm_pos = 5,
	eth_tgfm_on = (1 << eth_tgfm_pos),
	eth_tgfm_off = (0 << eth_tgfm_pos),
	eth_tgfm = eth_tgfm_on,

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

	eth_dmatxdesc_ic_pos = 30,
	eth_dmatxdesc_ic = (1 << eth_dmatxdesc_ic_pos),

	eth_dmatxdesc_ls_pos = 29,
	eth_dmatxdesc_ls = (1 << eth_dmatxdesc_ls_pos),

	eth_dmatxdesc_fs_pos = 28,
	eth_dmatxdesc_fs = (1 << eth_dmatxdesc_fs_pos),

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

#if 0
	ETH->MACCR &= ~eth_dm; /* strange(check auto neg) */
	ETH->MACCR &= ~(eth_fes_100mbs | eth_fes_1gbs);
#endif

	eth_start();
	eth_phy_init();
}

uint32_t eth_receive(uint8_t *recv_data)
{
	uint32_t size;

	if(!(rx_descriptor_ptr->rdes0 & eth_dmarxdesc_own) && /* is owned */
		!(rx_descriptor_ptr->rdes0 & eth_dmarxdesc_es) &&  /*is error*/
		(rx_descriptor_ptr->rdes0 & eth_dmarxdesc_ls) && /*is last desc*/ 
		(rx_descriptor_ptr->rdes0 & eth_dmarxdesc_fs)) /*is first desc*/
	{
		size = ((rx_descriptor_ptr->rdes0 & eth_dmarxdesc_fl) >> eth_dmarxdesc_fl_pos) - 4;
		kememcpy(recv_data, (uint8_t *)(rx_descriptor_ptr->rdes2), size);
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

	kememcpy((uint8_t *)(tx_descriptor_ptr->tdes2), send_data, size);
	tx_descriptor_ptr->tdes1 = (size & (eth_dmatxdesc_tbs1 >> eth_dmatxdesc_tbs1_pos));
	tx_descriptor_ptr->tdes0 |=
		eth_dmatxdesc_ls |
		eth_dmatxdesc_fs;

	tx_descriptor_ptr->tdes0 |=
		eth_dmatxdesc_own;

#if 0
	if(ETH->DMASR & eth_tbus) {
		ETH->DMASR = eth_tbus;
		ETH->DMATPDR = 0;
	}
#else
	ETH->DMASR = eth_tbus;
	ETH->DMATPDR = 0;
#endif

	tx_descriptor_ptr = (eth_dmadesc_t *)(tx_descriptor_ptr->tdes3)	;
}

void eth_rcc_init()
{
	rcc_periph_enable(rcc_eth_id);
	rcc_periph_enable(rcc_ethrx_id);
	rcc_periph_enable(rcc_ethtx_id);
}

void eth_start()
{
	ETH->MACCR |= eth_te; /* enable tx */
	ETH->DMAOMR |= eth_ftf; /* reset fifo */
	ETH->MACCR |= eth_re; /* enable rx */
	ETH->DMAOMR |= eth_st; /* enable tx dma */
	ETH->DMAOMR |= eth_sr; /* enable rx dma */
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
		/*eth_ra_on |*/
		/* marks frames that do not pass the source address filtering */
		eth_saf_off |
		/* does not forward any flow control frames */
		eth_pcf_off |
		/* receive broadcast frames */
		eth_bfd_off |
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
		/* connect internal 50-ohm phy resistor */
		eth_pr |
		/* frame gap */
		eth_ifg_96bit |
		/* speed */
		eth_fes_10mbs |
		/* full duplex mode */
		eth_dm |
		/* ipv4 checksum enable */
		eth_ipco;

	ETH->MACCR &= ~(
		/* watchdog enable(frame max size 2048 bytes) */
		eth_wd |
		/* jabber enable(close the transmitter if frame len more then 2048 bytes */
		eth_jd |
		/* self loop mode disable */
		eth_lm |
		/* crc auto-stripping disable */
		eth_apcs);

	eth_flow_control_init();
}

void eth_flow_control_init(void)
{
	ETH->MACFCR |=
		/* pause interval */ 
		eth_pt |
		/* disable auto-generation zero-value pause control frames */
		eth_zqpd |
		eth_plt_4ts;

	ETH->MACFCR &= ~(
		eth_upfd |
		/* does not parse the pause frames */
		eth_rfce |
		/* does not send the pause frames */
		eth_tfce);
}

void eth_dma_init(void)
{

	ETH->DMAOMR |=
		/* sending process will start after the complete frame is written into the FIFO */
		eth_tsf |
		/* discard error frames */
		eth_fef |
		/* forward short frames */
		eth_fugf;

	ETH->DMAOMR &= ~(
		/* discard tcp/ip checksum error frames */
		eth_dtcefd);

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
		txdesc_ptr->tdes0 |=
			eth_dmatxdesc_tch |
			eth_dmatxdesc_ic;
		txdesc_ptr->tdes2 = (uint32_t)&(tx_buff[i * eth_txbuff_size]); 

		if(i < (eth_txbuff_num - 1)) {
			txdesc_ptr->tdes3 = (uint32_t)(tx_descriptor + i + 1);
		} else {
			txdesc_ptr->tdes3 = (uint32_t)tx_descriptor;
		}
	}

	tx_descriptor_ptr = tx_descriptor;
	ETH->DMATDLAR = (uint32_t)tx_descriptor;
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
		eth_tgfm;
	ETH->MMCRIMR =
		eth_rgufm|
		eth_rfcem;

	ETH->DMASR = 0xFFFFFFFF;
#if 1
	ETH->DMAIER |=
		eth_nise |
		eth_aise |
		eth_tuie |
		eth_tjtie |
		eth_tbuie |
		eth_tpsie |
		eth_tie |
		eth_ple;
	nvic_enable_irq(nvic_eth_irqn, eth_irq_priority);
#else
	ETH->DMAIER |=
		/* normal interrupts */
		eth_nise_on |
		/* abnormal interrupts */
		eth_aise_on |
		/* receive interrupt */
		eth_rie_on |
		/* transmit interrupt */
		eth_tie_on |
		/* receive buffer unavailable interrupt */
		eth_rbuie_on |
		/* internal phy link status change interrupt */
		eth_ple_on;
	nvic_enable_irq(nvic_eth_irqn, eth_irq_priority);
#endif
}

void eth_reset(void)
{
	ETH->DMABMR |= eth_softreset;
	while(ETH->DMABMR & eth_softreset)
	{}
}

void eth_phy_init(void)
{
	uint16_t val;
	eth_write_phy_reg(phy_addr, phy_bmcr_addr, phy_reset); 
	tim_delay_ms(500);
#if 0
    eth_write_phy_reg(phy_addr, phy_mdix_addr, phy_tr_auto);
	tim_delay_ms(500);
#endif
#if 0
	eth_write_phy_reg(phy_addr, phy_bmcr_addr, phy_resautoneg);
	eth_write_phy_reg(phy_addr, phy_mdix_addr,
			phy_tr_auto |
			phy_polarity_normal);
	eth_write_phy_reg(phy_addr, phy_physr_addr, &val); 
#endif
	do {
		eth_read_phy_reg(phy_addr, phy_bmsr_addr, &val);
	} while(!(val & phy_autonegcmpl));

}
