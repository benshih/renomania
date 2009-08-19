#include <htc.h>

#define TIMEOUT			220
#define RMOT			GP5
#define LMOT			GP4
#define IRIN			GP2

// TMR0 set to tick every 
// R = 520 us -> 16
// L = 840 us -> 26
// RL = 1160 us -> 36
// PAUSE = 2632 us -> 82
#define RMOVElower		11
#define RMOVEhigher		21
#define LMOVElower		22
#define LMOVEhigher		31
#define RLMOVElower		32
#define RLMOVEhigher	41

#define PAUSE	70

#define _XTAL_FREQ 4000000

__CONFIG(MCLRDIS & WDTDIS & UNPROTECT & INTRC);

void main()
{
	unsigned char flag = 0;
	unsigned char i;
	unsigned char time;
	unsigned char fail_counter=0;

	TRIS = 0b000101;

	// Set-up TMR0
	// prescale = 1:32
	// increment = Prescale*4/fosc = 32*4/4MHz = 32us
	OPTION = 0b11010100;

	while(IRIN == 0);

	while(1)
	{

		TMR0 = 0;
		while(IRIN == 1) //wait until next instruction
		{
			if(TMR0 > TIMEOUT)
			{
				flag = 1;
				break;
			}
		}

		TMR0 = 0;
		while(IRIN == 0)
		{
			//new insturction
			if(TMR0 > TIMEOUT)
			{
				flag = 1;
				break;
			}
		}

		time = TMR0;
		TMR0 = 0;

		//determine what to do with instruction
		if(flag == 1)
		{
			flag = 0;
			RMOT = 0;
			LMOT = 0;
		}
		else if(time > RMOVElower && time < RMOVEhigher)
		{
			RMOT = 1;
			LMOT = 0;
		}
		else if(time > LMOVElower && time < LMOVEhigher)
		{
			RMOT = 0;
			LMOT = 1;
		}
		else if(time > RLMOVElower && time < RLMOVEhigher)
		{
			RMOT = 1;
			LMOT = 1;
		}
		else
		{
			RMOT = 0;
			LMOT = 0;
		}
	}
}

	