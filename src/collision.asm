xdef _projectGroupAxis

xref __frameset
xref _fixedHLmulBC
xref _constraint
xref _group
xref _vert

sizeof_group equ 17
sizeof_constraint equ 12 
sizeof_vert equ 19 
sizeof_axis equ 9

g1 equ ix+9
g2 equ ix+12 

vptr equ ix-33

minc equ ix-30
minv equ ix-27
mindepth equ ix-24

tempc equ ix-21
tempv equ ix-18
tempdepth equ ix-15 

nx equ ix-12 
ny equ ix-9

v0x equ ix-6 
v0y equ ix-3 

_projectGroupAxis: 
	ld hl,-33
	call __frameset
	
	ld hl,-1048576
	ld (mindepth),hl
	
	; vptr
	ld hl,_group
	ld b,(g1) 
	ld c,sizeof_group
	mlt bc 
	add hl,bc 
	ld b,(hl) 
	
	ld iy,_vert 
	ld d,b 
	ld e,sizeof_vert
	mlt de 
	add iy,de
	ld (vptr),iy
	
	; c = vend - vstart
	inc hl 
	ld a,(hl)
	sub a,b
	ld c,a
	
	ld d,(g2) 
	ld hl,_group 
	ld e,sizeof_group 
	mlt de
	add hl,de 
	inc hl 
	inc hl 
	; b = cend - cstart
	ld d,(hl) 
	inc hl 
	ld a,(hl)
	sub a,d
	ld b,a
	; de = constraint pointer 
	ld e,sizeof_constraint
	mlt de 
	ld hl,_constraint 
	add hl,de 
	ex de,hl 

loop1: 
	ld (tempc),de
	ld iy,0 
	add iy,de 
	lea de,iy+sizeof_constraint
	ld a,c
	exx 
	
	; get normal 
	ld hl,(iy+9)
	ld (nx),hl 
	ld de,(iy+6) 
	or a,a 
	sbc hl,hl 
	sbc hl,de 
	ld (ny),hl
	
	; get v0 
	ld h,(iy+0) 
	ld l,sizeof_vert
	mlt hl 
	ld de,_vert 
	add hl,de 
	lea de,v0x 
	ld bc,6 
	ldir 
	
	ld b,a ; b = vend - vstart 
	ld iy,(vptr)
	
	;initialize temp 
	push bc
	ld hl,(iy+0) 
	ld de,(v0x) 
	or a,a 
	sbc hl,de 
	ld bc,(nx) 
	call _fixedHLmulBC
	push hl 
	ld hl,(iy+3) 
	ld de,(v0y) 
	or a,a 
	sbc hl,de 
	ld bc,(ny) 
	call _fixedHLmulBC
	pop de 
	add hl,de 
	ld (tempdepth),hl 
	ld (tempv),iy 
	lea iy,iy+sizeof_vert
	pop bc

loop2: 
	push bc
	ld hl,(iy+0) 
	ld de,(v0x) 
	or a,a 
	sbc hl,de 
	ld bc,(nx) 
	call _fixedHLmulBC
	push hl 
	ld hl,(iy+3) 
	ld de,(v0y) 
	or a,a 
	sbc hl,de 
	ld bc,(ny) 
	call _fixedHLmulBC
	pop de 
	add hl,de 
	
	; if tempdepth > hl, tempdepth = hl 
	ld de,(tempdepth)
	ex de,hl 
	or a,a 
	sbc hl,de 
	jp m,endloop2
	ld (tempdepth),de
	ld (tempv),iy 
	
endloop2: 
	pop bc 
	lea iy,iy+sizeof_vert
	djnz loop2
	
	;if tempdepth > 0 , return 
	bit 7,(tempdepth+2)
	jr nz,cmpTempMin 
	
	lea hl,tempc
	ld de,(ix+6) 
	ld bc,sizeof_axis
	ldir 
	ld sp,ix 
	pop ix 
	ret 
	
	; if tempdepth > mindepth, min = temp
cmpTempMin: 
	ld hl,(tempdepth) 
	ld de,(mindepth) 
	or a,a 
	sbc hl,de 
	jp m,endloop1
	lea hl,tempc
	lea de,minc
	ld bc,sizeof_axis 
	ldir 
endloop1: 
	exx 
	dec b
	jp m,return
	jp nz,loop1
	
return: 	
	lea hl,minc
	ld de,(ix+6)
	ld bc,sizeof_axis 
	ldir
	
	ld sp,ix 
	pop ix 
	ret 
