	list		p=16f877a	; list directive to define processor
	#include	<p16f877a.inc>	; processor specific variable definitions
	
	__CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _WRT_OFF & _LVP_OFF & _CPD_OFF

start

	bcf STATUS, RP0
	bcf STATUS, RP1
	clrf PORTB
	clrf PORTA
	bsf STATUS, RP0
	clrw
	movwf TRISB
	clrw
	movwf TRISA
	bcf STATUS, RP0

	goto main

main
	bsf PORTA, 0

	movlw d'50'
	call delay1ms

	bcf PORTA, 0

	movlw d'50'
	call delay1ms
	
	goto main
	
delay1ms
	movwf h'71'
part2
	movlw d'250'
	movwf h'70'
part1	;about 100 cycles
	nop
	nop
	decfsz h'70', 1
	goto part1
	decfsz h'71', 1
	goto part2
	return

	END                       ; directive 'end of program'

