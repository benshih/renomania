#include <p18f1220.h>
#include <adc.h>

#pragma config WDT = OFF, LVP = OFF, OSC = INTIO2

#define ServoPin LATBbits.LATB3
#define ServoTris TRISBbits.TRISB3
//less than 1500: right

//////////////////////////////////////////////////////////////////
///
///				INTERRUPT CODE
///
//////////////////////////////////////////////////////////////////

void ccp1_isr();                    
int ADCres0, ADCres1, thres0, thres1;
volatile int ServoPos;          //used to hold the servo position
int wait = 0;
int forward = 0;
unsigned char buff;

#pragma code low_vector=0x18    //setup the ISR vector
void low_interrupt (){
  _asm GOTO ccp1_isr _endasm    //jump to interrupt handler
}
#pragma code

#pragma interruptlow ccp1_isr   //the ISR
void ccp1_isr(){
	OpenADC(ADC_FOSC_32 & ADC_RIGHT_JUST,ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_EXT & ADC_VREFMINUS_VSS, 0);
	SetChanADC(ADC_CH0);
	ConvertADC();						
	while(BusyADC());			
	ADCres0 = ReadADC(); //right sensor
	SetChanADC(ADC_CH1);
	ConvertADC();						
	while(BusyADC());			
	ADCres1 = ReadADC(); //left sensor
	CloseADC();
	//ServoPos = 1550;
	
	PORTBbits.RB1 = 0;
	PORTBbits.RB0 = 1;	
	if(wait<0 || wait>3)
		wait = 0;
	if(wait==0 && ADCres0 > thres0 && ADCres1 > thres1)
	{
		ServoPos = 1550;
		forward = 1;
		//wait = 3;
	}
	else if(wait==0 && ADCres0 < thres0 && ADCres1 < thres1)	
	{
		ServoPos = 1550;
	
		PORTBbits.RB0 = 0;
				PORTBbits.RB1 = 1;
		forward = 0;
			//wait = 3;
	}	
 	else if(wait==0 && ADCres0 > thres0 && ADCres1<thres1)//thres && ADCres0 < thres)//PORTAbits.RA4==1
	{

		ServoPos = 1850;	//left
		forward = 1;
			//wait = 3;
	}
	else if(wait==0 && ADCres0 < thres0 && ADCres1>thres1)//ADCres1 < thres && ADCres0 > thres)
	{
		ServoPos = 1200;	//right	
		forward = 1;
			//wait = 3;
	}
	if(ServoPin==1){            //will be 1 if we are at end of pulse
        ServoPin=0;             //turn off servo output
        CCPR1=20000-ServoPos;   //Off time = 20mS - Servo Time
    }
    else{
        ServoPin=1;             //turn on servo output
        CCPR1=ServoPos;         //On time 
    }
    wait--;
    PIR1bits.CCP1IF=0;          //clear int flag
}
#pragma code

/////////////////////////////////////////////////////////////////
///
///				MAIN CODE
///
/////////////////////////////////////////////////////////////////
void init(void)
{
	TRISA = 0xff;	//PORTA is an input
	TRISB = 0x00;	//PORTB is an output
	TRISBbits.TRISB2 = 1;
	PORTB = 0x00;	//PORTB is all low	
	
	//Servo Initialization
	OSCCON=0x70;                //Osc=8MHz
    ServoTris=0;                //make bit 0 output
    ServoPin=0;                 //Servo output off
    CCP1CON=0b00001011;         //Special event trigger
    T1CON=0b10010001;           //Timer 1 on with Pre=2
    //ServoPos=1500;              //set servo to mid position
    CCPR1=ServoPos;             //set CCP initial value   
    PIE1bits.CCP1IE=1;          //enable CCP1 interrupt
    INTCONbits.PEIE=1;          //enable peripheral interrupts
    INTCONbits.GIE=1;           //enable glogal interrupts
    INTCON2bits.RBPU=0;         //enable port b week pullups
}	
void main(void)
{
	int white0, white1, black0, black1;
	init();
	
   	while(1)
   	{	
   		if(PORTAbits.RA6==1)
   		{
			break;
		}	
   		else if(PORTAbits.RA4==1)
		{
			white0 = ADCres0;
			white1 = ADCres1;
			thres0 = (white0+black0)/2;
			thres1 = (white1+black1)/2;	
		}	
		else if(PORTAbits.RA6==1)
		{
			black0 = ADCres0;
			black1 = ADCres1;
			thres0 = (white0+black0)/2;
			thres1 = (white1+black1)/2;	
		}
	}
//    while(1)
//	{
//		/*SetChanADC(ADC_CH0);
//		ConvertADC();						
//		while(BusyADC());			
//		ADCres0 = ReadADC()/2; //right sensor
//		SetChanADC(ADC_CH1);
//		ConvertADC();						
//		while(BusyADC());			
//		ADCres1 = ReadADC()/2; //left sensor
//		*/
//		//ServoPos = 1500;
//		//PORTBbits.RB1 = 0;
//		//PORTBbits.RB0 = 1;
//		if(PORTAbits.RA2==1)//ADCres1 > thres && ADCres0 < thres)
//		{
//			ServoPos = 1600;	//left
//		}
//		else if(PORTAbits.RA4==1)//ADCres1 < thres && ADCres0 > thres)
//		{
//			ServoPos = 1400;	//right	
//		}	
//		else if(ADCres1 < thres && ADCres0 < thres)
//		{
//			//PORTBbits.RB0 = 0;
//			//PORTBbits.RB1 = 1;
//		}
//		
//      if(PORTAbits.RA4==1)
//		{
//			INTCON2bits.RBPU=1;         //disable port b week pullups
//			break;
//		}
//    }
}