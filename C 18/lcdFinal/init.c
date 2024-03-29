void initTMR0(void){
	T0CON = 0x03;	//Internal Clk,		INC on Low->High
					//8-bit Control,	Use 1:16 prescaler
					//Timer0 OFF
	TMR0H = 0x0B;	//Write 0x0BDB
	TMR0L = 0xDB;
	
    /** START YOUR ENGINES!! **/
   	INTCONbits.TMR0IE = 1;  //Enable TMR0 interrupt
    INTCONbits.PEIE = 1;    //Turn on LP interrupts
    INTCONbits.GIE = 1;     //Turn on HP interrupts
}	

void initPORTS(void){
	TRISA = 0x2F;	//0x2F Analog In
    TRISB = 0x1F;	//0x1F Analog In
    PORTA = 0x00;
    PORTB = 0x00;
}    

void initADC(void){
	OpenADC(ADC_FOSC_32	& ADC_RIGHT_JUST,
			ADC_CH0 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 
			0);
	OpenADC(ADC_FOSC_32	& ADC_RIGHT_JUST,
			ADC_CH1 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 
			0); 
	OpenADC(ADC_FOSC_32	& ADC_RIGHT_JUST,
			ADC_CH2 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 
			0); 
	OpenADC(ADC_FOSC_32	& ADC_RIGHT_JUST,
			ADC_CH3 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 
			0); 
	OpenADC(ADC_FOSC_32	& ADC_RIGHT_JUST,
			ADC_CH4 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 
			0); 
	OpenADC(ADC_FOSC_32	& ADC_RIGHT_JUST,
			ADC_CH8 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 
			0); 
	OpenADC(ADC_FOSC_32	& ADC_RIGHT_JUST,
			ADC_CH9 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 
			0); 
	OpenADC(ADC_FOSC_32	& ADC_RIGHT_JUST,
			ADC_CH10 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 
			0); 
	OpenADC(ADC_FOSC_32	& ADC_RIGHT_JUST,
			ADC_CH11 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 
			0); 	
	OpenADC(ADC_FOSC_32	& ADC_RIGHT_JUST,
			ADC_CH12 & ADC_INT_OFF & ADC_VREFPLUS_VDD & ADC_VREFMINUS_VSS, 
			0); 	 	
}	