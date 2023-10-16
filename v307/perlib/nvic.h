#ifndef NVIC_H_SENTRY
#define NVIC_H_SENTRY

#include "ch32v30x.h"

typedef int8_t nvic_irq_prior_t; 

enum {
	nvic_none_irq_prior = 0
};

typedef enum {
	nvic_none_irqn  	= 0,
	nvic_usart1_irqn = USART1_IRQn,
	nvic_usart2_irqn = USART2_IRQn,
	nvic_usart3_irqn = USART3_IRQn,
	nvic_tim6_irqn = TIM6_IRQn,
	nvic_tim7_irqn = TIM7_IRQn
} nvic_irqn_t;

void nvic_enable_irq(nvic_irqn_t irqn, nvic_irq_prior_t priority);
void nvic_disable_irq(nvic_irqn_t irqn);
void nvic_set_priority(nvic_irqn_t irqn, nvic_irq_prior_t priority);

#endif
