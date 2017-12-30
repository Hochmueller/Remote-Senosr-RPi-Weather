/*
 * SPI.h
 *
 * Created: 06.04.2017 09:43:09
 *  Author: Stephan
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>


#define SS_DOWN PORTB&= ~(1<<PB2)
#define SS_UP PORTB |= (1<<PB2)

void spi_init(void);
uint8_t spi_RW(uint8_t cData);


#endif /* SPI_H_ */