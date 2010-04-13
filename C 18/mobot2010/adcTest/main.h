#ifndef MAIN_H
#define MAIN_H

#include <p18f25k20.h>
#include <stdio.h>
#include <timers.h>
#include <adc.h>

#define OSC_4MHZ	0x05
#define OSC_8MHZ 	0x06
#define OSC_16MHZ 	0x07

void low_isr(void);
void high_isr(void);
void initTMR0(void);
void initPORTS(void);
void initADC(void);

#endif