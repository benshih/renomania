#ifndef MAIN_H
#define MAIN_H

#include <p18f25k20.h>
#include <stdio.h>
#include <timers.h>
#include <adc.h>
#include <delays.h>
#include <stdio.h>
#include <stdlib.h>

#define OSC_4MHZ	0x05
#define OSC_8MHZ 	0x06
#define OSC_16MHZ 	0x07

void low_isr(void);
void high_isr(void);
void SendUART(char*);
void CalcThreshold(void);
void CalcServosPos(void);
void CollectADC(void);
void StoreBlack(void);
void StoreWhite(void);
void LineTrace(void);

#endif