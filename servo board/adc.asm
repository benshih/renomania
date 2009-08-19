;****************************************
;
; ADC test code
; Renomania Electronics
;
; PIC16F877a/10Mhz
; 	RA0     : analog input
;   PORTB	: digital output
;
; 5/26/2008
;****************************************


	list		p=16f877a	; list directive to define processor
	#include	<p16f877a.inc>	; processor specific variable definitions
	
	__CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _WRT_OFF & _LVP_OFF & _CPD_OFF

;----------------------------
;-----------MACROs-----------
;----------------------------
Bank0:	MACRO
		bcf STATUS, RP0
		bcf STATUS, RP1
		ENDM
Bank1:	MACRO
		bsf STATUS, RP0
		bcf STATUS, RP1
		ENDM
Bank2:	MACRO
		bcf STATUS, RP0
		bsf STATUS, RP1
		ENDM
Bank3:	MACRO
		bsf STATUS, RP0
		bsf STATUS, RP1
		ENDM
;---------------------------
;--------Main Code----------
;---------------------------

initIO
	Bank0
	clrf	PORTA
	clrf	PORTB	
	Bank1
	movlw 	0x07
	movwf	TRISA 	;RA0,1,2 = input
	clrf	TRISB 	;RB  = output 
initADC
	Bank1
	movlw	0x84   	;AN0 AN1 AN3
	movwf 	ADCON1 	;Vref+ = Vcc
					;Right just
main
	Bank0
	movlw 	0x01	;Fosc/2, AN0, no GO, ADON
	movwf	ADCON0 	
	call 	delay.05ms
GoADC
	;Start ADC now
	bsf		ADCON0, 2 ;GO!
checkDONE
	btfsc 	ADCON0, 2 ;done?
	goto 	checkDONE ;if not
	;if DONE
	Bank1
	movf	ADRESL, 0
	Bank0
	movwf	h'75'	
;	movwf 	PORTB
	
	btfsc 	PORTA, 2
	goto 	release
	goto 	grab
grab
	bsf 	PORTA, 5
	call 	delay.5ms
	call 	delay.1ms
	call 	delay.1ms
	call	delay.1ms

	bsf		PORTB, 7
	call 	delay.5ms
	call 	delay.1ms
	call	delay.1ms
	call 	delay.1ms
	bcf 	PORTA, 5
	goto 	dly4us
release	
	bsf 	PORTA, 5
	call	delay.1ms
	call 	delay.1ms
	call 	delay.1ms
	call 	delay.1ms
	bsf		PORTB, 7
	call 	delay.5ms
	call	delay.1ms
	call 	delay.1ms
	call 	delay.1ms
	bcf 	PORTA, 5
	goto 	dly4us


dly4us
	call 	delay
	decfsz	h'75', 1
	goto 	dly4us
	bcf		PORTB, 7

	call 	delay10ms
	
	goto 	GoADC
	 
;----------------------------
;-------SUBROUTINES----------
;----------------------------

delay ;30 xtal cycles
	movlw 	d'9'
	movwf 	h'70'
pta	
	decfsz	h'70', 1
	goto 	pta
	nop
	return


delay.05ms ;125 xtal cycles
	movlw d'24'
	movwf h'70'
pt0
	nop
	nop
	decfsz h'70', 1
	goto pt0
	nop
	nop
	nop
	return

delay.1ms ;250 xtal cycles
	movlw d'49'
	movwf h'70'
pt1
	nop
	nop
	decfsz h'70', 1
	goto pt1
	nop
	nop
	nop
	return
	
delay.5ms ;1250 xtal cycles
	movlw d'5'
	movwf h'71'
pt2
	movlw d'49'
	movwf h'70'
pt3
	nop
	nop
	decfsz h'70', 1
	goto pt3
	decfsz h'71', 1
	goto pt2
	return

delay10ms ;25000 xtal cycles
	movlw d'25'
	movwf h'71'
ptx
	movlw d'199'
	movwf h'70'
pty
	nop
	nop
	decfsz h'70', 1
	goto pty
	decfsz h'71', 1
	goto ptx
	return
	
delay20ms ;50000 xtal cycles
	movlw d'50'
	movwf h'71'
pt4
	movlw d'199'
	movwf h'70'
pt5
	nop
	nop
	decfsz h'70', 1
	goto pt5
	decfsz h'71', 1
	goto pt4
	return
	
delay100ms ;250000 xtal cycles
	movlw d'250'
	movwf h'71'
pt6
	movlw d'199'
	movwf h'70'
pt7
	nop
	nop
	decfsz h'70', 1
	goto pt7
	decfsz h'71', 1
	goto pt6
	return
	


	END                       ; directive 'end of program'

