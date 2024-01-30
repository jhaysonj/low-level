section .data
msg: db "Gris UFRJ", 10
len: equ $-msg

section .text
global _start

_start_:
mov edx, len
mov ecx, msg
mov ebx, 1
mov eax, 4; write
int 0x80

mov ebx, 0
mov eax, 1
int 0x80
