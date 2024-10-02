.section text
.extern kstart
.global _start

_start:
	xor %rbp, %rbp
	jmp kstart
