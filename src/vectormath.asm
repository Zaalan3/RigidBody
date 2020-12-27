public _fxMul 
public _fxDiv
public _fxtoi 
public _sqrtInt 
public _normalize
public _fixedHLmulBC
public _fixedHLdivBC
public Sqrt24

_fxtoi: 
	pop bc 
	inc sp 
	pop hl 
	dec sp 
	push hl 
	push bc 
	ld a,h 
	ex de,hl 
	rlca 
	sbc hl,hl 
	ld h,d 
	ld l,e 
	ret 
	
_fxMul: 
	pop de 
	pop hl 
	pop bc 
	push de 
	push de 
	push de 
_fixedHLmulBC:
	ld a,0B7h
	ex de,hl 
	or a,a 
	sbc hl,hl 
	sbc hl,de 
	jp m,noswapHLDE
	ex de,hl 
	xor a,080h
noswapHLDE: 
	or a,a 
	sbc hl,hl 
	sbc hl,bc 
	jp m,noswapHLBC
	push hl 
	pop bc
    xor a,080h
noswapHLBC:  
	ld (MultSMC),a
	or a,a 
	sbc hl,hl 

	ld a,b 
	
	rla 
	jr nc,cont1
	add hl,de 
cont1:
	add hl,hl
	
	rla 
	jr nc,cont2
	add hl,de 
cont2:
	add hl,hl
	
	rla 
	jr nc,cont3
	add hl,de 
cont3:
	add hl,hl
	
	rla 
	jr nc,cont4
	add hl,de 
cont4:
	add hl,hl
	
	rla 
	jr nc,cont5
	add hl,de 
cont5:
	add hl,hl
	
	rla 
	jr nc,cont6
	add hl,de 
cont6:
	add hl,hl
	
	rla 
	jr nc,cont7
	add hl,de 
cont7:
	add hl,hl
	
	rla 
	jr nc,cont8
	add hl,de 
cont8:
	add hl,hl
	
	ld a,c 

	rla 
	jr nc,cont9
	add hl,de 
cont9:
	add hl,hl
	
	rla 
	jr nc,cont10
	add hl,de 
cont10:
	add hl,hl
	
	rla 
	jr nc,cont11
	add hl,de 
cont11:
	add hl,hl
	
	rla 
	jr nc,cont12
	add hl,de 
cont12:
	add hl,hl
	
	rla 
	jr nc,cont13
	add hl,de 
cont13:
	add hl,hl
	
	rla 
	jr nc,cont14
	add hl,de 
cont14:
	add hl,hl
	
	rla 
	jr nc,cont15
	add hl,de 
cont15:
	add hl,hl
	
	rla 
	jr nc,shiftDown 
	add hl,de 
	
shiftDown: 
	push af 
	inc sp 
	push hl 
	inc sp 
	pop hl 
	inc sp 
MultSMC: or a,a 
	ret nc 
	ex de,hl 
	or a,a 
	sbc hl,hl 
	sbc hl,de 
	ret
	
	
_fxDiv:
	pop de 
	pop hl 
	pop bc 
	push de
	push de
	push de
_fixedHLdivBC: 
	add hl,hl
	add hl,hl
	add hl,hl
	add hl,hl
	add hl,hl
	add hl,hl
	add hl,hl
	add hl,hl
	ex	de,hl
	xor	a,a
	sbc	hl,hl
	sbc	hl,bc
	jp	p,next0
	push	hl
	pop	bc
	inc	a
next0:
	or	a,a
	sbc	hl,hl
	sbc	hl,de
	jp	m,next1
	ex	de,hl
	inc	a
next1:
	add	hl,de
	rra
	ld	a,24
loop:
	ex	de,hl
	adc	hl,hl
	ex	de,hl
	adc	hl,hl
	add	hl,bc
	jr	c,spill
	sbc	hl,bc
spill:
	dec	a
	jr	nz,loop

	ex	de,hl
	adc	hl,hl
	ret	c
	ex	de,hl
	sbc	hl,hl
	sbc	hl,de
	ret

	
;https://www.cemetech.net/forum/viewtopic.php?p=253204
_sqrtInt:
		pop de 
		pop hl 
		push hl
		push de
; uhl = sqrt(uhl)
Sqrt24:
        dec     sp      ; (sp) = ?
        push    hl      ; (sp) = ?uhl
        dec     sp      ; (sp) = ?uhl?
        pop     iy      ; (sp) = ?u, uiy = hl?
        dec     sp      ; (sp) = ?u?
        pop     af      ; af = u?
        or      a,a
        sbc     hl,hl
        ex      de,hl   ; de = 0
        sbc     hl,hl   ; hl = 0
        ld      bc,0C40h ; b = 12, c = 0x40
Sqrt24Loop:
        sub     a,c
        sbc     hl,de
        jr      nc,Sqrt24Skip
        add     a,c
        adc     hl,de
Sqrt24Skip:
        ccf
        rl      e
        rl      d
        add     iy,iy
        rla
        adc     hl,hl
        add     iy,iy
        rla
        adc     hl,hl
        djnz    Sqrt24Loop
        ex      de,hl
        ret
		
	
_normalize: 
	pop de 
	pop iy 
	push de
	push de
	
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
	ret
