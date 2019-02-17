#include "stm32f0xx_hal.h"
#include "gpio-i2c.h"
#include <string.h>

#define SSD1316_ADDR 0x7a
uint8_t framebuf[128*4]={0}; //128*32 dot

void Write_Command(uint8_t cmd){
	uint8_t txbuf[3];
 	txbuf[0] = SSD1316_ADDR;
 	txbuf[1] = 0x00;
 	txbuf[2] = cmd;
	I2C_SendBuf(txbuf, 3, I2C_FLAG_START|I2C_FLAG_STOP);
}

void Write_Data(uint8_t *data, int len){
	uint8_t txbuf[2];
	txbuf[0] = SSD1316_ADDR;
	txbuf[1] = 0x40;
	I2C_SendBuf(txbuf, 2, I2C_FLAG_START);
	I2C_SendBuf(data, len, I2C_FLAG_STOP);
}

void SSD1316_Clear(void){
	memset(framebuf,0x00,sizeof(framebuf));
}

void SSD1316_Refresh(void){
	int  page;
	//memset(framebuf,0x0,sizeof(framebuf));
	for(page=0;page<4;page++)
	{
		Write_Command(0xb0+page);		//page0-page1
		Write_Command(0x00);		//low column start address
		Write_Command(0x10);		//high column start address
		Write_Data(framebuf+128*page, 128);
 }
}

void SSD1316_Page_write(uint8_t page, uint8_t *data){
		Write_Command(0xb0+(page>>2));		//page0-page1
		Write_Command(0x00);		//low column start address
		Write_Command(0x10|((page&0x03)<<1));		//high column start address
		memcpy(framebuf + page*32, data, 32);
		Write_Data(data, 32);
}

void SSD1360_Reset(){
	volatile int i = 100;
	GPIOA->BRR 	= GPIO_PIN_2;
	while(i--);
	GPIOA->BSRR 	= GPIO_PIN_2;
}


void SSD1316_reg_write( uint8_t len, uint8_t *reg){
	if(len == 0)
		SSD1360_Reset();
	else
		for(int i=0; i < len;i++){
			Write_Command(reg[i]);
		}
}

void SSD1316_screen_save(int on){
	static int times = 0;
	if(on)
		times++;
	else{
		times = 0;
		Write_Command(0xAF);
	}
	
	if(times > 30){
		Write_Command(0xAE);
	}
}

void SSD1316_Init(void) 
 { 
	 
	SSD1360_Reset();

  Write_Command(0xAE);      // Set Display Off 
	Write_Command(0xD5);      // Display divide ratio/osc. freq. mode 
	Write_Command(0xC1);      // 115HZ 
	Write_Command(0xA8);      // Multiplex ration mode: 
	Write_Command(0x1F); 
	Write_Command(0xAD);      // External or Internal VCOMH/ IREF Selection 
	Write_Command(0x00);      // Internal VCOMH/ External IREF 
	Write_Command(0x20);  		// Set Memory Addressing Mode 
	Write_Command(0x02);      // Page Addressing Mode 
	Write_Command(0xD3);      // Set Display Offset       
	Write_Command(0x00); 
	Write_Command(0x40);      // Set Display Start Line 
	Write_Command(0x8D);      // DC-DC Control Mode Set 
	Write_Command(0x14);      // DC-DC ON Mode Set   
	Write_Command(0xA1);      // 0xA0 Left Right Segment Remap     
	Write_Command(0xC0);      // 0xC8 Up Down Set COM Output Scan Direction 
	Write_Command(0xDA);      // Seg pads hardware: alternative 
	Write_Command(0x12); 
	Write_Command(0x81);      // Contrast control 
	Write_Command(0x53); 
	Write_Command(0xD9);      // Set pre-charge period    
	Write_Command(0x22); 
	Write_Command(0xDB);      // VCOMH deselect level mode 
	Write_Command(0x00); 
	Write_Command(0xA4);      // Set Entire Display On/Off 
	Write_Command(0xA6);      // Set Normal Display  

	SSD1316_Clear(); 
	SSD1316_Refresh();
	Write_Command(0xAF);      // Set Display On 
 } 

 // the most basic function, set a single pixel
void SSD1316_drawPixel(int16_t x, int16_t y, uint16_t color) {

    int index = x+ (y/8)*128;
    //printf("index = %d\n", index);

		if(color)
      framebuf[index] |=  (1 << (y&7));
		else
      framebuf[index] &= ~(1 << (y&7));
}
