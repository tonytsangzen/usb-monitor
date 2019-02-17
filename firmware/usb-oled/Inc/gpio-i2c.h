
#define I2C_FLAG_START 0x1
#define I2C_FLAG_STOP 0x2

void I2C_GPIO_Config(void);
int  I2C_SendBuf(uint8_t* data, int len, int flag);
