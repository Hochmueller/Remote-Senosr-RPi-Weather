/*
 * HP03S.h
 *
 * Created: 06.04.2017 09:47:40
 *  Author: Stephan
 */ 


#ifndef HP03S_H_
#define HP03S_H_
#include <avr/io.h>
#include "timer2.h"

#define HPEEPROM  0xA0      // I2C device address of EEPROM 24C02, see datasheet
#define HPADC	  0xEE		// I2C device address of ADC
#define HPXCLR	PD0

void HP_init(void);
void HP_get_Coef(void);
uint16_t HP_get_raw_temp(void);
uint16_t HP_get_raw_pres(void);
uint16_t simplesquere(uint8_t val);
void HP_get(void);


#endif /* HP03S_H_ */