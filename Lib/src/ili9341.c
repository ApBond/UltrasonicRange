#include "ili9341.h"

void TFTGpioInit(void)
{
	//PA5-SCK
	//PA7-MOSI
	//PB6-CS
	//RESET-PB8
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN;//Тактирование порта B и порта А
	GPIOA->MODER|=GPIO_MODER_MODE5_1 | GPIO_MODER_MODE7_1;//PA5,PA7 в режим альтернативной функции
	GPIOB->MODER|=GPIO_MODER_MODE6_0 | GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0;//PB6,PB8, PB9 в режим выхода
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR5 |  GPIO_PUPDR_PUPDR7);
	GPIOA->OSPEEDR|= GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR7;//Высокая скорость портов
	GPIOB->OSPEEDR|= GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9;
	GPIOA->AFR[0]|=GPIO_AFRL_AFRL5_0 | GPIO_AFRL_AFRL5_2 | GPIO_AFRL_AFRL7_0 | GPIO_AFRL_AFRL7_2;//Настройка альтернативных функций
}

void TFTspi1Init(void)
{
	//Stream 2 channel 2
	RCC->APB2ENR|=RCC_APB2ENR_SPI1EN;//Включить тактирование SPI1
	SPI1->CR1|=SPI_CR1_MSTR//Режим ведущего
	//| (SPI_CR1_BR)//Делитель частоты 
	| SPI_CR1_SSM//Программный NSS
	| SPI_CR1_SSI//NSS - high
	| SPI_CR1_SPE;//включить SPI1
	//SPI1->CR2|=SPI_CR2_TXDMAEN;//Разрешить вызов DMA
}

uint8_t TFTspiTransmit(uint8_t data)
{
	
	while(!(SPI1->SR & SPI_SR_TXE));
	*(volatile uint8_t *)&(SPI1->DR) = data;	
	while(!(SPI1->SR & SPI_SR_RXNE));

	return *(volatile uint8_t *)&(SPI1->DR);
}

void TFTSendCommand(uint8_t command)
{
    TFT_COMMAND;
		TFT_SELECT;
    TFTspiTransmit(command);
		TFT_DESELECT;
    TFT_DATA;
}

void TFTSendData(uint8_t data)
{
		TFT_SELECT;
		TFTspiTransmit(data);
		TFT_DESELECT;
}

void TFTSend16bData(uint16_t data)
{
    TFTSendData(data>>8);
    TFTSendData((uint8_t)data);
}

void TFTInit(void)
{
	TFTGpioInit();
	TFTspi1Init();
	TFT_RESET_SET;
	TFT_DATA;
  TFTSendCommand(0x01);
	//Power Control 1
	TFTSendCommand(0xC0);	
	TFTSendData(0x25);
  delay_ms(5);
	//Power Control 2
	TFTSendCommand(0xC1);	
	TFTSendData(0x11);
  delay_ms(5);
	//VCOM Control 1
	TFTSendCommand(0xC5);
	TFTSendData(0x2B);
	TFTSendData(0x2B); 
  delay_ms(5);
	//VCOM Control 2
	TFTSendCommand(0xC7);
	TFTSendData(0x86); 
  delay_ms(5);
	//COLMOD: Pixel Format Set
	TFTSendCommand(0x3A);
	TFTSendData(0x05);	
  delay_ms(5);
	//Frame Rate Control 
	TFTSendCommand(0xB1);
	TFTSendData(0x00);
	TFTSendData(0x18);
  delay_ms(5);
	//Display Function Control
	TFTSendCommand(0xB6);
	TFTSendData(0x0A);
	TFTSendData(0x82);
	TFTSendData(0x27);
  delay_ms(5);
	// Sleep Out
	TFTSendCommand(0x11);
	delay_ms(5);
	//Display On
	TFTSendCommand(0x29);
	TFTDrawArea(0,TFT_WIDTH,0,TFT_HEIGHT,0x0000);
}

void TFTSetCursosPosition(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2)
{
    TFTSendCommand(0x2A);
    TFTSend16bData(x1);
    TFTSend16bData(x2);
    TFTSendCommand(0x2B);
    TFTSend16bData(y1);
    TFTSend16bData(y2);
}

void TFTDrawArea(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2,uint16_t color)
{
	 uint32_t temp1,temp2,count=0,i;
   TFTSetCursosPosition(x1,x2,y1,y2);
   TFTSendCommand(0x2C);
   temp1=x2-x1;
   temp2=y2-y1;
   if(temp1!=0 && temp2!=0)
   {
       count=temp1*(temp2+2);
   }
   else if(temp1==0)
   {
       count=temp2;
   }
   else if(temp2==0)
   {
       count=temp1;
   }
   for(i=0;i<count;i++)
   {
        TFTSend16bData(color);
   }
}

void TFTDrawPicture(uint8_t* imag,uint16_t sizex,uint16_t sizey)
{
    TFTSetCursosPosition(0,sizex,0,sizey);
    TFTSendCommand(0x2C);
    while(*imag)
    {
        TFTSend16bData(*imag);
        imag++;
    }
}

void TFTDrawPixel(uint16_t x,uint16_t y,uint16_t color)
{
    TFTSetCursosPosition(x,x,y,y);
    TFTSendCommand(0x2C);
    TFTSend16bData(color);
}

void TFTDrawCircle(uint16_t R,uint16_t x,uint16_t y,uint16_t color)
{
		double i=0;
    for(i=0;i<6.28;i+=1/(2*3.14*R))
    {
        TFTDrawPixel(x+R*sin(i),y+R*cos(i),color);
    }
}

void TFTDrawRectangle(uint16_t a,uint16_t x,uint16_t y,uint16_t color)
{
    TFTDrawArea(x,x+a,y,y+a,color);
}

void TFTDrawChar(uint16_t x, uint16_t y, uint16_t color, uint16_t phone, uint8_t ascii, uint8_t size)
{
	uint8_t i,f;
	for (i = 0; i < 8; i++ )
	{
		for(f = 0; f < 8; f++)
		{
			if((simbols[ascii-0x20][i]>>(7-f))&0x01)
			{
				 TFTDrawRectangle(size,x+i*size, y+f*size, color);
			}
			else
			{	
				 TFTDrawRectangle(size,x+i*size, y+f*size, phone);
			}
		}
	}
}

void TFTDrawString(uint16_t x,uint16_t y,uint16_t color, uint16_t phone,uint8_t* string,uint8_t size)
{
    int i=0;
    while(*string)
    {
        TFTDrawChar(x,y+8*i*size,color,phone,*string,size);
        string++;
        i++;
    }
}