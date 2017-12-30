/*
 * RFM64.h
 *
 * Created: 06.04.2017 17:12:23
 *  Author: Stephan
 */ 


#ifndef RFM64_H_
#define RFM64_H_


#define F_CPU 8000000UL
#include <util/delay.h>

#include <avr/io.h>
#include "SPI.h"

#define RFM_WRITE 0x80
#define RFM_READ 0

#define PORTReset PORTB
#define DDRReset DDRB
#define PReset PB1

#define PORTIO5 PORTB
#define DDRIO5 DDRB
#define PINIO5 PINB
#define PIO5 PB0

#define PORTIO4 PORTD
#define DDRIO4 DDRD
#define PINIO4 PIND
#define PIO4 PD6

#define PORTIO3 PORTD
#define DDRIO3 DDRD
#define PINIO3 PIND
#define PIO3 PD7

#define PORTIO2 PORTB
#define DDRIO2 DDRB
#define PINIO2 PINB
#define PIO2 PB6

#define PORTIO1 PORTB
#define DDRIO1 DDRB
#define PINIO1 PINB
#define PIO1 PB1

#define PORTIO0 PORTD
#define DDRIO0 DDRD
#define PINIO0 PIND
#define PIO0 PD5

/*RFM69 Registers*/
#define FIFO 0x00
#define OPMODE 0x01
#define DATAMODUL 0x02
#define BITRATEMSB 0x03
#define BITRATELSB 0x04
#define FDEVMSB 0x05
#define FDEVLSB 0x06
#define FRFMSB 0x07
#define FRFMID 0x08
#define FRFLSB 0x09
#define OSC1 0x0A
#define AFCCTRL 0x0B
#define LISTEN1 0x0D
#define LISTEN2 0x0E
#define LISTEN3 0x0F
#define VERSION 0x10
#define PALEVEL 0x11
#define PARAMP 0x12
#define OCP 0x13
#define LNA 0x18
#define RXBW 0x19
#define AFCBW 0x1A
#define OOKPEAK 0x1B
#define OOKAVG 0x1C
#define OOKFIX 0x1D
#define AFCFEI 0x1E
#define AFCMSB 0x1F
#define AFCLSB 0x20
#define FEIMSB 0x21
#define FEILSB 0x22
#define RSSICONFIG 0x23
#define RSSIVALUE 0x24
#define DIOMAPPING1 0x25
#define DIOMAPPING2 0x26
#define IRQFLAGS1 0x27
#define IRQFLAGS2 0x28
#define RSSITHRESH 0x29
#define RXTIMEOUT1 0x2A
#define RXTIMEOUT2 0x2B
#define PREAMBLEMSB 0x2C
#define PREAMBLELSB 0x2D
#define SYNCCONFIG 0x2E
#define SYNCVALUE1 0x2F
#define SYNCVALUE2 0x30
#define SYNCVALUE3 0x31
#define SYNCVALUE4 0x32
#define SYNCVALUE5 0x33
#define SYNCVALUE6 0x34
#define SYNCVALUE7 0x35
#define SYNCVALUE8 0x36
#define PACKETCONFIG1 0x37
#define PAYLOADLENGTH 0x38
#define NODEADRS 0x39
#define BROADCASTADRS 0x3A
#define AUTOMODES 0x3B
#define FIFOTHRESH 0x3C
#define PACKETCONFIG2 0x3D
#define AESKEY1 0x3E
#define AESKEY2 0x3F
#define AESKEY3 0x40
#define AESKEY4 0x41
#define AESKEY5 0x42
#define AESKEY6 0x43
#define AESKEY7 0x44
#define AESKEY8 0x45
#define AESKEY9 0x46
#define AESKEY10 0x47
#define AESKEY11 0x48
#define AESKEY12 0x49
#define AESKEY13 0x4A
#define AESKEY14 0x4B
#define AESKEY15 0x4C
#define AESKEY16 0x4D
#define TEMP1 0x4E
#define TEMP2 0x4F
#define TESTLNA 0x58
#define TESTPA1 0x5A
#define TESTPA2 0x5C
#define TESTDAGC 0x6F
#define TESTAFC 0x71


/* User Settings */
#define FXO 32000000UL
#define FSTEP (FXO/524288UL)
#define BR 4800UL
#define FDEV (2*BR)  //MFSK n=2=fdev/fbit
#define FRF 868000000UL
//Power settings +2 - +17 dBm
#define POUT 13
#define SYNCID 0x55UL
#define PACKETLENGTH 0x0c

#define PREAMBLESIZE 0xF

/* Register Calculations */
//Bitrate
#define REG_BRMSB ((FXO/BR)>>8)&0xFF
#define REG_BRLSB ((FXO/BR)&0xFF)
//Fdev
#define REG_FDEVMSB ((FDEV/FSTEP)>>8)&0xFF
#define REG_FDEVLSB (FDEV/FSTEP)&0xFF
//Frf
#define REG_FRFMSB ((FRF/FSTEP)>>16)&0xFF
#define REG_FRFMID ((FRF/FSTEP)>>8)&0xFF
#define REG_FRFLSB (FRF/FSTEP)&0xFF

#define REG_PALEVEL (0b11<<5) | ((POUT+14)&0x1F)



typedef enum{
	sleep = 0,
	stdby = 1,
	fs = 2,
	tx = 3,
	rx = 4
	}modes;

void RFM69_reset(void);
void RFM69_init(void);
void RFM69_mode(modes mod);
uint16_t RFM69_getFlag(void);
modes RFM69_getMode(void);
uint8_t RMF69_getVersion(void);
void RFM69_send(uint8_t* data);

#endif /* RFM64_H_ */