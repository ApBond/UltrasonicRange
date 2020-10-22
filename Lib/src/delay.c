#include "delay.h"

uint32_t delayInit()
{
	return SysTick_Config(SYS_CORE_FREQUENSY/1000);
}

void delay_ms(volatile uint32_t nTime)
{
	//SysTick_Config(SYS_CORE_FREQUENSY/1000);
	TimingDelay=nTime;
 	while(TimingDelay!=0){}
}

void delay_us(volatile uint32_t nTime)
{
	TimingDelay=nTime;
 	while(TimingDelay!=0){}
}

void timingDelayDecrement(void)
{
	if(TimingDelay!=0) TimingDelay--;
}

void SysTick_Handler(void)
{
	timingDelayDecrement();
}
