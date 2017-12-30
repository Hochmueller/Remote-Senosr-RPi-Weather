/*
 * HDC1080.h
 *
 * Created: 24.07.2017 14:11:37
 *  Author: Stephan
 */ 


#ifndef HDC1080_H_
#define HDC1080_H_

#include <avr/io.h>
#include <avr/delay.h>
#include "i2cmaster.h"

#define HDC1080_ADDR 0x80
#define CONFIG_REG 0x02 
#define RH_REG 0x01
#define TEMP_REG 0x00


void HDC_getData (uint16_t* data);
void HDC_init (void);

#endif /* HDC1080_H_ */