#include "systick.h"
#include "rcc.h"

enum {
	systick_ste_pos = 0,
	systick_ste = (1 << systick_ste_pos),
	systick_stie_pos = 1,
	systick_stie = (1 << systick_stie_pos),
	systick_stclk_pos = 2,
	systick_stclk_hclk= (1 << systick_stclk_pos),
	systick_stclk_hclkdiv8 = (0 << systick_stclk_pos),
	systick_stre_pos = 3,
	systick_stre = (1 << systick_stre_pos),
	systick_mode_pos = 4,
	systick_mode_up = (1 << systick_mode_pos),
	systick_mode_down = (0 << systick_mode_pos),
	systick_initial_pos = 5,
	systick_initial = (1 << systick_initial_pos),
	systick_swie_pos = 31,
	systick_swie = (1 << systick_swie_pos),
	systick_cntif_pos = 0,
	systick_cntif = (1 << systick_cntif_pos),
};

static volatile uint32_t ticks = 0;

void systick_init(const uint32_t freq)
{
	systick_set_freq(freq);
	SysTick->CTLR |= systick_stie | systick_stclk_hclk | systick_stre | systick_initial;
	systick_enable();
}

void SysTick_Handler()
{
#if 0
	if(SysTick->SR & systick_cntif) {
#endif
		uint32_t ticks = systick_get_ticks();
		uint32_t new_ticks = ticks + 1;
		systick_set_ticks(new_ticks);
#if 0
	}
#endif
}

void systick_delay_ms(const uint32_t ms)
{
	uint32_t start = ticks;
	uint32_t end = start + ms;

	if(end < start)
		while(ticks > start) {}

	while(ticks < end) {}
}

uint32_t systick_get_ticks()
{
	return ticks;
}

void systick_set_ticks(const uint32_t new_ticks)
{
	ticks = new_ticks;
}


void systick_enable()
{
	SysTick->CTLR |= systick_ste;
}

void systick_disable()
{
	SysTick->CTLR &= ~systick_ste;
}

void systick_set_freq(const uint32_t freq)
{
	uint32_t clk = rcc_get_system_clk();
	uint64_t cmp = clk / freq;
	SysTick->CMPLR = (cmp);
	SysTick->CMPHR = (cmp >> 32);
#if 0
	SysTick->CMP = cmp;
#endif
}
