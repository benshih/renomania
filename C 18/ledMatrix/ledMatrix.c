#include <p18f1220.h>
#include <timers.h>

#pragma config LVP=OFF, OSC=INTIO2, WDT=OFF

#define buffA 0x80
#define buffB 0x81
#define column 0x82
//unsigned char buffB;
unsigned char col0buff,col1buff,col2buff,col3buff,col4buff;
unsigned char columnCount;

/////////////////////////////////////////////////////////////////
///
///				INTERRUPT CODE
///
/////////////////////////////////////////////////////////////////

void upCol_isr(void);    
void updateBuffers(void);
void updatePorts(void);
void init(void);
void init_tmr(void);
                


#pragma code low_vector=0x18    //setup the ISR vector
void low_interrupt (){
  _asm GOTO upCol_isr _endasm   //jump to interrupt handler
}
#pragma code

#pragma interruptlow upCol_isr  //the ISR
void upCol_isr(){
	INTCONbits.TMR0IF=0;
	WriteTimer0(0xFF00);
	//buffA = buffA & 0x5E;			//preserve MCLR, AN0, RA7
	
	_asm 
		movlw 0x5E
		andwf buffA, 1, 1
	_endasm
	/*
	if(columnCount==0)
	{
		buffA = buffA | 0x02;
		buffB = col0buff;
	}
	else if(columnCount==1)
	{
		buffA = buffA | 0x04;
		buffB = col1buff;
	}
	else if(columnCount==2)
	{
		buffA = buffA | 0x08;
		buffB = col2buff;
	}
	else if(columnCount==3)
	{
		buffA = buffA | 0x10;
		buffB = col3buff;
	}
	else if(columnCount==4)
	{
		buffA = buffA | 0x40;
		buffB = col4buff;
	}*/
	updatePorts();
	columnCount++;
	if(columnCount==5)
		columnCount = 0;		
}
#pragma code

/////////////////////////////////////////////////////////////////
///
///				MAIN CODE
///
/////////////////////////////////////////////////////////////////

void main(void)
{
	init();
	init_tmr();
	while(1)
	{
	}
}   

/////////////////////////////////////////////////////////////////
///
///				FUNCTIONS
///
/////////////////////////////////////////////////////////////////

void init(void)
{
	TRISA = 0x00;
	TRISB = 0x00;		
	PORTA = 0xFF;
	PORTB = 0x00;
	
	columnCount = 0x00;
}	
void init_tmr(void)
{
	OSCCON=0x70;                //Osc=8MHz
	
    INTCONbits.TMR0IE=1;		//timer0 interrupt enabled
    INTCON2bits.TMR0IP=0;		//priority: low
    INTCONbits.GIE=1;           //glogal interrupts enabled
    INTCONbits.PEIE=1;          //periph interrupts enabled
	WriteTimer0(0xFF00);
	OpenTimer0( TIMER_INT_ON &	//interrupt: on
				T0_16BIT &		//counter: 16bit
				T0_SOURCE_INT &	//oscillator: internal
				T0_PS_1_32 );	//prescaler: 1 to 32
				
	
}	
void wait(int delay)
{
	int x = 0;
	while(x<=delay)
		x++;
}

void updatePorts(void)
{	
	_asm
		movff 	PORTA, buffA
		movff 	PORTB, buffB
	_endasm
}	

void nextCol(void)
{
	_asm
		incf 	column, 1, 1
		movf 	column, 0, 1
		addwf 	PCL, 1, 1
		
		retlw 	0x02
		retlw	0x04
		retlw	0x08
		retlw	0x10
		retlw	0x40	
	_endasm
}	