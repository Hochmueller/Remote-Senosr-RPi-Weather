/*
 * HDC1080.c
 *
 * Created: 24.07.2017 14:11:21
 *  Author: Stephan
 */ 
#include "HDC1080.h"

/************************************************************************/
/* get data into a array with length 2                                  */
/************************************************************************/
void HDC_init (void)
{
	i2c_start(HDC1080_ADDR|I2C_WRITE);
	i2c_write(CONFIG_REG);
	i2c_write(0x10);
	i2c_stop();
}

void HDC_getData (uint16_t* data)
{
	uint16_t temp;
	uint16_t rh;
	i2c_start(HDC1080_ADDR|I2C_WRITE);
	i2c_write(TEMP_REG);
	i2c_stop();
	_delay_ms(200);
	i2c_start(HDC1080_ADDR|I2C_READ);
	temp=i2c_readAck()<<8;
	temp |= i2c_readAck();
	rh = i2c_readAck()<<8;
	rh |= i2c_readAck();
	data[0]=temp;
	data[1]=rh;
}