#include "main.h"
#include "init.h"

//*****************************
//		Global Variables
//*****************************
char servo=0;
char str[16];

//Initialize
unsigned short int black[10]	= {0,0,0,0,0,0,0,0,0,0};
unsigned short int white[10] 	= {0,0,0,0,0,0,0,0,0,0};
unsigned short int thres[10]	= {0,0,0,0,0,0,0,0,0,0};
unsigned short int adc[10]		= {0,0,0,0,0,0,0,0,0,0};
unsigned short int servosPosH[9]= {0,0,0,0,0,0,0,0,0};
unsigned short int servosPosL[9]= {0,0,0,0,0,0,0,0,0};
unsigned short int servosHigh, servosLow;

/**Added**/
unsigned short int splitCounter=0;
unsigned short int detectingWhite=10;
unsigned short int splitOpStarts=0;
unsigned short int prev_line_thres=0;

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
	if(servo==0){
		//TMR0H = 0xFE; TMR0L = 0x24; //left 1.9
		//TMR0H = 0xFE; TMR0L = 0x3D; //left 1.8
		//TMR0H = 0xFE; TMR0L = 0x56; //left 1.7
		//TMR0H = 0xFE; TMR0L = 0x6F; //left 1.6
		//TMR0H = 0xFE; TMR0L = 0x88; //center 1.5
		//TMR0H = 0xFE; TMR0L = 0xA1; //right 1.4
		//TMR0H = 0xFE; TMR0L = 0xBA; //right 1.3
		//TMR0H = 0xFE; TMR0L = 0xD3; //right 1.2
		//TMR0H = 0xFE; TMR0L = 0xEC; //right 1.1
		WriteTimer0(servosHigh);
		servo = 1;
		PORTAbits.RA4 = 1;
	}
	else if(servo==1){
		//TMR0H = 0xED; TMR0L = 0x8A; //left 18.9
		//TMR0H = 0xED; TMR0L = 0xA3; //left 18.8
		//TMR0H = 0xED; TMR0L = 0xBC; //left 18.7
		//TMR0H = 0xED; TMR0L = 0xD5; //left 18.6
		//TMR0H = 0xED; TMR0L = 0xEE; //center
		//TMR0H = 0xEE; TMR0L = 0x07; //right 18.4
		//TMR0H = 0xEE; TMR0L = 0x20; //right 18.3
		//TMR0H = 0xEE; TMR0L = 0x39; //right 18.2
		//TMR0H = 0xEE; TMR0L = 0x52; //right 18.1
		WriteTimer0(servosLow);
		servo = 0;
		PORTAbits.RA4 = 0;
	}		
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
	while(1){ //set up loop
		CollectADC();
		if(PORTCbits.RC0 == 0) StoreBlack(); //stores and prints black values	
		else if(PORTCbits.RC1 == 0)	StoreWhite(); //stores and prints white values	
		else if(PORTCbits.RC2 == 0){
			sprintf(str,"Starting Race Mode\n\r");
			SendUART(str);
			CalcThreshold();
			CalcServosPos();
			T0CONbits.TMR0ON = 1;
			break;
		}		 
	}	
	while(1){ //race mode loop
		CollectADC();
		/*sprintf(str,"%5d %5d %5d %5d %5d %5d %5d %5d %5d %5d\n\r",
				adc[0],adc[1],adc[2],adc[3],adc[4],
				adc[5],adc[6],adc[7],adc[8],adc[9]);
		SendUART(str);*/
		
		/** TRACE THE LINE!! **/
		LineTrace();
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
 	SetChanADC(ADC_CH3); ConvertADC(); while(BusyADC()); adc[9] = ReadADC();
	SetChanADC(ADC_CH4); ConvertADC(); while(BusyADC()); adc[3] = ReadADC();
	SetChanADC(ADC_CH8); ConvertADC(); while(BusyADC()); adc[4] = ReadADC();
	SetChanADC(ADC_CH9); ConvertADC(); while(BusyADC()); adc[5] = ReadADC();
	SetChanADC(ADC_CH10); ConvertADC();while(BusyADC()); adc[6] = ReadADC();
	SetChanADC(ADC_CH11); ConvertADC();while(BusyADC()); adc[7] = ReadADC();
	SetChanADC(ADC_CH12); ConvertADC();while(BusyADC()); adc[8] = ReadADC();
}	

void StoreWhite(void){
	white[0] = adc[0]; white[1] = adc[1]; white[2] = adc[2]; 
	white[3] = adc[3]; white[4] = adc[4]; white[5] = adc[5]; 
	white[6] = adc[6]; white[7] = adc[7]; white[8] = adc[8]; 
	white[9] = adc[9];
	sprintf(str,"White: %5d %5d %5d %5d %5d %5d %5d %5d %5d %5d\n\r",
			white[0],white[1],white[2],white[3],white[4],
			white[5],white[6],white[7],white[8],white[9]);
	SendUART(str); 
}

void StoreBlack(void){
	black[0] = adc[0]; black[1] = adc[1]; black[2] = adc[2]; 
	black[3] = adc[3]; black[4] = adc[4]; black[5] = adc[5]; 
	black[6] = adc[6]; black[7] = adc[7]; black[8] = adc[8]; 
	black[9] = adc[9]; 
	sprintf(str,"Black: %5d %5d %5d %5d %5d %5d %5d %5d %5d %5d\n\r",
				black[0],black[1],black[2],black[3],black[4],
				black[5],black[6],black[7],black[8],black[9]);
	SendUART(str); 
}		

