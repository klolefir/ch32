#ifndef RCC_H_SENTRY
#define RCC_H_SENTRY

#include "ch32v30x.h"

enum {
	rcc_prediv1_pos = 0,
	rcc_prediv1		= (0b0000 << rcc_prediv1_pos), /* div 1 */ 
	rcc_prediv2_pos = 4,
	rcc_prediv2 	= (0b1111 << rcc_prediv2_pos),
	rcc_pll2mul_pos = 8,
	rcc_pll2mul 	= (0b0000 << rcc_pll2mul_pos), /* mul 2.5(not used) */
	rcc_pll3mul_pos	= 12,
	rcc_pll3mul		= (0b1111 << rcc_pll3mul_pos),
	rcc_pllmul_pos	= 18,
	rcc_pllmul		= RCC_PLLMULL18_EXTEN,
	rcc_usbpsc_pos	= 22,
	rcc_usbpsc		= (0b11 << rcc_usbpsc_pos), /* div 3 */
	rcc_ahbdiv_pos = 4,
	rcc_ahbdiv	= RCC_HPRE_DIV1,
	rcc_apb1div_pos = 8,
	rcc_apb1div	= RCC_PPRE1_DIV1,
	rcc_apb2div_pos = 11,
	rcc_apb2div = RCC_PPRE2_DIV2,
	rcc_adcpsc_pos = 14,
	rcc_adcpsc 	= RCC_ADCPRE_DIV6,
	rcc_prediv1src_pos = 16,
	rcc_prediv1src_pll2 = (1 << rcc_prediv1src_pos),
	rcc_prediv1src_hse = (0 << rcc_prediv1src_pos),
	rcc_pllsrc_pos = 16,
	rcc_pllsrc_hse = (1 << rcc_pllsrc_pos),
	rcc_pllsrc_hsi = (0 << rcc_pllsrc_pos),

	rcc_pll3on_pos = 28,
	rcc_pll3on = (1 << rcc_pll3on_pos),

	rcc_pll3rdy_pos = 29,
	rcc_pll3rdy = (1 << rcc_pll3rdy_pos)
};

typedef enum {
	rcc_adcpsc_6 = (0b10 << rcc_adcpsc_pos)
} rcc_adcpsc_t;

typedef enum {
	rcc_usbpsc_3 = (0b11 << rcc_usbpsc_pos)
} rcc_usbpsc_t;

typedef enum {
	rcc_ahbdiv_1 = (0b0000 << rcc_ahbdiv_pos)
} rcc_ahbdiv_t;

typedef enum {
	rcc_apb1div_1 = (0b000 << rcc_apb1div_pos)
} rcc_apb1div_t;

typedef enum {
	rcc_apb2div_1 = (0b000 << rcc_apb2div_pos),
	rcc_apb2div_2 = (0b100 << rcc_apb2div_pos)
} rcc_apb2div_t;

typedef enum {
	rcc_pllmul_18 = (0b0000 << rcc_pllmul_pos),
	rcc_pllmul_3 = (0b0001 << rcc_pllmul_pos),
	rcc_pllmul_4 = (0b0010 << rcc_pllmul_pos),
} rcc_pllmul_t;

typedef enum {
	rcc_pll2mul_2dot5 = (0b0000 << rcc_pll2mul_pos),
	rcc_pll2mul_12dot5 = (0b0001 << rcc_pll2mul_pos),
} rcc_pll2mul_t;

typedef enum {
	rcc_prediv1_1 = (0b0000 << rcc_prediv1_pos),
	rcc_prediv1_2 = (0b0001 << rcc_prediv1_pos),
	rcc_prediv1_3 = (0b0010 << rcc_prediv1_pos),
} rcc_prediv1_t;

typedef enum {
	rcc_prediv2_2 	= (0b0001 << rcc_prediv2_pos)
} rcc_prediv2_t;

typedef enum {
	rcc_pll3mul_15 	= (0b1101 << rcc_pll3mul_pos)
} rcc_pll3mul_t;

typedef enum {
	rcc_none_id,
	rcc_tim6_id,
	rcc_tim7_id,
	rcc_gpioa_id,
	rcc_gpiob_id,
	rcc_gpioc_id,
	rcc_gpiod_id,
	rcc_gpioe_id,
	rcc_usart1_id,
	rcc_usart2_id,
	rcc_usart3_id,
	rcc_eth_id,
	rcc_ethrx_id,
	rcc_ethtx_id
} rcc_periph_id_t;

void rcc_deinit();
void rcc_init();
void rcc_pll3_enable();
void rcc_pll3_disable();
void rcc_prediv2_config(const rcc_prediv2_t div);
void rcc_pll3mul_config(const rcc_pll3mul_t mul);
void rcc_periph_enable(const rcc_periph_id_t id);
void rcc_periph_reset(const rcc_periph_id_t id);
uint32_t rcc_get_system_clk();

#endif
