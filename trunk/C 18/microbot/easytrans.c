#include <htc.h>

#define TIMEOUT			40
#define RMOT			GP5
#define LMOT			GP4

// R = 395 us = 13 -> 15.31 pulses
// L = 711 us = 23 -> 27.09 pulses
// RL = 1026 us = 33 -> 38.87 pulses
#define RMOV		15
#define LMOV		27
#define RLMOV		39
#define PAUSE		100

#define TMR0LOW		3
#define TMR0HIGH	7

#define _XTAL_FREQ 4000000

__CONFIG(MCLRDIS & WDTDIS & UNPROTECT & INTRC);

void main()
{
	unsigned char flag = 0;
	unsigned char time;
	unsigned char i;

	TRIS = 0b111101;

	// Set-up TMR0 for 38.0 khz = 26us period
	// High for 12us, low for 14us -> 6/7 ticks
	// prescale = 1:2
	// increment = Prescale*4/fosc = 2*4/4MHz = 2us
	OPTION = 0b00010000;

	while(GP3 == 0 || GP0 == 0)
	{
		TMR0 = 0;

		//determine what type of instruction to send
		// 1 means not pushed, 0 means button pushed
		if(GP3 == 0 && GP0 == 1) // instruction
		{
			//new insturction
			TMR0 = 0;
			for(i=0; i< RMOV; i++)
			{
				TMR0 = 0;
				GP1 = 1;
				while(TMR0 < TMR0LOW);
				GP1 = 0;
				while(TMR0 < TMR0HIGH);
			}
			for(i=0; i< PAUSE; i++)
			{
				TMR0 = 0;
				GP1 = 0;
				while(TMR0 < 21);
			}
		}
		else if(GP3 == 1 && GP0 == 0) // instruction
		{
			//new insturction
			TMR0 = 0;
			for(i=0; i< LMOV; i++)
			{
				TMR0 = 0;
				GP1 = 1;
				while(TMR0 < TMR0LOW);
				GP1 = 0;
				while(TMR0 < TMR0HIGH);
			}
			for(i=0; i< PAUSE; i++)
			{
				TMR0 = 0;
				GP1 = 0;
				while(TMR0 < 21);
			}
		}
		else if(GP3 == 0 && GP0 == 0) // instruction
		{
			//new insturction
			TMR0 = 0;
			for(i=0; i< RLMOV; i++)
			{
				TMR0 = 0;
				GP1 = 1;
				while(TMR0 < TMR0LOW);
				GP1 = 0;
				while(TMR0 < TMR0HIGH);
			}
			for(i=0; i< PAUSE; i++)
			{
				TMR0 = 0;
				GP1 = 0;
				while(TMR0 < 21);
			}
		}
/*		else if(GP3 == 1 && GP5 == 1) // instruction
		{
			//no command, go sleep
			SLEEP();
		}*/
	}
	TMR0 = 0;
	GP1 = 0;
	while(TMR0 < 21);
	flag = GP3<<1||GP0;
	SLEEP();
}

	