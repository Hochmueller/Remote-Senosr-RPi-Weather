/*
 * HP03S.c
 *
 * Created: 06.04.2017 09:47:27
 *  Author: Stephan
 */ 
#include "HP03S.h"
#include "i2cmaster.h"
#define F_CPU 8000000UL
#include <util/delay.h>

struct HPcoefficient {
	uint16_t C1;
	uint16_t C2;
	uint16_t C3;
	uint16_t C4;
	uint16_t C5;
	uint16_t C6;
	uint16_t C7;
	uint8_t A;
	uint8_t B;
	uint8_t C;
	uint8_t D;
} HPCoef;

struct HPResults{
	float temperatur;
	float pressure;
}HPRes;



void HP_init(void)
{
	    DDRD |= (1<<HPXCLR);
	    PORTD &= ~(1<<HPXCLR);
		timer2_init();
		timer2_start();
}

void HP_get_Coef(void)
{
	PORTD &= ~(1<<HPXCLR);
	i2c_start_wait(HPEEPROM+I2C_WRITE);
	i2c_write(0x10);
	i2c_rep_start(HPEEPROM+I2C_READ);
	HPCoef.C1 = (i2c_readAck()<<8);
	HPCoef.C1 |= (i2c_readAck());
	HPCoef.C2 = (i2c_readAck()<<8);
	HPCoef.C2 |= (i2c_readAck());
	HPCoef.C3 = (i2c_readAck()<<8);
	HPCoef.C3 |= (i2c_readAck());
	HPCoef.C4 = (i2c_readAck()<<8);
	HPCoef.C4 |= (i2c_readAck());
	HPCoef.C5 = (i2c_readAck()<<8);
	HPCoef.C5 |= (i2c_readAck());
	HPCoef.C6 = (i2c_readAck()<<8);
	HPCoef.C6 |= (i2c_readAck());
	HPCoef.C7 = (i2c_readAck()<<8);
	HPCoef.C7 |= (i2c_readAck());
	HPCoef.A = i2c_readAck();
	HPCoef.B = i2c_readAck();
	HPCoef.C = i2c_readAck();
	HPCoef.D = i2c_readAck();
	i2c_stop();
}

uint16_t HP_get_raw_temp(void)
{
	uint16_t adcval;
	PORTD |= (1<<HPXCLR);
	i2c_start_wait(HPADC+I2C_WRITE);
	i2c_write(0xFF);
	i2c_write(0xE8);
	i2c_stop();
	_delay_ms(100);
	i2c_start_wait(HPADC+I2C_WRITE);
	i2c_write(0xFD);
	i2c_rep_start(HPADC+I2C_READ);
	adcval = (i2c_readAck()<<8);
	adcval |= i2c_readNak();
	i2c_stop();
	
	PORTD &= ~(1<<HPXCLR);
	return adcval;
}

uint16_t HP_get_raw_pres(void)
{
	uint16_t adcval;
	PORTD |= (1<<HPXCLR);
	i2c_start_wait(HPADC+I2C_WRITE);
	i2c_write(0xFF);
	i2c_write(0xF0);
	i2c_stop();
	_delay_ms(80);
	i2c_start_wait(HPADC+I2C_WRITE);
	i2c_write(0xFD);
	i2c_rep_start(HPADC+I2C_READ);
	adcval = (i2c_readAck()<<8);
	adcval |= i2c_readNak();
	i2c_stop();
	
	PORTD &= ~(1<<HPXCLR);
	return adcval;
}

uint16_t simplesquere(uint8_t val)
{
	uint16_t temp=1;
	for(int i = 0; i<val; i++)
	temp*=2;
	return temp;
}

void HP_get(void)
{
	
	uint16_t rawTemp = HP_get_raw_temp();
	uint16_t rawPres = HP_get_raw_pres();
	
	float dut;
	dut = (float)(rawTemp)-(float)(HPCoef.C5);
	if(rawTemp>=HPCoef.C5)
	dut = dut - dut*dut/16384.0*(float)(HPCoef.A)/(float)(simplesquere(HPCoef.C));
	else
	dut = dut - dut*dut/16384.0*(float)(HPCoef.B)/(float)(simplesquere(HPCoef.C));
	
	HPRes.temperatur = (250 + dut*((float)HPCoef.C6)/65536 - dut/((float)simplesquere(HPCoef.D)))/10;
	
	HPRes.pressure = (float)(HPCoef.C1)+(float)(HPCoef.C3)*dut/1024.0;
	HPRes.pressure = HPRes.pressure*((float)(rawPres)-7168.0)/16384.0;
	HPRes.pressure = HPRes.pressure - ((float)(HPCoef.C2)+((float)(HPCoef.C4)-1024.0)*dut/16384.0)*4.0;
	HPRes.pressure=(HPRes.pressure*10.0/32.0+(float)(HPCoef.C7))/10.0;

}