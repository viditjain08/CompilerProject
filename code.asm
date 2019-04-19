SECTION	.bss

b3	resw 1
b2	resw 1
c2	resw 1
d2	resw 1
sinput	resb	16
SECTION	.data
negative	db	'-'
global	_start
write:
	mov		cx,ax
	mov		ax,0Ah
	push		ax
	mov		ax,cx
	mov		cx,1
	cmp		ax,0
	jge		Label1
	neg		ax
	push		cx
	push		ax
	mov		edx,1
	mov		ecx,negative
	mov		ebx,1
	mov		eax,4
	int		80h
	pop		ax
	pop		cx
Label1:
	inc		cx
	mov		si,10
	mov		dx,0h
	idiv		si
	add		dx,48
	push		dx
	cmp		ax,0
	jnz		Label1
Label2:
	mov		eax,esp
	push		cx
	mov		edx,1
	mov		ecx,eax
	mov		ebx,1
	mov		eax,4
	int		80h
	pop		cx
	dec		cx
	pop		ax
	cmp		cx,0
	jnz		Label2
	ret
read:
	mov		ax,0h
	mov		[sinput],ax
	mov		edx,16
	mov		ecx,sinput
	mov		ebx,0
	mov		eax,3
	int		80h
	mov		ax,0h
	push		ax
	mov		si,10
	mov		bl,[sinput]
	mov		ecx,sinput
	cmp		bl,[negative]
	jnz		Label6
	inc		ecx
	mov		bl,[ecx]
	pop		dx
	mov		dx,1h
	push		dx
Label6:
	sub		bl,30h
	mov		bh,0h
	mul		si
	add		ax,bx
	inc		ecx
	mov		bl,[ecx]
	cmp		bl,0ah
	jnz		Label6
	pop		dx
	cmp		dx,0
	je		Label7
	neg		ax
Label7:
	ret

_start:
call	read
mov	[b2],ax
mov	ax,20
push	ax
pop	ax
mov	[c2],ax
call	read
mov	[d2],ax
mov	ax,[b2]
push	ax
mov	ax,[c2]
push	ax
pop	bx
pop	ax
add	ax,bx
push	ax
mov	ax,[d2]
push	ax
pop	bx
pop	ax
add	ax,bx
push	ax
pop	ax
mov	[b3],ax
mov	ax,[b3]
call	write
mov	ebx,0
mov	eax,1
int	80h
,100
push	ax
pop	bx
pop	ax
add	ax,bx
push	ax
pop	ax
mov	[d6],ax
jmp	label3
label2:
mov	ax,[d6]
push	ax
mov	ax,200
push	ax
pop	bx
pop	ax
sub	ax,bx
push	ax
pop	ax
mov	[d6],ax
label3:
mov	ax,[d6]
call	write
mov	ebx,0
mov	eax,1
int	80h
