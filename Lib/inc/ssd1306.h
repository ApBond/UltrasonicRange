#ifndef __SSD1306
#define __SSD1306

#include "stm32f411xe.h"
#include "fonts.h"
#include "stdlib.h"

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define BUFFER_SIZE 1024

#define CS_SET GPIOB->BSRR|=GPIO_BSRR_BS6
#define CS_RES GPIOB->BSRR|=GPIO_BSRR_BR6
#define RESET_SET GPIOB->BSRR|=GPIO_BSRR_BS8
#define RESET_RES GPIOB->BSRR|=GPIO_BSRR_BR8
#define DATA GPIOB->BSRR|=GPIO_BSRR_BS9
#define COMMAND GPIOB->BSRR|=GPIO_BSRR_BR9

#define HEAD_ON 1
#define HEAD_OFF 0

#define SPI2_DR_8bit         *(__IO uint8_t*)&(SPI2->DR)

typedef struct SSD1306_List
{
	struct SSD1306_ListElem *first;
	struct SSD1306_ListElem *currentElement;
	uint8_t size;
	char *top;
}SSD1306_List;

typedef struct SSD1306_ListElem
{
	struct SSD1306_ListElem *prev;
	struct SSD1306_ListElem *next;
	char *name;
	uint8_t id;
}SSD1306_ListElem;


void ssd1306DisplayUPD(void);
void ssd1306RunDisplayUPD(void);
void ssd1306StopDispayUPD(void);
void ssd1306SendCommand(uint8_t command);
void ssd1306SendData(uint8_t data);
void ssd1306Init(void);
void ssd1306DrawPixel(uint16_t x, uint16_t y,uint8_t color);
uint8_t getPixelStatus(uint16_t x, uint16_t y);
uint8_t ssd1306SetCursos(uint16_t x,uint16_t y);
char ssd1306Puts(char ch, FontDef_t* Font, uint8_t color);
void ssd1306Printf(char* str,FontDef_t* Font,uint8_t color);
void ssd1306PrintDigit(uint32_t dig,FontDef_t* Font,uint8_t color);
void ssd1306DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t c);
void ssd1306DrawRect(uint16_t x,uint16_t y,uint8_t width,uint8_t height,uint8_t color);
void ssd1306DrawCircle(uint16_t x0,uint16_t y0,uint8_t r,uint8_t color);
void ssd1306DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint8_t c);
void ssd1306ClearDisplay(uint8_t color);
void ssd1306ListAddElement(SSD1306_List *list,char* name,uint8_t id);
void ssd1306ListSetPosition(SSD1306_List *list,uint8_t pos);
void ssd1306PrintList(SSD1306_List *list,FontDef_t* Font, uint8_t head);
#endif