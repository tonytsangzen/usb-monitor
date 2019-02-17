/*
*/
#include "stm32f0xx_hal.h"
#include "stdlib.h"
#include "gpio-i2c.h"

#define TRUE			1
#define FALSE			0
#define I2C_GPIO	GPIOA
#define I2C_SCL		GPIO_PIN_1
#define I2C_SDA		GPIO_PIN_0
#define I2C_RST		GPIO_PIN_2

#define RST_H			(I2C_GPIO->BSRR 	= I2C_RST)
#define RST_L			(I2C_GPIO->BRR 		= I2C_RST)
#define SCL_H			(I2C_GPIO->BSRR 	= I2C_SCL)
#define SCL_L			(I2C_GPIO->BRR 		= I2C_SCL)
#define	SDA_H			(I2C_GPIO->BSRR		= I2C_SDA)
#define SDA_L			(I2C_GPIO->BRR		= I2C_SDA)
#define	SCL_READ 	(I2C_GPIO-IDR 		& I2C_SCL)
#define SDA_READ 	(I2C_GPIO->IDR 		& I2C_SDA)
 
#define I2C_PageSize 8
 
void I2C_GPIO_Config(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;
	/*Configure I2C1 Pins:SCL and SDA*/
	GPIO_InitStructure.Pin = I2C_SCL |I2C_SDA;
	GPIO_InitStructure.Speed =GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	HAL_GPIO_Init(I2C_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = I2C_RST;
	GPIO_InitStructure.Speed =GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(I2C_GPIO,&GPIO_InitStructure);
}
 
void I2C_delay(void)
{
	__ASM("nop");
	__ASM("nop");
	__ASM("nop");
	__ASM("nop");
	__ASM("nop");
	__ASM("nop");
}

int  I2C_START(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_READ) return FALSE;  //SDA线为低电平总线忙，退出
	
	SDA_L;
	I2C_delay();
	if(SDA_READ) return FALSE; //SDA线为高电平则总线出错，退出
	
	SDA_L;
	I2C_delay();
	return TRUE;
}
 
void I2C_STOP(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();	
}
 
void I2C_Ack(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();	
}
 
void I2C_NoAck(void)
{
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}
 
int  I2C_WaitAck(void)  //返回为：-1有ACK， =0 无ACK
{
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_READ)
	{
		SCL_L;
		return FALSE;
	}
	SCL_L;
	return TRUE;
	
}
 
void I2C_SendByte(uint8_t SendByte) //数据从高位到低位
{
	uint8_t i=8;
	while(i--)
	{
		SCL_L;
		I2C_delay();
		if(SendByte & 0x80)
			SDA_H;
		else
			SDA_L;
		SendByte <<= 1;
		I2C_delay();
		SCL_H;
		I2C_delay();
		
	}
	SCL_L;	
}

uint8_t I2C_ReceiveByte(void) //数据从高位到低位
{
	uint8_t i=8;
	uint8_t ReceiveByte = 0;
	
	SDA_H;
	while(i--)
	{
		ReceiveByte <<= 1;
		SCL_L;
		I2C_delay();
		SCL_H;
		I2C_delay();
		if(SDA_READ)
		{
			ReceiveByte|=0x01;
		}		
	}
	SCL_L;
	return ReceiveByte;		
}

int  I2C_SendBuf(uint8_t* data, int len, int flag)
{
 	if(flag & I2C_FLAG_START)
		if(!I2C_START()) return FALSE;
	
	for(int i=0; i < len; i++){
		I2C_SendByte(data[i]); //数据
		if(!I2C_WaitAck())
		{		
			I2C_STOP();
			return FALSE;
		}
	}
	
	if(flag & I2C_FLAG_STOP)
		I2C_STOP();
	return TRUE;
}
