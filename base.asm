IDEAL
MODEL small
STACK 100h
DATASEG
; --------------------------
; Your variables here

string dw ?


xpos db ? 	; the x component of the ant's position
ypos db ?	; the y component of the ant's position
dir db 0	; the direction of the ant
Malloc db 120 dup(0)	; "fake" stack
malloc_index dw 0	; the index of the "fake" stack
lastest_element dw ? 	; the pop mem reference

buffer_x db ?
buffer_y db ?
buffer_i db ?
WIDTH db 5
HEIGHT db 5

; --------------------------
CODESEG
include "utils.inc"	; where all procs are definedma

;all of the macros
	

Macro calloc arg1	; pointer allocation macro
	push cx	; save cx, cuz going to use it
	mov cl, [arg1]	; the allocated pointer's value be arg1 
	call fake_push	; allocating
	pop cx
endm
	; might add that to calloc
	; call get_pointer
	; lea mod, [stack+sp]




Macro _% agr1 , agr2	; the macro for calling modulo
	push [arg1]		; pushed arg1
	push [arg2]		; pushed arg2
	call Modulo		; calls the modulo proc
endm _%


Macro println arg1	; a quick way to print stuff
	mov dx, arg1	; insert arg1, into dx
	call print		; calls the print proc
endm println





start:
	mov ax, @data
	mov ds, ax	
; --------------------------

	

; --------------------------
	
exit:
	mov ax, 4c00h
	int 21h
END start
