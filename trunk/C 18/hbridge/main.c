#include <p18f25k20.h>
#include <delays.h>

#pragma config FOSC = INTIO67, LVP = OFF

void main(void)
{
	OSCCONbits.IRCF0 = 1;	//4MHz internal oscillator
	OSCCONbits.IRCF1 = 0;	//T = 1ms 
	OSCCONbits.IRCF2 = 1;
	
	TRISA = 0x00;
	TRISB = 0x00;
	
	PORTA = 0x0C;
	
	// 1 and 4
	// 2 and 3
//	while(1)
//	{
		PORTBbits.RB0=0; //right PMOS off
		PORTBbits.RB1=0; //left PMOS off
		PORTBbits.RB2=1; //right NMOS off
		PORTBbits.RB3=1; //left NMOS off
		PORTB=0x0C;
		Delay10KTCYx(1000);
		//while(1);
		PORTBbits.RB0=1 ; //right PMOS on
		PORTBbits.RB1=0; //left PMOS off
		PORTBbits.RB2=1; //right NMOS off
		PORTBbits.RB3=0; //left NMOS on
		PORTB=0x05;
		Delay10KTCYx(1000);
		//while(1);
		PORTBbits.RB0=0 ; //right PMOS off
		PORTBbits.RB1=0; //left PMOS off
		PORTBbits.RB2=1; //right NMOS off
		PORTBbits.RB3=1; //left NMOS off
		//PORTB=0x3A;
		PORTA=0x0C;
		Delay10KTCYx(1000);
		PORTBbits.RB0=0 ; //right PMOS off
		PORTBbits.RB1=1; //left PMOS on
		PORTBbits.RB2=0; //right NMOS on
		PORTBbits.RB3=1; //left NMOS off
		PORTB=0x0A;
		Delay10KTCYx(1000);
		PORTBbits.RB0=0 ; //right PMOS off
		PORTBbits.RB1=0; //left PMOS off
		PORTBbits.RB2=1; //right NMOS off
		PORTBbits.RB3=1; //left NMOS off
		PORTB=0x0C;
		Delay10KTCYx(1000);
		while(1)
		PORTAbits.RA7=0; //right PMOS off
		PORTAbits.RA2=0; //left PMOS off
		PORTAbits.RA4=1; //right NMOS off
		PORTAbits.RA3=1; //left NMOS off*/
//	}	
	while(1);

}
	