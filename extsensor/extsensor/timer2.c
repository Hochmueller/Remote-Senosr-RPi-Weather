/*
 * timer2.c
 *
 * Created: 06.04.2017 09:56:48
 *  Author: Stephan
 */ 
#include "timer2.h"

//wrong ocr2a???
void timer2_init(void)
{
	TCCR2A = (1<<COM2B0)|(1<<WGM21);
	OCR2A = 122;
	DDRD |= (1<<PD3);
}

void timer2_start(void)
{
	TCCR2B = (1<<CS20);
}

void timer2_stop(void)
{
	TCCR2B = 0;
}
