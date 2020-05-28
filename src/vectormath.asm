xdef _fxMul 
xdef _fxtoi 
xdef _sqrtInt 
xdef _normalize
xdef _fixedHLmulBC

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
	ld b,8 
loop2: 
	rla 
	jr nc,cont2
	add hl,de 
cont2:
	add hl,hl 
	djnz loop2
	
	ld a,c 
	ld b,7 

loop3: 
	rla 
	jr nc,cont3
	add hl,de 
cont3:
	add hl,hl 
	djnz loop3	
	
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
endfxmul:
	nop 
	
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
