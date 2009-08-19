#include <p18f1220.h>
#pragma config WDT = OFF, LVP = OFF, OSC = INTIO2

#define ServoPin LATBbits.LATB3
#define ServoTris TRISBbits.TRISB3

void ccp1_isr();                    

volatile int ServoPos;          //used to hold the servo position

#pragma code low_vector=0x18    //setup the ISR vector
void low_interrupt (){
  _asm GOTO ccp1_isr _endasm    //jump to interrupt handler
}
#pragma code

#pragma interruptlow ccp1_isr   //the ISR
void ccp1_isr(){
    if(ServoPin==1){            //will be 1 if we are at end of pulse
        ServoPin=0;             //turn off servo output
        CCPR1=20000-ServoPos;   //Off time = 20mS - Servo Time
    }
    else{
        ServoPin=1;             //turn on servo output
        CCPR1=ServoPos;         //On time 
    }
    PIR1bits.CCP1IF=0;          //clear int flag
}
#pragma code

void main()
{
    OSCCON=0x70;                //Osc=8MHz
    ServoTris=0;                //make bit 0 output
    ServoPin=0;                 //Servo output off
    CCP1CON=0b00001011;         //Special event trigger
    T1CON=0b10010001;           //Timer 1 on with Pre=2
    ServoPos=1500;              //set servo to mid position
    CCPR1=ServoPos;             //set CCP initial value   
    PIE1bits.CCP1IE=1;          //enable CCP1 interrupt
    INTCONbits.PEIE=1;          //enable peripheral interrupts
    INTCONbits.GIE=1;           //enable glogal interrupts
    INTCON2bits.RBPU=0;         //enable port b week pullups
    while(1)
	{                   //loop forever
    	ServoPos=1500;          //set center
    }
}