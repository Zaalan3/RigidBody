
xdef _fxMul 
xdef _fxtoi 
xdef _sqrtInt 
;xdef _normalize

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
fixedHLmulBC:
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
        pop     iy      ; (sp) = ?u, uix = hl?
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
		