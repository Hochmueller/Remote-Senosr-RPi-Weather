/*
 * RFM69.c
 *
 * Created: 06.04.2017 17:12:09
 *  Author: Stephan
 */ 
#include "RFM69.h"

const uint16_t RFM69ConfigTbl[20] = {
 0x0200,        //RegDataModul : Packet Mode, FSK, no Shaping 
 0x0502,        //RegFdevMsb      241*61Hz = 35KHz   
 0x0641,        //RegFdevLsb
 0x0334,        //RegBitrateMsb      32MHz/0x3410 = 2.4kpbs
 0x0410,        //RegBitrateLsb
 0x131A,        //RegOcp         Disable OCP
 0x1808,        //RegLNA            50   
 0x1952,        //RegRxBw         RxBW  83KHz
 0x2C00,        //RegPreambleMsb   
 0x2D0A,        //RegPreambleLsb   10Byte Preamble
 0x2E90,        //enable Sync.Word   2+1=3bytes
 0x2FAA,        //0xAA            SyncWord = aa2dd4
 0x302D,        //0x2D
 0x31D4,        //0xD4
 0x3738,        //RegPacketConfig1  CRC manchester encode
 0x380C,        //RegPayloadLength  12bytes for length & Fixed length
 0x3C95,        //RegFiFoThresh      
 //0x581B,        //RegTestLna        Normal sensitivity
 0x582D,        //RegTestLna        increase sensitivity with LNA (Note: consumption also increase!)
 0x6F30,        //RegTestDAGC       Improved DAGC
 //0x6F00,        //RegTestDAGC       Normal DAGC
 0x0104,        // RegOpMode = Standby  
  
};


void RFM69_reset(void)
{
	PORTReset |= (1<<PReset);
	_delay_ms(1);
	PORTReset &= ~(1<<PReset);
}

void RFM69_init(void)
{
	DDRReset |= (1<<PReset);	
	DDRIO0 &= ~(1<<PIO0);
	DDRIO1 &= ~(1<<PIO1);
	DDRIO2 &= ~(1<<PIO2);
	DDRIO3 &= ~(1<<PIO3);
	DDRIO4 &= ~(1<<PIO4);
	DDRIO5 &= ~(1<<PIO5);
	
	
	RFM69_reset();
	/*modulation shaping*/
#if 0
	SS_DOWN;
	spi_RW(RFM_WRITE|DATAMODUL);
	spi_RW(0b00000001);
	SS_UP;
	/* Clocks */
	SS_DOWN;
	spi_RW(RFM_WRITE|BITRATEMSB);
	//Bit Rate reg MSB
	spi_RW(REG_BRMSB);
	//Bit Rate reg LSB
	spi_RW(REG_BRLSB);
	//FDEV MSB
	spi_RW(REG_FDEVMSB);
	//FDEV LSB
	spi_RW(REG_FDEVLSB);
	//FRF MSB
	spi_RW(REG_FRFMSB);
	//FRF Middle
	spi_RW(REG_FRFMID);
	//FRF LSB
	spi_RW(REG_FRFLSB);
	SS_UP;
	//power settings
	SS_DOWN;
	spi_RW(RFM_WRITE|PALEVEL);
	spi_RW(REG_PALEVEL);
	SS_UP;
	//DIO Mapping
	SS_DOWN;
	spi_RW(RFM_WRITE|DIOMAPPING1);
	/*
	DIO0 Packet send
	DIO1 FIFO Level
	DIO2 -----------
	DIO3 FIFO full
	DIO4 PLL Lock
	DIO5 MODE Ready
	*/
	spi_RW(0b00000011);
	spi_RW(0b11110111);
	SS_UP;
	//preamble size
	SS_DOWN;
	spi_RW(RFM_WRITE|PREAMBLELSB);
	spi_RW(PREAMBLESIZE);
	SS_UP;
	//sync
	SS_DOWN;
	spi_RW(RFM_WRITE|SYNCCONFIG);
	//sync on; FIFOFILLCONDITION=if sync address interrupt; syncSize=8bypte; syntTolleranze=0;
	spi_RW(0b10111000);
	//all bytes of same ID
	for(int i=0;i<8;i++)
		spi_RW(SYNCID);
	SS_UP;
	//Packetconfig
	SS_DOWN;
	spi_RW(RFM_WRITE|PACKETCONFIG1);
	//FIX lenth; Manchester; No CRC; No Address
	spi_RW(0b00100000);
	//pyloadLength
	spi_RW(PACKETLENGTH);
	SS_UP;
	//TX Config(FIFO Thresh)
	SS_DOWN;
	spi_RW(RFM_WRITE|FIFOTHRESH);
	//TxStartCondition=FifoLevel; FifoThreshold=Packetlength
	spi_RW((PACKETLENGTH-1)&0x7F);
	SS_UP;
#endif

#if 1
	for(int i=0; i<20; i++)
	{
		SS_DOWN;
		spi_RW(RFM_WRITE|(RFM69ConfigTbl[i]>>8));
		spi_RW(RFM69ConfigTbl[i]&0xFF);
		SS_UP;
	}
	SS_DOWN;
	spi_RW(RFM_WRITE|DIOMAPPING1);
	/*
	DIO0 Packet send
	DIO1 FIFO Level
	DIO2 -----------
	DIO3 FIFO full
	DIO4 PLL Lock
	DIO5 MODE Ready
	*/
	spi_RW(0b00000011);
	spi_RW(0b11110111);
	SS_UP;
	//power settings
	SS_DOWN;
	spi_RW(RFM_WRITE|PALEVEL);
	spi_RW(REG_PALEVEL);
	SS_UP;
	//fifo threshold
	SS_DOWN;
	spi_RW(RFM_WRITE|FIFOTHRESH);
	//TxStartCondition=FifoLevel; FifoThreshold=Packetlength
	spi_RW((PACKETLENGTH-1)&0x7F);
	SS_UP;
	
	SS_DOWN;
	spi_RW(RFM_WRITE|FRFMSB);
	//FRF MSB
	spi_RW(REG_FRFMSB);
	//FRF Middle
	spi_RW(REG_FRFMID);
	//FRF LSB
	spi_RW(REG_FRFLSB);
	SS_UP;

	
	

#endif


}

