;**********************************************************************
;   This file is a basic code template for object module code         *
;   generation on the PIC16F877A. This file contains the              *
;   basic code building blocks to build upon.  As a project minimum   *
;   the 16F877A.lkr file will also be required for this file to       *
;   correctly build. The .lkr files are located in the MPLAB          *  
;   directory.                                                        *
;                                                                     *
;   If interrupts are not used all code presented between the         *
;   code section "INT_VECTOR and code section "MAIN" can be removed.  *
;   In addition the variable assignments for 'w_temp' and             *
;   'status_temp' can be removed.                                     *                         
;                                                                     *
;   If interrupts are used, as in this template file, the 16F877A.lkr *
;   file will need to be modified as follows: Remove the lines        *
;     CODEPAGE   NAME=vectors  START=0x0      END=0x4      PROTECTED  *
;   and                                                               *
;     SECTION    NAME=STARTUP  ROM=vectors                            *
;   and change the start address of the page0 section from 0x5 to 0x0 *                                                               *
;                                                                     *
;   Refer to the MPASM User's Guide for additional information on     *
;   features of the assembler and linker (Document DS33014).          *
;                                                                     *
;   Refer to the respective PIC data sheet for additional            *
;   information on the instruction set.                               *
;                                                                     *
;**********************************************************************
;                                                                     *
;    Filename:	    xxx.asm                                           *
;    Date:                                                            *
;    File Version:                                                    *
;                                                                     *
;    Author:                                                          *
;    Company:                                                         *
;                                                                     * 
;                                                                     *
;**********************************************************************
;                                                                     *
;    Files required:                                                  *
;                                                                     *
;                                                                     *
;                                                                     *
;**********************************************************************
;                                                                     *
;    Notes:                                                           *
;                                                                     *
;                                                                     *
;                                                                     *
;                                                                     *
;**********************************************************************


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

TEN EQU 0x69
	movlw d'10'
	movwf TEN
DIGIT1 EQU 0x70
	movlw d'0'
	movwf DIGIT1
DIGIT2 EQU 0x71
	movlw d'0'
	movwf DIGIT2	
DIGIT3 EQU 0x72
	movlw d'0'
	movwf DIGIT3
	
; remaining code goes here

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

incUnits
	incf DIGIT1, 1
	movlw d'9'
	subwf DIGIT1, 0
	movwf h'75'
	decfsz h'75', 1
	goto incUnits

incTens
	incf DIGIT2, 1
	movlw d'9'
	subwf DIGIT2, 0
	movwf h'75'
	decfsz h'75', 1
	goto incUnits
	



	goto print

print

	bcf PORTA, 2
	movf DIGIT1, 0
	call bin2seg
	movwf PORTB
	bsf PORTA, 0
	clrwdt
	nop


	bcf PORTA, 0
	movf DIGIT2, 0
	call bin2seg
	movwf PORTB
	bsf PORTA, 1
	clrwdt
	nop

	bcf PORTA, 1
	movf DIGIT3, 0
	call bin2seg
	movwf PORTB
	bsf PORTA, 2
	clrwdt
	nop

	goto print
	
bin2seg
	addwf PCL, f
	
	retlw b'01111110'
	retlw b'00101000'
	retlw b'01011011'
	retlw b'01101011'
	retlw b'00101101'
	retlw b'01100111'
	retlw b'01110111'
	retlw b'01101000'
	retlw b'01111111'
	retlw b'00101111'
	
	END                       	; directive 'end of program'

