#include "encoder.h"


void encoderInit(void)
{
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;//Тактирование порта A
	RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;//Такстирование таймера 2
	GPIOA->MODER|=GPIO_MODER_MODE0_1 | GPIO_MODER_MODE1_1;//PA0,PA1 в режим алдьтернативных функцияй
	GPIOA->AFR[0]|=GPIO_AFRL_AFRL0_0 | GPIO_AFRL_AFRL1_0;//PA0,PA1 - AF1
	GPIOA->PUPDR|=GPIO_PUPDR_PUPD0_0 | GPIO_PUPDR_PUPD1_0;//PA0,PA1 Pull up
	TIM2->CCER = TIM_CCER_CC1P | TIM_CCER_CC2P;//Инвертированный режим (по срезу)
	TIM2->CCMR1 = TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;//Сконфигурировать CH0,CH1 для работы по триггеру
	TIM2->SMCR = TIM_SMCR_SMS_1;//Режим энкодера
	TIM2->CCMR1|=0xE<<TIM_CCMR1_IC1F_Pos | 0xE<<TIM_CCMR1_IC2F_Pos;//Настройка входных фильтров
	TIM2->ARR = 125;
	//Верхний предел счета
	TIM2->CR1|= TIM_CR1_CEN;//Старт таймера
		/*NVIC_EnableIRQ(EXTI0_IRQn);
	//SYSCFG->EXTICR[0]|=SYSCFG_EXTICR1_EXTI0_Pos
	EXTI->IMR|=EXTI_IMR_IM0;
	EXTI->FTSR|=EXTI_FTSR_TR0;
	//EXTI->RTSR|=EXTI_RTSR_TR0;*/
}

/*void EXTI0_IRQHandler(void)
{
	uint32_t i;
	EXTI->PR|=EXTI_PR_PR0;
	for(i=0;i<0x7FFF;i++);
	if(GPIOA->IDR & GPIO_IDR_ID0)
	{
		if(!(GPIOA->IDR & GPIO_IDR_ID1))
		{
			encoderCount++;
		}
	}
}*/