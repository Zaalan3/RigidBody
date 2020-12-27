public _solveConstraints

extern _constraint 
extern _vert 

extern _fixedHLmulBC
extern _normalize
extern Sqrt24

sizeof_constraint equ 12
sizeof_vert equ 13

v0 equ ix+0
v1 equ ix+1
len equ ix+2 
k equ ix+5 
dirx equ ix+6 
diry equ ix+9
 
_solveConstraints: 
	push ix 
	ld ix,_constraint
loop: 
	ld iy,_vert
	ld e,(v0)
	ld d,sizeof_vert
	mlt de 
	add iy,de 
	
	ld d,sizeof_vert
	ld e,(v1) 
	mlt de 
	lea bc,ix+0 ; bc = constraint pointer
	ld ix,_vert
	add ix,de
	
	ld hl,(ix+3) 
	ld de,(iy+3) 
	or a,a 
	sbc hl,de 
	push hl
	
	ld hl,(ix+0) 
	ld de,(iy+0) 
	or a,a 
	sbc hl,de 
	
	push bc 	;swap bc and ix
	lea bc,ix+0  
	pop ix 
	
	ld (dirx),hl ;dirx = v1.x - v0.x 
	pop hl 
	ld (diry),hl ;diry = v1.y - v0.y  
	
	push iy 
	push bc 
	
	lea iy,dirx 
	
normalize: 	
	ld bc,(iy+0) 
	or a,a 
	sbc hl,hl 
	add hl,bc 
	call _fixedHLmulBC 
	push hl 
	ld bc,(iy+3) 
	or a,a 
	sbc hl,hl 
	add hl,bc
	call _fixedHLmulBC 
	pop de 
	add hl,de 
	push iy 
	call Sqrt24
	add hl,hl
	add hl,hl
	add hl,hl
	add hl,hl
	pop iy 	
	
	push hl 
	push hl 
	pop bc 
	ld de,65536 
DivideDEBC:
	xor	a,a
	sbc	hl,hl
	sbc	hl,bc
	jp	p,nnext0
	push	hl
	pop	bc
	inc	a
nnext0:
	or	a,a
	sbc	hl,hl
	sbc	hl,de
	jp	m,nnext1
	ex	de,hl
	inc	a
nnext1:
	add	hl,de
	rra
	ld	a,24
nloop:
	ex	de,hl
	adc	hl,hl
	ex	de,hl
	adc	hl,hl
	add	hl,bc
	jr	c,nspill
	sbc	hl,bc
nspill:
	dec	a
	jr	nz,nloop

	ex	de,hl
	adc	hl,hl
	jr	c,enddiv
	ex	de,hl
	sbc	hl,hl
	sbc	hl,de

enddiv: 	
	push hl 
	ld	bc,(iy+0) 
	call _fixedHLmulBC
	ld (iy+0),hl 
	pop hl 
	ld bc,(iy+3) 
	call _fixedHLmulBC
	ld (iy+3),hl 
	pop hl 
	
	ld de,(len) 
	or a,a 
	sbc hl,de ; hl = length - clength
	ld bc,0 
	ld c,(k) 
	call _fixedHLmulBC
	
	ld bc,(dirx) 
	push hl 
	call _fixedHLmulBC
	pop bc 
	push hl 
	ld hl,(diry) 
	call _fixedHLmulBC
	ex de,hl ; de = dlen*diry
	pop bc ; bc = dlen*dirx 
	
	exx 
	lea hl,ix+sizeof_constraint
	exx 
	pop ix 
	pop iy 
	
	ld a,(ix+12)
	or a,a 
	jr z,next
	
	ld hl,(ix+0)
	or a,a 
	sbc hl,bc 
	ld (ix+0),hl 
	
	ld hl,(ix+3)
	or a,a 
	sbc hl,de 
	ld (ix+3),hl 
	
next: 
	ld a,(iy+12) 
	or a,a
	jr z,loopend 
	
	ld hl,(iy+0) 
	add hl,bc 
	ld (iy+0),hl 
	
	ld hl,(iy+3) 
	add hl,de 
	ld (iy+3),hl
	
loopend: 
	exx 
	push hl 
	pop ix 
	ld a,(k)
	or a,a 
	jp nz,loop 
	
breakloop: 
	pop ix 
	ret 
