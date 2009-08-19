	list		p=16f877a	; list directive to define processor
	#include	<p16f877a.inc>	; processor specific variable definitions
	
	__CONFIG _CP_OFF & _WDT_OFF & _BODEN_OFF & _PWRTE_ON & _HS_OSC & _WRT_OFF & _LVP_OFF & _CPD_OFF

; '__CONFIG' directive is used to embed configuration data within .asm file.
; The labels following the directive are located in the respective .inc file.
; See respective data sheet for additional information on configuration word.

;***** VARIABLE DEFINITIONS (examples)

; example of using Shared Uninitialized Data Section
;INT_VAR		UDATA_SHR	   
w_temp		RES     1		; variable used for context saving 
status_temp	RES     1		; variable used for context saving
pclath_temp	RES	1		; variable used for context saving

; example of using Uninitialized Data Section
;TEMP_VAR	UDATA			; explicit address specified is not required
temp_count	RES	1		; temporary variable (example)


; example of using Overlayed Uninitialized Data Section
; in this example both variables are assigned the same GPR location by linker
;G_DATA		UDATA_OVR		; explicit address can be specified
flag		RES	2		; temporary variable (shared locations - G_DATA)

;G_DATA		UDATA_OVR   
count		RES	2		; temporary variable (shared locations - G_DATA)

;**********************************************************************
;RESET_VECTOR	CODE	0x000		; processor reset vector
	nop				; nop required for icd
	movlw	high  start		; load upper byte of 'start' label
	movwf	PCLATH			; initialize PCLATH
	goto	start			; go to beginning of program


;INT_VECTOR	CODE	0x004		; interrupt vector location
;	goto INTERRUPT
;MAIN	CODE
INTERRUPT

	movwf	w_temp			; save off current W register contents
	movf	STATUS,w		; move status register into W register
	movwf	status_temp		; save off contents of STATUS register
	movf	PCLATH,w		; move pclath register into w register
	movwf	pclath_temp		; save off contents of PCLATH register

; isr code can go here or be located as a call subroutine elsewhere

	movf	pclath_temp,w		; retrieve copy of PCLATH register
	movwf	PCLATH			; restore pre-isr PCLATH register contents
	movf	status_temp,w		; retrieve copy of STATUS register
	movwf	STATUS			; restore pre-isr STATUS register contents
	swapf	w_temp,f
	swapf	w_temp,w		; restore pre-isr W register contents
	retfie				; return from interrupt


start

	nop				; code starts here (example)
	banksel	count			; example
	clrf	count			; example

; remaining code goes here

	bcf STATUS, RP0
	bcf STATUS, RP1
	clrf PORTA
	clrf PORTB
	
	bsf STATUS, RP0
	movlw 0x06	;sets all of portA as digital
	movwf ADCON1
	movlw 0x02	;sets just portA4 as input
	movwf TRISA
	clrf TRISB
	clrf h'72'
	
	bcf STATUS, RP0
	
wait	
	bsf PORTA, 0 ;standby lamp is on
	
	movlw d'255'	;wait
	movwf h'70'
d1
	movlw d'255'
	movwf h'71'
d2
	decfsz h'71', 1
	goto d2
	decfsz h'70', 1
	goto d1

	goto standby

standby				
	btfss PORTA, 1 ;skip is intput is HIGH
	goto standby
	goto receive

receive
	bcf PORTA, 0 ;standby lamp is off

;first signal
	movlw d'5'	;delay loop: 261 xtal cycles
	movwf h'70'
pt1
	movlw d'16'
	movwf h'71'
pt2
	decfsz h'71', 1
	goto pt2
	decfsz h'70', 1
	goto pt1

	bcf PORTB, 3
	btfsc PORTA, 1
	bsf PORTB, 3
	;bcf PORTA, 3
	

;second signal
	movlw d'10'	;delay loop: 522 xtal cycles
	movwf h'70'
pt3
	movlw d'16'
	movwf h'71'
pt4
	decfsz h'71', 1
	goto pt4
	clrwdt
	nop
	nop
	decfsz h'70', 1
	goto pt3
	clrwdt

	bcf PORTB, 3
	btfsc PORTA, 1
	bsf PORTB, 3
	;bcf PORTA, 3

;third signal
	movlw d'10'	;delay loop: 522 xtal cycles
	movwf h'70'
pt5
	movlw d'16'
	movwf h'71'
pt6
	decfsz h'71', 1
	goto pt6
	clrwdt
	nop
	nop
	decfsz h'70', 1
	goto pt5
	clrwdt

	bcf PORTB, 3
	btfsc PORTA, 1
	bsf PORTB, 3
	;bcf PORTA, 3
	
;fourth signal	
	movlw d'10'	;delay loop: 522 xtal cycles
	movwf h'70'
pt7
	movlw d'16'
	movwf h'71'
pt8
	decfsz h'71', 1
	goto pt8
	clrwdt
	nop
	nop
	decfsz h'70', 1
	goto pt7
	clrwdt

	bcf PORTB, 3
	btfsc PORTA, 1
	bsf PORTB, 3
	;bcf PORTA, 3

;fifth signal
	movlw d'10'	;delay loop: 522 xtal cycles
	movwf h'70'
pt9
	movlw d'16'
	movwf h'71'
pt10
	decfsz h'71', 1
	goto pt10
	clrwdt
	nop
	nop
	decfsz h'70', 1
	goto pt9
	clrwdt

	bcf PORTB, 3
	btfsc PORTA, 1
	bsf PORTB, 3
	;;bcf PORTA, 3

;sixth signal		
	movlw d'10'	;delay loop: 522 xtal cycles
	movwf h'70'
pt11
	movlw d'16'
	movwf h'71'
pt12
	decfsz h'71', 1
	goto pt12
	clrwdt
	nop
	nop
	decfsz h'70', 1
	goto pt11
	clrwdt

	bcf PORTB, 3
	btfsc PORTA, 1
	bsf PORTB, 3
	;bcf PORTA, 3
		
;seventh signal
	movlw d'10'	;delay loop: 522 xtal cycles
	movwf h'70'
pt13
	movlw d'16'
	movwf h'71'
pt14
	decfsz h'71', 1
	goto pt14
	clrwdt
	nop
	nop
	decfsz h'70', 1
	goto pt13
	clrwdt

	bcf PORTB, 3
	btfsc PORTA, 1
	bsf PORTB, 3
	;bcf PORTA, 3

;eighth signal
	movlw d'10'	;delay loop: 522 xtal cycles
	movwf h'70'
pt15
	movlw d'16'
	movwf h'71'
pt16
	decfsz h'71', 1
	goto pt16
	clrwdt
	nop
	nop
	decfsz h'70', 1
	goto pt15
	clrwdt

	bcf PORTB, 3
	btfsc PORTA, 1
	bsf PORTB, 3
	;bcf PORTA, 3
	
;	movf h'72', 0
;	movwf PORTB
stop
	goto stop
	END                       	; directive 'end of program'

