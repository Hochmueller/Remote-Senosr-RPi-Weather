/*
 * extsensor.c
 *
 * Created: 02.04.2017 00:02:03
 * Author : Stephan
 */ 

/****************************************************************************
Title:    Access serial EEPROM 24C02 using I2C interace
Author:   Peter Fleury <pfleury@gmx.ch>
File:     $Id: test_i2cmaster.c,v 1.3 2015/09/16 09:29:24 peter Exp $
Software: AVR-GCC 4.x
Hardware: any AVR device can be used when using i2cmaster.S or any
          AVR device with hardware TWI interface when using twimaster.c

Description:
    This example shows how the I2C/TWI library i2cmaster.S or twimaster.c 
	can be used to access a serial eeprom.
 
*****************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#define F_CPU 8000000UL
#include <avr/delay.h>
#include "i2cmaster.h"
#include "HP03S.h"
#include "SPI.h"
#include "RFM69.h"
#include "power.h"
#include "HDC1080.h"

extern struct HPResults{
	float temperatur;
	float pressure;
}HPRes;


volatile uint8_t tick = 0;
volatile uint8_t wakeup=0;

int main(void)
{
	uint8_t data[PACKETLENGTH];
	cli();
	MCUSR &= ~(1<<WDRF);
	wdt_reset();
	wdt_disable();
	//wdt_enable(WDTO_1S);
	WDTCSR = (1<<WDCE)|(1<<WDE);
	WDTCSR = (1<<WDP3);
	WDTCSR |= (1<<WDIE);
	sei();
		
	setup_power();
	i2c_init();
	HP_init();
	HDC_init();
	
	spi_init();
	RFM69_init();
	RFM69_mode(sleep);
		
	HP_get_Coef();
	HP_get();
	

	
	while(1)
	{
		if(wakeup)
		{
			power_up();
			_delay_ms(5);
			HP_get();
			HP_get();
			*((float*)data) = HPRes.temperatur;
			*(((float*)data)+1) = HPRes.pressure;
			HDC_getData((uint16_t*)(data+8));
			RFM69_send(data);
			power_down();
			wakeup=0;
		}
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();

		
		//_delay_ms(500);
	}
		
}




ISR(WDT_vect) {
	wdt_reset();
	//WDTCSR |= (1<<WDIE);
	//wdt_disable();
	// do something here
	if(wakeup)
	{
		wdt_enable(WDTO_15MS);
		_delay_ms(100);
	}
	tick++;
	//wakeup=1;
	if(tick>=15)
	{
		wakeup=1;
		tick=0;
	}
	//wdt_enable(WDTO_4S);
	//WDTCSR |= (1<<WDIE); // reenable interrupt to prevent system reset
	
}