#include <p18f1220.h>
#include <timers.h>

#define buffA 0x80
#define buffB 0x81
#define column 0x82
unsigned char col0buff,col1buff,col2buff,col3buff,col4buff;

void upCol_isr(void);    
void updateBuffers(void);
void updatePorts(void);
void init(void);
void init_tmr(void);

/////////////////////////////////////////////////////////////////
///
///				INTERRUPT CODE
///
/////////////////////////////////////////////////////////////////
               
#pragma code low_vector=0x18    //setup the ISR vector
void low_interrupt (){
  _asm GOTO upCol_isr _endasm   //jump to interrupt handler
}
#pragma code

#pragma interruptlow upCol_isr  //the ISR
void upCol_isr(){
	INTCONbits.TMR0IF=0;
	//write to timer

	_asm 
		movlw 0x5E
		andwf buffA, 1, 1
	_endasm

	updatePorts();		
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
}	
void init_tmr(void)
{
	OSCCON=0x77;                //Osc=16MHz
	
    INTCONbits.TMR0IE=1;		//timer0 interrupt enabled
    INTCON2bits.TMR0IP=0;		//priority: low
    INTCONbits.GIE=1;           //glogal interrupts enabled
    INTCONbits.PEIE=1;          //periph interrupts enabled
    //write to timer
	_asm
		movlw	0x84			//timer enable
		movwf	T0CON, 1		//prescaled 1:32, 16 bits
								//internal clock on rising edge
		movlw	0xF0			//timer counts 256 times
		movwf	TMR0H, 1
		movlw	0x00
		movwf	TMR0L, 1								
	_endasm
		
    //open timer
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