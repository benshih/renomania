#include <p18f25k20.h>
#include <delays.h>

#pragma config FOSC = INTIO67, LVP = OFF

void main(void)
{
	char a = 0;
	char temp = 0;
	OSCCONbits.IRCF0 = 1;	//4MHz internal oscillator
	OSCCONbits.IRCF1 = 0;	//T = 1ms 
	OSCCONbits.IRCF2 = 1;
	
	TRISA = 0x00;
	TRISB = 0x00;
	
	PORTA = 0x0C;
	
	// 1 and 4
	// 2 and 3
		PORTBbits.RB0=0; //right PMOS off
		PORTBbits.RB1=0; //left PMOS off
		PORTBbits.RB2=1; //right NMOS off
		PORTBbits.RB3=1; //left NMOS off
		PORTB=0x0C;
		Delay10KTCYx(5);
	while(1)
	{
	//	if(a==20) a = 0;
//		if(a>=20) temp = 40 - a;
//		else if(a<20) temp = a;
//		else if(a>40) a=0;
//		LATBbits.LATB0=1 ; //right PMOS on
//		LATBbits.LATB1=0; //left PMOS off
//		LATBbits.LATB2=1; //right NMOS off
//		LATBbits.LATB3=0; //left NMOS on
		if(a>40){
			a=1;
		}	
		
		if(a>20){
			LATB=0x05;
			Delay1KTCYx(40-a);
			LATB=0x0C;
			Delay1KTCYx(a-20);
		}	
		else if(a<=20){
			LATB=0x0A;
			Delay1KTCYx(20-a);
			LATB=0x0C;
			Delay1KTCYx(a);
		}	
		
		//while(1);
//		PORTBbits.RB0=0 ; //right PMOS off
//		PORTBbits.RB1=0; //left PMOS off
//		PORTBbits.RB2=1; //right NMOS off
//		PORTBbits.RB3=1; //left NMOS off

		
		
//		LATBbits.LATB0=0 ; //right PMOS off
//		LATBbits.LATB1=1; //left PMOS on
//		LATBbits.LATB2=0; //right NMOS on
//		LATBbits.LATB3=1; //left NMOS off
		

	//	LATB=0x0A;
	//	Delay1KTCYx(a);
		a++;
//		PORTBbits.RB0=0 ; //right PMOS off
//		PORTBbits.RB1=0; //left PMOS off
//		PORTBbits.RB2=1; //right NMOS off
//		PORTBbits.RB3=1; //left NMOS off
//		PORTB=0x0C;
//		Delay10KTCYx(5);

	}	
	while(1);

}
	