/*
 * spi.c
 *
 * Created: 06.04.2017 09:06:27
 *  Author: Stephan
 */ 
#include "SPI.h"

void spi_init(void)
{
	 DDRB |= (1<<PB2) | (1<<PB3) | (1<<PB5);
	 SS_UP;
	
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

uint8_t spi_RW(uint8_t cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
	return SPDR;
}

