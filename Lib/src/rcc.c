#include "rcc.h"

int RccClockInit(void)
{
	//Enable HSE
	//Setting PLL
	//Enable PLL
	//Setting count wait cycles of FLASH
	//Setting AHB1,AHB2 prescaler
	//Switch to PLL	
	uint16_t timeDelay;
	RCC->APB1ENR|=RCC_APB1ENR_PWREN;
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOHEN;
	RCC->APB2ENR|=RCC_APB2ENR_SYSCFGEN;
	RCC->CR|=RCC_CR_HSEBYP;//HSE Bypass
	RCC->CR|=RCC_CR_HSEON;//Enable HSE
	for(timeDelay=0;;timeDelay++)
	{
		if(RCC->CR&RCC_CR_HSERDY) break;
		if(timeDelay>0x1000)
		{
			RCC->CR&=~RCC_CR_HSEON;
			return 1;
		}
	}	
	RCC->PLLCFGR=0;
	RCC->PLLCFGR|=100<<RCC_PLLCFGR_PLLN_Pos | 4<<RCC_PLLCFGR_PLLM_Pos | RCC_PLLCFGR_PLLSRC_HSE;//PLLx100, PLL div /2, PLL source:HSE
	RCC->CR|=RCC_CR_PLLON;//Enable PLL
	for(timeDelay=0;;timeDelay++)
	{
		if(RCC->CR&RCC_CR_PLLRDY) break;
		if(timeDelay>0x1000)
		{
			RCC->CR&=~RCC_CR_HSEON;
			RCC->CR&=~RCC_CR_PLLON;
			return 2;
		}
	}
	FLASH->ACR|=3<<FLASH_ACR_LATENCY_Pos;
	RCC->CFGR|=RCC_CFGR_PPRE1_DIV2;//APB1 prescaler=2
	RCC->CFGR|=RCC_CFGR_SW_PLL;//Switch to PLL
	while((RCC->CFGR&RCC_CFGR_SWS)!=(0x02<<2)){}
	RCC->CR&=~RCC_CR_HSION;//Disable HSI
	return 0;
}
