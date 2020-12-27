public _projectGroupAxis

extern __frameset
extern _fixedHLmulBC
extern _constraint
extern _group
extern _vert

sizeof_group equ 17
sizeof_constraint equ 12 
sizeof_vert equ 13 
sizeof_axis equ 9

g1 equ ix+6
g2 equ ix+9 

vptr equ ix-33

minc equ ix-30
minv equ ix-27
mindepth equ ix-24

tminc equ ix-21
tminv equ ix-18
tmindepth equ ix-15 

nx equ ix-12 
ny equ ix-9

v0x equ ix-6 
v0y equ ix-3 

_projectGroupAxis: 
	ld hl,-33
	call __frameset
		
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

	ld hl,-1048576
	ld (mindepth),hl

	
loop1: 
	ld (tminc),de
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
	
	ld (tmindepth),hl 
	ld (tminv),iy 
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
	ex de,hl
	ld hl,(tmindepth)
	or a,a 
	sbc hl,de
	jp m,endloop2
	ld (tmindepth),de
	ld (tminv),iy 
endloop2: 
	pop bc 
	lea iy,iy+sizeof_vert
	djnz loop2
	
	;if tempdepth > 0 , return 
	ld de,(tmindepth)
	or a,a 
	sbc hl,hl 
	sbc hl,de
	jr z,cmpTempMin
	jp p,cmpTempMin
earlyRet: 	
	ld sp,ix 
	pop ix 
	ld sp,ix 
	pop ix
	ret 
	
	; if tempdepth > mindepth, min = temp
cmpTempMin:
	ex de,hl 
	ld bc,(mindepth) 
	or a,a 
	sbc hl,hl 
	sbc hl,bc 
	or a,a 
	sbc hl,de
	jr c,endloop1
	lea hl,tminc
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
	ld sp,ix 
	pop ix 
	ret 