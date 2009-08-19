.equ DDRB=0x17
.equ PORTB=0x18
.equ PINB=0x16
.equ DDRD=0x11
.equ PORTD=0x12
.equ PIND=0x10

.org 0x0000
entry:
	ldi r16,0xFF
	out DDRD,r16
	ldi r16,0x00
	out DDRB,r16
	ldi r16,0xff
	out PORTB,r16

main:
	in r16,PINB
	mov r17,r16
	inc r17
	mul	r16, r17
	lsr r0
	out PORTD,r0
	rjmp main
