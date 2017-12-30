/*
 * _power.c
 *
 * Created: 03.05.2017 22:48:38
 *  Author: Stephan
 */ 
#include <avr/io.h>
#include "power.h"
#include "HP03S.h"
#include "i2cmaster.h"
#include "SPI.h"
#include "RFM69.h"

void setup_power(void)
{
	/* power reduction register */
	PRR |= (1<<PRTIM0)|(1<<PRTIM1)|(1<<PRUSART0)|(1<<PRADC);
	/* Dissable analog comperator */
	ACSR |= (1<<ACD);
}

void power_down(void)
{
	PORTD &= ~(1<<HPXCLR);
	RFM69_mode(sleep);
	PRR |= (1<<PRTWI)|(1<<PRTIM2)|(1<<PRSPI);
}

void power_up(void)
{
	PRR &= ~((1<<PRTWI)|(1<<PRTIM2)|(1<<PRSPI));
	i2c_init();
	spi_init();
}