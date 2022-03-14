IDEAL	; getting an error code 2 when trinng to run
MODEL small
STACK 100h
DATASEG
	filename 	db 'readme.txt',0 ; string ending with 0, every cell (...\cell\...) must be up to 8 char long
	fileOpenErrorMessage db 'cant open filename, error code: ', ?, 10, '$' ; string ending with $, place 33 left for error code
	fileOpenErrorCodeIndex dw 33
	fileWriteErrorMessage db "failed to write to filename", 10, '$'
	afterRead db 'After read action', 10, '$'
	afterWrite db 'After write action', 10, '$'
	filehandle 	dw ?
	NumOfBytes dw 100
	Buffer db 400 dup(0), "$"
CODESEG
; file modes:
; ReadOnly mode - 00 = 0
; WriteOnly mode = 01 = 1
; Read/Write mode = 10 = 2

; Open file - open a file and return it's handle.
; input: based on global variable filename in [bp + 2]
; output: based on global variable filehandle in [bp + 4] by reference
proc OpenFile
	mov bp, sp
	
	xor al, al
	
	; lea - Load Effective Address; equevalent to mov dx, offset filename
	mov dx, [bp + 4]; dx holds filename offset
	mov al, 2 ; read/write mode
	mov ah, 3Dh ; file open command value
	int 21h
	
	jc FileOpenError
	
	; return value
	mov bx, [bp + 2]; filehandle offset
	mov [bx], ax
	
	jmp EndFileOpen

FileOpenError :
	; update error code in error message
	mov bx, [fileOpenErrorCodeIndex]
	add al, 30h
	mov [fileOpenErrorMessage+bx], al
	
	mov dx, offset fileOpenErrorMessage
	mov ah, 9h
	int 21h

EndFileOpen:	
	ret 4
endp OpenFile

; Read file
; input: filehandle global variable in [bp+6] to work with 
; 		 and NumOfBytes global variable to read in [bp+4]
; output: Buffer array in [bp+2], Buffer[NumOfBytes] holds bytes read

proc ReadFromFile
	mov bp, sp
	
	mov bx, [bp + 2]
	mov dx,bx ; buffer offset to returned full with NumOfBytes
	
	mov bx, [bp + 6] ; filehandle
	mov cx, [bp + 4] ; NumOfBytes to read
	
	mov ah,3Fh ; file read command value
	int 21h
	
	ret 6
endp ReadFromFile 

; Write file
; input: filehandle global variable to work with in [bp + 4] and NumOfBytes global variable to write in [bp + 2]
; output: Buffer array and NumOfBytes, Buffer[NumOfBytes] holds bytes read

proc WriteToFile
	mov bp, sp
	
	mov bx, [bp + 4]; filehandle
	mov cx, [bp + 2]; NumOfBytes to write from buffer
	
	mov ah,40h ; file write command value
	mov dx,offset Buffer
	int 21h
	
	jnc EndFileWrite
	
	mov dx, offset fileWriteErrorMessage
	mov ah, 9h
	int 21h

EndFileWrite:
	ret
endp WriteToFile

; close file
; input: filehandle global variable in [bp + 2]
proc CloseFile
	mov bp, sp
	
	mov bx, [bp + 2]; filehandle
	
	mov ah,3Eh ; file close command value
	int 21h
	
	ret 2
endp CloseFile

start :
	mov ax, @data
	mov ds, ax
	
	;filehandle = OpenFile(&filename)
	push offset filename
	push offset filehandle; ret value
	call OpenFile
	
	; Buffer = ReadFromFile(filehandle, NumOfBytes)
	push [filehandle]
	push [NumOfBytes]
	push offset Buffer; ret value
	call ReadFromFile
	
	; Console.WriteLine(afterRead)
;	mov dx, offset afterRead
;	mov ah, 09
;	int 21h
	
	mov si, 10
	lea dx, [buffer+si]	; use that to control the hexagons
	mov ah, 9
	int 21h
	
	;filehandle = CloseFile(filehandle)
	push [filehandle]
	call CloseFile
	
	;mov dx, offset Buffer
	;mov ah, 09
	;int 21h
	
	;...
	
	
	
	
exit :
	mov ax, 4c00h
	int 21h
END start
