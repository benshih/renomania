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
			
initPORTA
	Bank0
	clrf 	PORTA
	Bank1
	clrf	TRISA	
	goto 	initTMR1

initTMR1
	Bank0
	clrf 	INTCON				;Turn off interrupts and flags
	clrf 	PIR1				;Clear TMR1IF 
	clrf	PIE1				;Clear TMR1IE
	movlw	0x04 				;Timer off		, use internal Fosc/4
	movwf 	T1CON				;Don't sycronize, prescaler is 1:1
	goto 	main
	
main
	Bank0
	bsf		T1CON, TMR1ON		;Timer1 on
on
	bsf 	PORTA, 0
on1
	btfss	PIR1, TMR1IF
	goto 	on1
	bcf		PIR1, TMR1IF
off
	bcf		PORTA, 0
off1
	btfss	PIR1, TMR1IF
	goto	off1
	bcf		PIR1, TMR1IF
	goto 	on
	

;--------------------------------
;-----------SUBROUTINES----------
;--------------------------------
	
delay500us
	movlw 	d'250'
	movwf 	h'70'
part1							;about 1000 cycles
	nop				
	nop
	decfsz 	h'70', 1
	goto 	part1
	return

	END                   		;directive 'end of program'
