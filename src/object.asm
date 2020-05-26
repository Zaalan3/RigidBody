xdef _solveConstraints

xref _constraint 
xref _vert 

xref _fixedHLmulBC
xref _normalize

sizeof_constraint equ 12
sizeof_vert equ 19

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
	ld hl,_vert
	ld e,(v0)
	ld d,sizeof_vert
	mlt de 
	add hl,de 
	push hl 
	pop iy ; iy = &vert[v0] 
	
	or a,a 
	sbc hl,de 
	ld d,sizeof_vert
	ld e,(v1) 
	mlt de 
	add hl,de 
	lea bc,ix+0 ; bc = constraint pointer
	push hl 
	pop ix 	; ix = &vert[v1] 
	
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
	
	pea dirx 
	call _normalize 
	pop de  
	
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
	
	ld a,(ix+18)
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
	ld a,(iy+18) 
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
	
	
	
	
	
	