modes RFM69_getMode(void)
{
	uint8_t temp;
	SS_DOWN;
	spi_RW(RFM_READ|OPMODE);
	temp = spi_RW(0);
	SS_UP;
	temp = (temp>>2)&0x7;
	return temp;
	
}
void RFM69_mode(modes mod)
{
	uint8_t temp;
	SS_DOWN;
	spi_RW(RFM_READ|OPMODE);
	temp = spi_RW(0);
	SS_UP;
	temp &= 0b11100011;
	temp |= (mod<<2);
	SS_DOWN;
	spi_RW(RFM_WRITE|OPMODE);
	spi_RW(temp);
	SS_UP;
	while(!(PINIO5&(1<<PIO5)));
}



uint16_t RFM69_getFlag(void)
{
	volatile uint16_t ret;
	SS_DOWN;
	spi_RW(RFM_READ|IRQFLAGS1);
	ret = spi_RW(0);
	//ret <<=8;
	ret |= (spi_RW(0)<<8);
	SS_UP;
	return ret;
}

uint8_t RMF69_getVersion(void)
{
	uint16_t ret;
	SS_DOWN;
	spi_RW(RFM_READ|VERSION);
	ret = spi_RW(0);
	SS_UP;
	return ret;
}

void RFM69_send(uint8_t* data)
{
	modes current = RFM69_getMode();
	volatile uint16_t flag=0;
	
	flag = RFM69_getFlag(); 
	
	SS_DOWN;
	//fill fifo to level
	spi_RW(RFM_WRITE|FIFO);
	for(int i=0; i<PACKETLENGTH; i++)
		spi_RW(data[i]);
	SS_UP;
	flag = RFM69_getFlag(); 
	RFM69_mode(tx);
	flag = RFM69_getFlag(); 
	while(!(PINIO0&(1<<PIO0)));
	//set back to last mode
	RFM69_mode(current);
}
/**
notes:
	Lock detect indicator:
		but to one pin to deteckt if module is able to send (tabel21 -22)
		
	ModeReady & TxReady interrupts
		
	When AFC is enabled and performed automatically at the receiver startup, the channel filter used by the receiver
	during the AFC and the AGC is RxBwAfc instead of the standard RxBw setting.
	
	enable DAGC
	
	modulation factor between 0.5 & 10
	
	maybe use manchaster code to enable bit sync (page 32)
	
	in RX:
		 AfcAutoOn = 1
		 */