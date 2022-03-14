proc Movement basic far
uses ax, bx
	mov bl, [dir]
	cmp bl, 00	; dir is '00'
	jne Movement2	; jump to the next 'if'
	mov al, [ypos]
	dec al
	mov [ypos], al	; return ypos--
	jmp Movement_middle_jmp	; reutrn
	
Movement2:
	cmp bl, 01	; the dir is 1
	jne Movement3	; jump to the next 'if'
	mov al, [ypos]
	dec al
	mov [ypos], al	; return ypos--
	
	mov al, [xpos]
	dec al
	mov [xpos], al	;return xpos--
	jmp Movement_middle_jmp	; retrn
	
Movement3:
	cmp bl, 02	; the dir is 2
	jne Movement4	; jump to the next 'if'
	mov al, [ypos]
	inc al
	mov [ypos], al	; return ypos++
	
	mov al, [xpos]
	dec al
	mov [xpos], al	;return xpos--
	jmp Movement_middle_jmp	; retrn

	
Movement_middle_jmp:	; cuz there is a limit to jump HEIGHT, countering that using a "check point".
		jmp Movement_end
	
Movement4:
	cmp bl, 03	; the dir is 'u'
	jne Movement5	; jump to the next 'if'
	mov al, [ypos]
	inc al
	mov [ypos], al	; return ypos++
	jmp Movement_middle_jmp	; retrn
	
Movement5:
	cmp bl, 04	; the dir is 'l'
	jne Movement6	; jump to the next 'if'
	mov al, [ypos]
	inc al
	mov [ypos], al	; return ypos++
	
	mov al, [xpos]
	inc al
	mov [xpos], al	;return xpos++
	jmp Movement_middle_jmp	; retrn
	
Movement6:	
	cmp bl, 05	; the dir is 'y'
	jne Movement_end	; jump to the next 'if'
	mov al, [ypos]
	dec al
	mov [ypos], al	; return ypos--
	
	mov al, [xpos]
	inc al
	mov [xpos], al	;return xpos++
	jmp Movement_middle_jmp	; retrn
	
Movement_end:
	ret
endp Movement



proc rotation	;key
	mov bp, sp
	mov bl, [bp+2]	; (mov bl, key)
	cmp bl, 'n'	; key is 'n'
	jne Rotation2	; jump to the next 'if'
	push [word ptr dir]	; return dir
	jmp Rotation_middle_jump	;	return
	
Rotation2:
	cmp bl, 'r'	; the key is 'r'
	jne Rotation3	; jump to the next 'if'
	dec [dir]
	push[word ptr dir]
	push[6]	
	call Modulo	; return dir -1 % 6
	jmp Rotation_middle_jump	;	return
	
Rotation3:
	cmp bl, 'x'	; the key is 'x'
	jne Rotation4	; jump to the next 'if'
	dec [dir]
	dec [dir]
	push[word ptr dir]
	push[6]
	call Modulo	; return dir -2 % 6
	jmp Rotation_middle_jump	;	return

Rotation_middle_jump:	; cuz there is a limit to jump HEIGHT, countering that using a "check point".
	jmp Rotation_end
	
Rotation4:
	cmp bl, 'u'	; the key is 'u'
	jne Rotation5	; jump to the next 'if'
	dec [dir]
	dec [dir]
	dec [dir]
	push [word ptr dir]
	push[6]
	call Modulo	; return dir -3 % 6
	jmp Rotation_end	;	return
	
Rotation5:
	cmp bl, 'l'	; the key is 'l'
	jne Rotation6	; jump to the next 'if'
	inc [dir]
	push[word ptr dir]
	push[6]
	call Modulo	; return dir +1 % 6
	jmp Rotation_end	;	return
	
Rotation6:	
	cmp bl, 'y'	; the key is 'y'
	jne Rotation_end	; jump to the next 'if'
	inc [dir]
	inc [dir]
	push[word ptr dir]
	push[6]
	call Modulo	; return dir +2 % 6
	jmp Rotation_end	;	return
	
Rotation_end:
endp Rotation



; appends CL to the fake stack 'Malloc'
proc Fake_push basic far 
uses si, cl
	inc si	; next index (init must be: mov si, [malloc_index])
	cmp si, 120 ; the end of the stack
	jge Fake_push_Error ; there is an error (overflow)
	mov [Malloc+si], cl	; the inserting action
	mov [malloc_index], si
	
Fake_push_Error:
	
	ret
endp Fake_push

; pops from the fake stack 'Malloc'
proc Fake_pop basic far
uses si
	mov si, [malloc_index]
	mov [Malloc+si], 00 
	dec si
	mov [malloc_index], si
endp Fake_pop


; pushed the last allocated pointer to the stack 
proc get_pointer basic far
uses si
	mov si, [malloc_index]
	push [word ptr Malloc + si]
endp get_pointer


; translates an index into 2d vector for position
proc index_to_pos basic far
uses ax
	
	_% buffer_i, WIDTH	; buffer index % matrix width
	; saved in Malloc[index]
	lea mod ,[stack + sp]
	mov al, [byte ptr mod]
	dec al	; al -> index % WIDTH -1
	mov [buffer_x], al	; save buffer_x
	
	mov ax, [word ptr buffer_i]
	div [HEIGHT]
	mov [buffer_y], al; save buffer_y
	ret 1
endp index_to_pos


proc Modulo basic far
uses ax	; reset ax to its original value at a return
arg @@a:word, @@b:byte	; the arguments
	mov ax, [@@a]	
	div [@@b]	; divide

	xor al, al	; reset al, cuz the reminderis at ah
	mov cl, ah	; prep for pointer allocation
	call fake_push	; pointer allocation
	
	ret	; pop the arguments from the stack
endp Modulo

; translates a 2d position vector into a 1d index
proc pos_to_index basic far
uses ax
	mov ah, [buffer_y]
	mov al, [WIDTH]
	mul  ah	; ax = buffer_y * WIDTH
	add ax, [word ptr buffer_x]
	mov [word ptr buffer_i], ax ; save buffer_y * WIDTH + buffer_x
endp pos_to_index



proc dec_to_base basic far
uses ax, cx, bx
arg @@base:byte, @@num:word
	
	calloc 0h	; allocate a pointer for the result , default value be '0h'
	; lea result Malloc[index].... ;that deal
	
	calloc 0h ; allocate a pointer for the "power" variable , default value be '0h'
	; lea power Malloc[index].... ;that deal
	
dec_to_base_loop:	
	mov ax, [@@num]
	cmp ax, 00 ;compare num to zero
	jle dec_to_base_end ; if num <= 0 , then go to the end of the proc
	
	;_% [@@num], [@@base]	; num % base
	; lea mod Malloc[index].... ;that deal
	
	mov ax, [@@num]
	mov bl, [@@base]
	div bl		; num / base
	mov [byte ptr @@num], al	; num /= base
	
	mov al, [byte ptr power]
	mov bl, [digit]
	mul bl	; power * digit in ax
	mov cx, ax	; for comfort reasons
	mov ax, [result]	; so i could add to it
	add ax, cx	; result += power * digit
	
	mov al, [power]
	mov bl, 10
	mul bl	; power * 10
	mov [word ptr power], ax ; power *= 10
	
	jmp dec_to_base_loop	; loop that

dec_to_base_end:
	ret
endp dec_to_base


proc print basic far
uses dx
	mov ah, 09
	int 21h
endp print