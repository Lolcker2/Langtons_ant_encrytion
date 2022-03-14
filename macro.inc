Macro calloc arg1	; pointer allocation macro
	push cx	; save cx, cuz going to use it
	mov cl, [arg1]	; the allocated pointer's value be arg1 
	call fake_push	; allocating
	pop cx
endm


Macro _% agr1 , agr2
	push [arg1]
	push [arg2]
	call Modulo
endm _%


Macro println arg1
	mov dx, arg1
	call print
endm println
