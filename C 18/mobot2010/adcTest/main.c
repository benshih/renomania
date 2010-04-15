#include "main.h"
#include "init.h"

//*****************************
//		Global Variables
//*****************************
char ran1, ran2;
char str[16];
//Initialize
unsigned short int black[10]	= {0,0,0,0,0,0,0,0,0,0};
unsigned short int white[10] 	= {0,0,0,0,0,0,0,0,0,0};
unsigned short int thres[10]	= {0,0,0,0,0,0,0,0,0,0};
unsigned short int adc[10]		= {0,0,0,0,0,0,0,0,0,0};
unsigned char line[10]			= {0,0,0,0,0,0,0,0,0,0};

//*****************************
//		INTERRUPT PROTOTYPES
//*****************************
#pragma config FOSC = INTIO67, LVP = OFF
#pragma code high_vector = 0x08	//setup the high ISR vector
void interrupt_at_high_vector(void) 
	{_asm GOTO high_isr _endasm}
#pragma code
#pragma interrupt high_isr      //the high ISR
void high_isr(void){
	INTCONbits.TMR0IF = 0;
	TMR0H = 0x0B; TMR0L = 0xDB;
   	sprintf(str,"\n\r");
   	SendUART(str);
}
#pragma code low_vector = 0x18	//setup the low ISR vector
void interrupt_at_low_vector(void) 
	{_asm GOTO low_isr _endasm}
#pragma code
#pragma interruptlow low_isr    //the low ISR
void low_isr(void){
	PIR1bits.TMR1IF = 0;
	TMR1H = 0x0B;
	TMR1L = 0xDB;	
	if(ran1 == 0) ran1 = 1;
	else ran1 = 0;
}
//*****************************
//		MAIN FUNCTION
//*****************************
void main(void)
{
	//RA0:AN0   RA1:AN1   RA2:AN2   RA3:AN3   RA5:AN4
	//RB2:AN8   RB3:AN9   RB1:AN10  RB4:AN11  RB0:AN12
    initCLK(OSC_8MHZ);
	initTMR0();
	initPORTS(); 	
	initADC();
	initUART();
	while(1){
		CollectADC();
		if(PORTCbits.RC0 == 0){
			StoreBlack();
			sprintf(str,"Black: %5d %5d %5d %5d %5d %5d %5d %5d %5d %5d\n\r",
					black[0],black[1],black[2],black[3],black[4],
					black[5],black[6],black[7],black[8],black[9]);
			SendUART(str);
		}	
		else if(PORTCbits.RC1 == 0){
			StoreWhite();	
			sprintf(str,"White: %5d %5d %5d %5d %5d %5d %5d %5d %5d %5d\n\r",
					white[0],white[1],white[2],white[3],white[4],
					white[5],white[6],white[7],white[8],white[9]);
			SendUART(str);
		}	
		else if(PORTCbits.RC2 == 0){
			//T0CONbits.TMR0ON = 1;
			sprintf(str,"Starting Race Mode\n\r");
			SendUART(str);
			CalcThreshold();
			break;
		}		 
	}	
	while(1){
		CollectADC();
		sprintf(str,"%5d %5d %5d %5d %5d %5d %5d %5d %5d %5d\n\r",
				adc[0],adc[1],adc[2],adc[3],adc[4],
				adc[5],adc[6],adc[7],adc[8],adc[9]);
		//SendUART(str);
 	}
}
void SendUART(char *c)
{
	char temp;
	int i = 0;

	do{
		temp = c[i++];
		TXREG = temp;
		Delay100TCYx(10);
	} while( c[i] != '\0' );
}

void CollectADC(void){
	SetChanADC(ADC_CH1); ConvertADC(); while(BusyADC()); adc[0] = ReadADC();
 	SetChanADC(ADC_CH1); ConvertADC(); while(BusyADC()); adc[1] = ReadADC();
	SetChanADC(ADC_CH2); ConvertADC(); while(BusyADC()); adc[2] = ReadADC();
 	SetChanADC(ADC_CH3); ConvertADC(); while(BusyADC()); adc[3] = ReadADC();
	SetChanADC(ADC_CH4); ConvertADC(); while(BusyADC()); adc[4] = ReadADC();
	SetChanADC(ADC_CH8); ConvertADC(); while(BusyADC()); adc[5] = ReadADC();
	SetChanADC(ADC_CH9); ConvertADC(); while(BusyADC()); adc[6] = ReadADC();
	SetChanADC(ADC_CH10); ConvertADC();while(BusyADC()); adc[7] = ReadADC();
	SetChanADC(ADC_CH11); ConvertADC();while(BusyADC()); adc[8] = ReadADC();
	SetChanADC(ADC_CH12); ConvertADC();while(BusyADC()); adc[9] = ReadADC();
}	

void StoreWhite(void){
	white[0] = adc[0]; white[1] = adc[1]; white[2] = adc[2]; 
	white[3] = adc[3]; white[4] = adc[4]; white[5] = adc[5]; 
	white[6] = adc[6]; white[7] = adc[7]; white[8] = adc[8]; 
	white[9] = adc[9]; 
}

void StoreBlack(void){
	black[0] = adc[0]; black[1] = adc[1]; black[2] = adc[2]; 
	black[3] = adc[3]; black[4] = adc[4]; black[5] = adc[5]; 
	black[6] = adc[6]; black[7] = adc[7]; black[8] = adc[8]; 
	black[9] = adc[9];  
}		

void CalcThreshold(void){
	int i=0;
	for(;i<10;i++){
		thres[i] = (white[i] + black[i]) >> 1;
	}
	sprintf(str,"Thres: %5d %5d %5d %5d %5d %5d %5d %5d %5d %5d\n\r",
				thres[0],thres[1],thres[2],thres[3],thres[4],
				thres[5],thres[6],thres[7],thres[8],thres[9]);
	SendUART(str);	
			
}	