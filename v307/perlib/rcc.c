#include "rcc.h"
#include "general.h"

static uint32_t system_clk;

void rcc_deinit()
{
	RCC->CTLR |= RCC_HSION;
	while(!(RCC->CTLR & RCC_HSIRDY))
	{}
	RCC->CFGR2 = 0x00000000;
	RCC->CFGR0 = 0x00000000;
	while((RCC->CFGR0 & RCC_SWS) != RCC_SWS_HSI)
	{}
	RCC->CTLR &= ~RCC_PLLON;
	while(RCC->CTLR & RCC_PLLRDY)
	{}
	RCC->CTLR &= ~RCC_HSEON;
	while(RCC->CTLR & RCC_HSERDY)
	{}
	RCC->CTLR &= ~(RCC_CSSON | RCC_HSEBYP);
}

void rcc_init()
{
	rcc_deinit();
	RCC->CTLR |= RCC_HSEON;
	while(!(RCC->CTLR & RCC_HSERDY))
	{}
	RCC->CFGR2 |=
		rcc_prediv1src_hse |
		rcc_prediv1_1 |
		rcc_prediv2_2	|
		rcc_pll3mul_15	|
		rcc_pll2mul_2dot5;
	RCC->CFGR0 |=
		rcc_pllsrc_hse |
		rcc_pllmul_18 |
		rcc_ahbdiv_1 |
		rcc_apb1div_1 |
		rcc_apb2div_2 |
		rcc_usbpsc_3 |
		rcc_adcpsc_6;	

	RCC->CTLR |= rcc_pll3on;
	while(!(RCC->CTLR & rcc_pll3rdy))
	{}

	RCC->CTLR |= RCC_PLLON;
	while(!(RCC->CTLR & RCC_PLLRDY))
	{}
	RCC->CFGR0 |= RCC_SW_PLL;
	while((RCC->CFGR0 & RCC_SWS) != RCC_SWS_PLL)
	{}
	system_clk = 144000000;
}

void rcc_periph_enable(const rcc_periph_id_t id)
{
	switch(id) {
	case rcc_gpioa_id:	RCC->APB2PCENR |= RCC_IOPAEN;
						break;

	case rcc_gpiob_id:	RCC->APB2PCENR |= RCC_IOPBEN;	
						break;

	case rcc_gpioc_id:	RCC->APB2PCENR |= RCC_IOPCEN;
						break;

	case rcc_gpiod_id:	RCC->APB2PCENR |= RCC_IOPDEN;
						break;

	case rcc_gpioe_id:	RCC->APB2PCENR |= RCC_IOPEEN;
						break;

	case rcc_usart1_id:	RCC->APB2PCENR |= RCC_USART1EN;
						break;

	case rcc_usart2_id:	RCC->APB1PCENR |= RCC_USART2EN;
						break;

	case rcc_usart3_id:	RCC->APB1PCENR |= RCC_USART3EN;
						break;

	case rcc_tim6_id:	RCC->APB1PCENR |= RCC_TIM6EN;
						break;

	case rcc_tim7_id:	RCC->APB1PCENR |= RCC_TIM7EN;
						break;

	case rcc_eth_id:	RCC->AHBPCENR |= RCC_ETH_MACEN;
						break;

	case rcc_ethrx_id:	RCC->AHBPCENR |= RCC_ETH_MAC_RXEN;
						break;

	case rcc_ethtx_id:	RCC->AHBPCENR |= RCC_ETH_MAC_TXEN;
						break;

	case rcc_none_id:	break;
	}
}

void rcc_periph_reset(const rcc_periph_id_t id)
{
	switch(id) {
	case rcc_gpioa_id:	RCC->APB2PRSTR |= RCC_IOPARST;
						break;

	case rcc_gpiob_id:	RCC->APB2PRSTR |= RCC_IOPBRST;	
						break;

	case rcc_gpioc_id:	RCC->APB2PRSTR |= RCC_IOPCRST;
						break;

	case rcc_gpiod_id:	RCC->APB2PRSTR |= RCC_IOPDRST;
						break;

	case rcc_gpioe_id:	RCC->APB2PRSTR |= RCC_IOPERST;
						break;

	case rcc_usart1_id:	RCC->APB2PRSTR |= RCC_USART1RST;
						break;

	case rcc_usart2_id:	RCC->APB1PRSTR |= RCC_USART2RST;
						break;

	case rcc_usart3_id:	RCC->APB1PRSTR |= RCC_USART3RST;
						break;

	case rcc_eth_id:	RCC->AHBRSTR |= RCC_ETH_MACRST;
						break;

	case rcc_none_id:	
	default:			break;
	}
}

uint32_t rcc_get_system_clk()
{
	return system_clk;
}

uint32_t rcc_get_apb1_clk()
{
	return system_clk;
}

uint32_t rcc_get_apb2_clk()
{
	return system_clk / 2;
}

void rcc_pll3_enable()
{
	RCC->CTLR |= rcc_pll3on;
}

void rcc_pll3_disable()
{
	RCC->CTLR &= ~rcc_pll3on;
}

void rcc_prediv2_config(const rcc_prediv2_t div)
{
	RCC->CFGR2 &= ~rcc_prediv2;
	RCC->CFGR2 |= div;
}

void rcc_pll3mul_config(const rcc_pll3mul_t mul)
{
	RCC->CFGR2 &= ~rcc_pll3mul;
	RCC->CFGR2 |= mul;
}
