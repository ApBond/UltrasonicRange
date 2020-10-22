#include "adc.h"

void adcInit(uint8_t channel)
{
	uint16_t timeout;
	RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;//Включить тактирование ADC1
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;//Включить тактирование GPIOA
	GPIOA->MODER|=GPIO_MODER_MODE0<<(channel*2);
	ADC->CCR|=ADC_CCR_ADCPRE_0;//Делитель АЦП на 4
	ADC1->CR2|=ADC_CR2_ADON;//Включить АЦП
	ADC1->CR1|=ADC_CR1_RES_1//8 ми битный режим АЦП
	| ADC_CR1_SCAN//Режим сканирования
	| ADC_CR1_JEOCIE;//Включить прерывание по окончанию преобразования инжективного канала
	ADC1->JSQR|=(channel<<ADC_JSQR_JSQ4_Pos);//Выбор канала
	NVIC_EnableIRQ(ADC_IRQn);
	ADC1->CR2|=ADC_CR2_JSWSTART;//Старт преобразования инжективной группы
}