void CalcThreshold(void){
	short i=0;
	for(;i<10;i++){
		thres[i] = (white[i] + black[i]) >> 1;
	}
	sprintf(str,"Thres: %5d %5d %5d %5d %5d %5d %5d %5d %5d %5d\n\r",
				thres[0],thres[1],thres[2],thres[3],thres[4],
				thres[5],thres[6],thres[7],thres[8],thres[9]);
	SendUART(str);	
			
}	

void CalcServosPos(void) {
		/** Servos Position for High Values **/
		servosPosH[0] = 0xFE24; //left 1.9
		servosPosH[1] = 0xFE3D; //left 1.8
		servosPosH[2] = 0xFE56; //left 1.7
		servosPosH[3] = 0xFE6F; //left 1.6
		servosPosH[4] = 0xFE88; //center 1.5
		servosPosH[5] = 0xFEA1; //right 1.4
		servosPosH[6] = 0xFEBA; //right 1.3
		servosPosH[7] = 0xFED3; //right 1.2
		servosPosH[8] = 0xFEEC; //right 1.1
		
		/** Servos Position for Low Values **/
		servosPosL[0] = 0xED8A; //left 18.9
		servosPosL[1] = 0xEDA3; //left 18.8
		servosPosL[2] = 0xEDBC; //left 18.7
		servosPosL[3] = 0xEDD5; //left 18.6
		servosPosL[4] = 0xEDEE; //center
		servosPosL[5] = 0xEE07; //right 18.4
		servosPosL[6] = 0xEE20; //right 18.3
		servosPosL[7] = 0xEE39; //right 18.2
		servosPosL[8] = 0xEE52; //right 18.1		
}

void LineTrace(void) {
	//Split branches to the LEFT
	if(SplitTraceOne() == -1) {
		servosHigh = servosPosH[0];
		servosLow = servosPosL[0];
		SplitTraceOne();
	}
	
	//Split branches to the RIGHT
	else if(SplitTraceOne() == 1) {
		servosHigh = servosPosH[8];
		servosLow = servosPosL[8];
		SplitTraceOne();
	}
	
	//We want to trace staright line (includes the split case)
	else if(SplitTraceOne() == 0) {
		StraightTrace();
	}

	StraightTrace();
	
	/**
	if(splitOpStarts==0) {
		StraightTrace();
		SplitTraceTwo();
	}
	else {
	
	switch(SplitTraceTwo()) {
	case 0:
		//Line Trace	
		StraightTrace();
		break;
	case 1: 
		//Turn Right
		servosHigh = servosPosH[8];
		servosLow = servosPosL[8];
		StraightTrace();
		break;
	case 2:
		//Turn Left
		servosHigh = servosPosH[0];
		servosLow = servosPosL[0];
		StraightTrace();
		break;
	case 3: 
		//Go straight
		StraightTrace();
		break;
	case 4:
		//Go Straight
		StraightTrace();
		break;
	case 5:
		//Go Straight
		StraightTrace();
		break;
	case 6:
		//Turn Right
		servosHigh = servosPosH[4];
		servosLow = servosPosL[4];
		StraightTrace();
		break;
	case 7:
		//Go Straight
		StraightTrace();
		break;
	case 8:
		//Go Straight
		StraightTrace();
		break;
	default:
		StraightTrace();
		break;
	}

	}
	**/

}

/* I've added this chunk of code */
//Split check that uses weightage in line area
short SplitTraceOne(void) {
  	unsigned short int left=0,right=0,i=5,j=0;

	//If all sensors are on white line (dense area)
	if(adc[0]&&adc[1]&&adc[2]&&
	   adc[3]&&adc[4]&&adc[5]&&
	   adc[6]&&adc[7]&&adc[8]) {

		//Compute # of left and right sensors on or off white line	
		for(;j<4; j++) {
			if(adc[j] > thres[j]) {
				left++;
			}
		}
		for(;i<9;i++) {
		   	if(adc[i] > thres[i]) {
		   		right++;
		   	}
		}

		//If the RIGHT white line area is more dense
		if(right > left) {
			return 1;
		}

		//If the LEFT white line area is more dense
 		if(right < left) {
			return -1;
		}

		//If the RIGHT and LEFT whie line area is same dense
		if(right == left) {
			return 0;
		}
	}

	return 0;
}

//Split check that uses a counter
short SplitTraceTwo(void) {
  	if(adc[0]&&adc[1]&&adc[2]&&
	   adc[3]&&adc[4]&&adc[5]&&
	   adc[6]&&adc[7]&&adc[8]) {
		  splitOpStarts = 1;
		  detectingWhite = 1;
  	}
	
    else if(!(adc[0]&&adc[1]&&adc[2]&&
	    	  adc[3]&&adc[4]&&adc[5]&&
	    	  adc[6]&&adc[7]&&adc[8]) {
		  detectingWhite = 0;
   	}

    if(!detectingWhite) {
		splitCounter++;
		splitOpStarts=0;
  	}
  }
	
  return splitCounter;
}


void StraightTrace(void) {
  for(;i<9;i++) {
	  	  if(adc[i] > thres[i]) {
			  line_thres += i;
			  count++;
		  }
	  } 

	//if something goes wrong - go off track
	if(count==0) {
		line_thres = prev_line_thres;
	}
	else {
	  line_thres = (line_thres)/count; 
	}
	
	 servosHigh = servosPosH[line_thres];
	 servosLow = servosPosL[line_thres];
	 prev_line_thres = line_thres;	
}