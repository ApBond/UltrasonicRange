#ifndef __TIMERS
#define __TIMERS

#include "stm32f411xe.h"

#define TIM3_ON TIM3->CR1|=TIM_CR1_CEN

void Tim3InitPWM(void);

#endif
