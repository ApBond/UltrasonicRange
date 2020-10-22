#include "main.h"

uint16_t range=0;
uint8_t flag=0;

void TIM4_IRQHandler(void)
{
	if((TIM4->SR &TIM_SR_UIF)!=0)
	{
		if(flag==2) 
		{
			EXTI->IMR&=~EXTI_IMR_IM0;
			range =0xFFFF;
			flag=1;
		}
		else
		{
			TIM4->SR&=~TIM_SR_UIF;//Сброс флага прерывания
			GPIOA->BSRR=GPIO_BSRR_BR1;//Срез измерительно импульса
			EXTI->IMR|=EXTI_IMR_IM0;//Разрешение EXTI0
		}
	}
}

void EXTI0_IRQHandler(void)
{
	EXTI->PR|=EXTI_PR_PR0;//Сброс флага прерывания
	if((GPIOA->IDR & GPIO_IDR_ID0) != 0)//Если прерывание по фронту
	{
		TIM4->ARR=0xFFFF;//Считаем до максимума
		TIM4->CNT=0;//Считаем с нуля
		TIM4->CR1|=TIM_CR1_CEN;//Запустить счет
		flag=2;
	}
	else//Если срез - счет окончен
	{
		TIM4->CR1&=~TIM_CR1_CEN;//Выключаем таймер
		range=TIM4->CNT;//Запоминаем время
		EXTI->IMR&=~EXTI_IMR_IM0;//Запрет EXTI0
		flag=1;
	}
}

void timer4Init(void)
{
	RCC->APB1ENR|=RCC_APB1ENR_TIM4EN;//Тактирование таймера 9
	TIM4->PSC=100;//Предделитель таймера
	TIM4->ARR=0xFFFF;
	TIM4->CR1|=TIM_CR1_CEN;
	TIM4->CR1|=TIM_CR1_OPM;//Режим одиночного счета
	TIM4->DIER|=TIM_DIER_UIE;//Включить прерывания по обновлению
	NVIC_EnableIRQ(TIM4_IRQn);
}

void scan(void)
{
	uint8_t i=0;
	TIM4->DIER|=TIM_DIER_UIE;
	TIM4->CNT=0;
	TIM4->ARR=10;
	GPIOA->BSRR=GPIO_BSRR_BS1;//Фронт измерительного импульса
	TIM4->CR1|=TIM_CR1_CEN;//Запустить счет
	for(i=0;i<60;i++)
	{
		if(flag==1) break;
		delay_ms(1);
	}
}

void triggerInit(void)
{
	SYSCFG->EXTICR[0]|=SYSCFG_EXTICR1_EXTI0_PA;//Выбор прерывания EXTI0 на PA0
	EXTI->FTSR|=EXTI_FTSR_TR0;//Разрешить прерывание по срезу
	EXTI->RTSR|=EXTI_RTSR_TR0;//Разрешить прерывание по фронту
	EXTI->IMR|=EXTI_IMR_IM0;//Разрешение EXTI0
	NVIC_EnableIRQ(EXTI0_IRQn);
}

int main()
{
	uint8_t buff[5];
	RccClockInit();
	delayInit();
	TFTInit();
	triggerInit();
	timer4Init();
	__enable_irq();
	GPIOA->MODER|=GPIO_MODER_MODE1_0;//PA1 в режив выходы
	GPIOA->OSPEEDR|=GPIO_OSPEEDER_OSPEEDR1;//Максимальная скоростьь на PA1
	GPIOA->PUPDR|=GPIO_PUPDR_PUPD0_1;//Pull down
	while(1)
	{
		scan();
		if(range!=0xFFFF)
		{
			//TFTDrawString(0,0,0x0000,0x0000,"            ",3);
			sprintf(buff,"%3.2f sm      ",(float)((float)range/58));
			TFTDrawString(0,0,0x0FE2,0x0000,(uint8_t*)buff,3);
		}
		else
		{
			TFTDrawString(0,0,0x1F38,0x00F8,"out of range",3);
		}
		delay_ms(300);
	}
}
