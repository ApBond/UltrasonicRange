#include "timers.h"

//The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)

void Tim3InitPWM(void)
{
	RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;//Тактирование таймера 2
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;//Тактирование порта A и В
	GPIOA->MODER|=GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;//PA6,PA7,PB0 в режим альтернативной функции
	GPIOB->MODER|=GPIO_MODER_MODE0_1;
	GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7;//Высокая скорость портов PA6,PA7,PB0
	GPIOB->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR0;
	GPIOA->AFR[0]|=GPIO_AFRL_AFRL6_1 | GPIO_AFRL_AFRL7_1;//Альтернативная функция AF2 на PA6,PA7,PB0
	GPIOB->AFR[0]|=GPIO_AFRL_AFRL0_1;
	TIM3->ARR=2000;//Регистр автоперезагрузки
	TIM3->PSC=1000;//Настройка делителя
	TIM3->CR1&=~TIM_CR1_DIR;//Счет вверх
	TIM3->CR1 &= ~TIM_CR1_CMS;//Режим быстрого ШИМ
	TIM3->CCMR1|=TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;//CH1,CH2 - прямой ШИМ.
	TIM3->CCMR2|=TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;//CH3 - прямой ШИМ
	TIM3->CCER|=TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E;//Настройка каналов 1,2,3 на выход. Активный уровень - высокий.
	TIM3->CR1|=TIM_CR1_CEN;//Включить таймер
}