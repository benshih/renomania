	list		p=16f877a	; list directive to define processor
	#include	<p16f877a.inc>	; processor specific variable definitions
	
	__CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _WRT_OFF & _LVP_OFF & _CPD_OFF

Bank0:		MACRO
			bcf STATUS, RP0
			bcf STATUS, RP1
			ENDM
Bank1:		MACRO
			bsf STATUS, RP0
			bcf STATUS, RP1
			ENDM
Bank2:		MACRO
			bcf STATUS, RP0
			bsf STATUS, RP1
			ENDM
Bank3:		MACRO
			bsf STATUS, RP0
			bsf STATUS, RP1
			ENDM

	ORG 	0x000
	goto 	initPORTB
	
	ORG 	0x004
	incf 	0x70
	movf	0x70, 0
	movwf	PORTB
	bcf		PIR1, TMR1IF		;must turn off manually
	movlw	0xB0				;Setting these two registers
	movwf	TMR1L				;will make the interrupt go 
	movlw	0x3C				;off every 20ms.
	movwf	TMR1H				;
	retfie		
	
initPORTB
	Bank0
	clrf	PORTA
	clrf 	PORTB
	clrf	0x70
	Bank1
	clrf	TRISA
	clrf	TRISB	
	goto 	initTMR1

initTMR1
	Bank0
	clrf 	INTCON				;Turn off interrupts and flags
	clrf 	PIR1				;Clear TMR1IF 
	Bank1
	clrf	PIE1				;Clear TMR1IE
	Bank0
	movlw	0x04 				;Timer off		 , internal Fosc/4
	movwf 	T1CON				;Don't syncronize, prescaler is 1:1
	Bank1
	bsf		PIE1, TMR1IE		;Enable the tmr1 interrupt
	Bank0
	bsf		T1CON, TMR1ON		;Timer1 on
	bsf		INTCON, PEIE
	bsf		INTCON, GIE			;Global interrupt enabled	
	goto 	main
	
main
	bsf		PORTA, 0
	goto 	main

	END