#ifndef _ILI_H
#define _ILI_H

#include "stm32f411xe.h"
#include "delay.h"
#include "TFTFont.h"
#include <math.h>


#define TFT_DESELECT GPIOB->BSRR|=GPIO_BSRR_BS6
#define TFT_SELECT GPIOB->BSRR|=GPIO_BSRR_BR6
#define TFT_RESET_SET GPIOB->BSRR|=GPIO_BSRR_BS8
#define TFT_RESET_RES GPIOB->BSRR|=GPIO_BSRR_BR8
#define TFT_DATA GPIOB->BSRR|=GPIO_BSRR_BS9
#define TFT_COMMAND GPIOB->BSRR|=GPIO_BSRR_BR9

#define TFT_WIDTH 240
#define TFT_HEIGHT 320

extern const uint8_t simbols[96][8];

	
void TFTGpioInit(void);
void TFTspi1Init(void);
uint8_t TFTspiTransmit(uint8_t data);
void TFTInit(void);
void TFTSendCommand(uint8_t command);
void TFTSendData(uint8_t data);
void TFTSend16bData(uint16_t data);
void TFTSetCursosPosition(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
void TFTDrawArea(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2,uint16_t color);
void TFTDrawPicture(uint8_t* imag,uint16_t sizex,uint16_t sizey);
void TFTDrawPixel(uint16_t x,uint16_t y,uint16_t color);
void TFTDrawCircle(uint16_t R,uint16_t x,uint16_t y,uint16_t color);
void TFTDrawRectangle(uint16_t a,uint16_t x,uint16_t y,uint16_t color);
void TFTDrawChar(uint16_t x, uint16_t y, uint16_t color, uint16_t phone, uint8_t ascii, uint8_t size);
void TFTDrawString(uint16_t x,uint16_t y,uint16_t color, uint16_t phone,uint8_t* string,uint8_t size);

#endif