#include "tim_hd.h"

void TIM6_IRQHandler(void)    __attribute__((interrupt(/*"WCH-Interrupt-fast"*/)));

static volatile uint32_t tim6_ticks = 0;
static volatile uint32_t tim7_ticks = 0;

uint32_t tim6_get_ticks()
{
	return tim6_ticks;
}

void tim6_set_ticks(const uint32_t ticks)
{
	tim6_ticks = ticks;
}

uint32_t tim7_get_ticks()
{
	return tim7_ticks;
}

void tim7_set_ticks(const uint32_t ticks)
{
	tim7_ticks = ticks;
}

#if 1
void TIM6_IRQHandler()
{
	uint32_t ticks;
	TIM_TypeDef *tim = TIM6;
	if(tim->INTFR & TIM_UIF) {
		ticks = tim6_get_ticks();
		tim6_set_ticks(ticks + 1);
	}
	tim->INTFR &= ~TIM_UIF;
}
#endif

void TIM7_IRQHandler()
{
	uint32_t ticks;
	TIM_TypeDef *tim = TIM7;
	if(tim->INTFR & TIM_UIF) {
		ticks = tim7_get_ticks();
		tim7_set_ticks(ticks + 1);
		tim->INTFR &= ~TIM_UIF;
	}
}
