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

	movlw b'11001100'
	movwf PORTB
	goto main

main

	rrf PORTB
	call delay
	call delay
	call delay
	call delay
	call delay
	call delay
	call delay
	call delay
	call delay
	call delay
	call delay
	call delay
	call delay
	call delay
	call delay
	call delay
	goto main
	
delay
	movlw d'250'
	movwf h'71'
ptx
	movlw d'250'
	movwf h'70'
pty
	nop
	nop
	decfsz h'70', 1
	goto pty
	decfsz h'71', 1
	goto ptx
	return


	END                       	; directive 'end of program'

