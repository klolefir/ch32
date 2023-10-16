#include "stddef.h"
#include "tim.h"
#include "tim_hd.h"
#include "nvic.h"
#include "rcc.h"


static TIM_TypeDef *switch_tim(const tim_t *tim_set);
static nvic_irqn_t switch_irqn(const tim_t *tim_set);
static nvic_irq_prior_t switch_irq_prior(const tim_t *tim_set);
static rcc_periph_id_t switch_rcc_id(const tim_t *tim_set);
static void tim_rcc_init(const tim_t *tim_set);
static void irq_init(const tim_t *tim_set);
static void freq_init(const tim_t *tim_set);

void tim_init(const tim_t *tim_set)
{
	tim_rcc_init(tim_set);
	TIM_TypeDef *tim = switch_tim(tim_set);
#if 0
	tim->CTLR1 &= ~(TIM_OPM | TIM_UDIS);
	tim->CTLR1 |= TIM_ARPE | TIM_URS;
#endif
	tim->CTLR1 &= TIM_UDIS;
	tim->CTLR1 |= TIM_ARPE | TIM_URS;
	irq_init(tim_set);
	freq_init(tim_set);
	tim_enable(tim_set);
	tim->SWEVGR = TIM_UG;
}

void tim_hold_init()
{
	tim_t hold_tim;
	hold_tim.tim = tim_num_6;
	hold_tim.psc = 16800;
	hold_tim.arr = 10;
	hold_tim.irq_state = tim_irq_on;

	tim_init(&hold_tim);
}
void tim_delay_ms(const uint32_t ms)
{
	uint32_t time = 0;
	tim6_set_ticks(time);
	while(time != ms) {
		time = tim6_get_ticks();
	}
}

void tim_set_freq(const tim_t *tim_set, uint32_t freq)
{
	TIM_TypeDef *tim = switch_tim(tim_set);
	tim->PSC = (uint16_t)(tim_set->psc - 1);
	tim->ATRLR = (uint16_t)(tim_set->arr - 1);
}

void tim_enable(const tim_t *tim_set)
{
	TIM_TypeDef *tim = switch_tim(tim_set);
	tim->CTLR1 |= TIM_CEN;
}

void tim_disable(const tim_t *tim_set)
{
	TIM_TypeDef *tim = switch_tim(tim_set);
	tim->CTLR1 &= ~TIM_CEN;
}

void tim_enable_irq(tim_t *tim_set)
{
	TIM_TypeDef *tim = switch_tim(tim_set);
	tim_set->irq_state = tim_irq_on;
	tim->INTFR &= ~TIM_UIF;
	tim->DMAINTENR &= ~TIM_UIE;
}

void tim_disable_irq(tim_t *tim_set)
{
	TIM_TypeDef *tim = switch_tim(tim_set);
	tim_set->irq_state = tim_irq_off;
	tim->DMAINTENR &= ~TIM_UIE;
}

uint32_t tim_get_ticks(const tim_t *tim_set)
{
	uint32_t ticks;
	switch(tim_set->tim) {
	case tim_num_6:	ticks = tim6_get_ticks();
					break;
	case tim_num_7:	ticks = tim7_get_ticks();
					break;
	default:		ticks = 0;
					break;
	}

	return ticks;
}

void tim_set_ticks(const tim_t *tim_set, const uint32_t ticks)
{
	switch(tim_set->tim) {
	case tim_num_6:	tim6_set_ticks(ticks);
					break;
	case tim_num_7:	tim7_set_ticks(ticks);
					break;
	default:		return;
	}
}

TIM_TypeDef *switch_tim(const tim_t *tim_set)
{
	switch(tim_set->tim) {
	case tim_num_6:	return TIM6;
	case tim_num_7: return TIM7;
	default:		return 0;
	}
}

nvic_irqn_t switch_irqn(const tim_t *tim_set)
{
	switch(tim_set->tim) {
	case tim_num_6:	return nvic_tim6_irqn;
	case tim_num_7: return nvic_tim7_irqn;
	default:		return nvic_none_irqn;
	}
}

nvic_irq_prior_t switch_irq_prior(const tim_t *tim_set)
{
	switch(tim_set->tim) {
	case tim_num_6:	return tim6_irq_prior;	
	case tim_num_7: return tim7_irq_prior;
	default:		return nvic_none_irq_prior;
	}
}

rcc_periph_id_t switch_rcc_id(const tim_t *tim_set)
{
	switch(tim_set->tim) {
	case tim_num_6:	return rcc_tim6_id;
	case tim_num_7: return rcc_tim7_id;
	default:		return rcc_none_id;
	}
}

void tim_rcc_init(const tim_t *tim_set)
{
	rcc_periph_id_t tim_id = switch_rcc_id(tim_set);
	rcc_periph_enable(tim_id);
}

void irq_init(const tim_t *tim_set)
{
	TIM_TypeDef *tim = switch_tim(tim_set);
	nvic_irqn_t tim_irqn = switch_irqn(tim_set);
	nvic_irq_prior_t tim_irq_prior = switch_irq_prior(tim_set);
	nvic_enable_irq(tim_irqn, tim_irq_prior);

	tim->INTFR &= ~TIM_UIF; /*reset interrupt status*/
	switch(tim_set->irq_state) {
	case tim_irq_on:	tim->DMAINTENR |= TIM_UIE;
						break;
	case tim_irq_off:	tim->DMAINTENR &= ~TIM_UIE;
						break;
	}
}

void freq_init(const tim_t *tim_set)
{
	TIM_TypeDef *tim = switch_tim(tim_set);
	tim->PSC = (uint16_t)(tim_set->psc - 1);
	tim->ATRLR = (uint16_t)(tim_set->arr - 1);
}
