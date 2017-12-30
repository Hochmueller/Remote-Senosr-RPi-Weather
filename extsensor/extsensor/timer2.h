/*
 * timer2.h
 *
 * Created: 06.04.2017 09:56:30
 *  Author: Stephan
 */ 


#ifndef TIMER2_H_
#define TIMER2_H_

#include <avr/io.h>

void timer2_init(void);
void timer2_start(void);
void timer2_stop(void);


#endif /* TIMER2_H_ */