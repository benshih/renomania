.org 0x0000

main:
	clr 	r16
	clr		r17
loop:
	inc		r17
	add		r16, r17
	cpi		r17, 30
	brne	loop
	sts		0x0200, r16
	
