
.import callptr4
.importzp tmp4

.define BANK_SELECT 	$8000
.define BANK_DATA		$8001
.define A12_INVERT		%10000000
.define BANK_DPCM_FLAG	%01000000

.if !.defined(MMC3_BANK_FLAGS)
	.if .defined(USE_BANKABLE_DPCM) .and .defined(USE_CHR_A12_INVERT)
		MMC3_BANK_FLAGS = BANK_DPCM_FLAG | A12_INVERT
	.elseif .defined(USE_BANKABLE_DPCM)
		MMC3_BANK_FLAGS = BANK_DPCM_FLAG
	.elseif .defined(USE_CHR_A12_INVERT)
		MMC3_BANK_FLAGS = A12_INVERT
	.else
		MMC3_BANK_FLAGS = 0
	.endif
.endif

;values 0 or $80, for the $8000 register
;(changes in tileset mapping)

;if invert bit is 0 
;mode 0 changes $0000-$07FF
;mode 1 changes $0800-$0FFF
;mode 2 changes $1000-$13FF
;mode 3 changes $1400-$17FF
;mode 4 changes $1800-$1BFF
;mode 5 changes $1C00-$1FFF

;if invert bit is $80
;mode 0 changes $1000-$17FF
;mode 1 changes $1800-$1FFF
;mode 2 changes $0000-$03FF
;mode 3 changes $0400-$07FF
;mode 4 changes $0800-$0BFF
;mode 5 changes $0C00-$0FFF

.pushseg
.segment "ZEROPAGE"
mmc3_ptr:		.res 2 ; array for the irq parser
mmc3_index:		.res 1 ; index to this array
_irq_done:		.res 1

; A bank shadow stores the most recent value of the write to the MMC3 $8000 register. When a thread is interruptable,
; then it MUST save the value of the select before writing it to the register. If a thread is the one that does the interrupting
; (ie NMI or IRQ) then it should NOT save the value, but should restore the value to the MMC3 $8000 register before returning.
; Because the IRQ is not able to interrupt the NMI thread under the current configuration, we only need one shadow. If this is changed
; so that the scanline IRQ can run, then you need a second shadow, and NMI should save the current select value to the second shadow
; and restore the value of the first shadow to $8000 on exit.
bank_shadow: 	.res 1
mmc3_bank_6:	.res 1
mmc3_bank_7:	.res 1

.segment "BSS"
mmc3_temp:		.res 1

.segment "LOWCODE"

;needs to be mapped to the fixed bank
.export _set_mirroring, _set_wram_mode, _disable_irq
.export _set_irq_ptr

; Rename the exports to match what the C header defines them as.
.export _set_prg_a000:=main_thread_set_prg_7
.export _current_prg_a000:=mmc3_bank_7

.ifdef USE_BANKABLE_DPCM
	CODE_BANK_SELECT = (7 | MMC3_BANK_FLAGS)
	ALT_BANK_SELECT = (6 | MMC3_BANK_FLAGS)
	code_bank_fn = main_thread_set_prg_7
	alt_bank_fn = main_thread_set_prg_6
	.export _bank_code:=main_thread_set_prg_7

	.exportzp _current_code_bank:=mmc3_bank_7
	.exportzp _current_alt_bank:=mmc3_bank_6

	.export _set_prg_c000:=main_thread_set_prg_6
.else
	; if we aren't banking DPCM then we export functions for banking $8000
	CODE_BANK_SELECT = (6 | MMC3_BANK_FLAGS)
	ALT_BANK_SELECT = (7 | MMC3_BANK_FLAGS)
	code_bank_fn = main_thread_set_prg_6
	alt_bank_fn = main_thread_set_prg_7
	.export _bank_code:=main_thread_set_prg_6

	.exportzp _current_code_bank:=mmc3_bank_6
	.exportzp _current_alt_bank:=mmc3_bank_7

	.export _set_prg_8000:=main_thread_set_prg_6
.endif

.exportzp CODE_BANK_SELECT, ALT_BANK_SELECT

; Any banking routines that begin with `main_thread` should NOT be used in NMI or IRQ.

; sets the bank at $8000-9fff or $c000-$dfff if USE_BANKABLE_DPCM is enabled
; only changes the A register
main_thread_set_prg_6:
	sta mmc3_bank_6
	lda #(6 | MMC3_BANK_FLAGS)
	sta bank_shadow
	sta BANK_SELECT
	lda mmc3_bank_6
	sta BANK_DATA
	rts

; sets the bank at $a000-bfff
main_thread_set_prg_7:
	sta mmc3_bank_7
	lda #(7 | MMC3_BANK_FLAGS)
	sta bank_shadow
	sta BANK_SELECT
	lda mmc3_bank_7
	sta BANK_DATA
	rts

; rexport the following function for C use
.export _bank_trampoline:=main_thread_bank_trampoline
; Use with cc65 wrapped-call pragmas to automatically switch banks when calling different methods
main_thread_bank_trampoline:
	; A needs to be preserved and passed to the function in ptr4 that we are trampolining to.
	; we don't touch x or y so nothing to preserve there.
	; IN - tmp4 - bank to switch to
	; IN - ptr4 - function to call
	sta mmc3_temp ; store the current value in A so we can pass it to the banked function later
	; First we check to see if the bank we are switching to is the same already.
	lda _current_code_bank
	cmp tmp4
	bne @do_bank_switch
		; since they are the same, we can skip the banking and just run the function and return
		lda mmc3_temp
		jmp callptr4
@do_bank_switch:
	; save the current bank value to the stack so we can restore it later
	pha
		lda tmp4
		jsr code_bank_fn
		lda mmc3_temp
		jsr callptr4
	pla
	jmp code_bank_fn

; The following are mainthread safe CHR banking routines
.export _set_chr_mode_0:=main_thread_set_chr_0
.export _set_chr_mode_1:=main_thread_set_chr_1
.export _set_chr_mode_2:=main_thread_set_chr_2
.export _set_chr_mode_3:=main_thread_set_chr_3
.export _set_chr_mode_4:=main_thread_set_chr_4
.export _set_chr_mode_5:=main_thread_set_chr_5

main_thread_set_chr_0:
	pha
		lda #(0 | MMC3_BANK_FLAGS)
		sta bank_shadow
		sta BANK_SELECT
	pla
	sta BANK_DATA
	rts

main_thread_set_chr_1:
	pha
		lda #(1 | MMC3_BANK_FLAGS)
		sta bank_shadow
		sta BANK_SELECT
	pla
	sta BANK_DATA
	rts

main_thread_set_chr_2:
	pha
		lda #(2 | MMC3_BANK_FLAGS)
		sta bank_shadow
		sta BANK_SELECT
	pla
	sta BANK_DATA
	rts

main_thread_set_chr_3:
	pha
		lda #(3 | MMC3_BANK_FLAGS)
		sta bank_shadow
		sta BANK_SELECT
	pla
	sta BANK_DATA
	rts

main_thread_set_chr_4:
	pha
		lda #(4 | MMC3_BANK_FLAGS)
		sta bank_shadow
		sta BANK_SELECT
	pla
	sta BANK_DATA
	rts

main_thread_set_chr_5:
	pha
		lda #(5 | MMC3_BANK_FLAGS)
		sta bank_shadow
		sta BANK_SELECT
	pla
	sta BANK_DATA
	rts

; MIRROR_VERTICAL 0
; MIRROR_HORIZONTAL 1	
_set_mirroring:
	sta $a000
	rts

; WRAM_OFF $40
; WRAM_ON $80
; WRAM_READ_ONLY $C0
_set_wram_mode:
	sta $a001
	rts

_disable_irq:
	sta $e000 ;any value
	lda #<default_array
	ldx #>default_array
	;jmp _set_irq_ptr ; fall through
	
_set_irq_ptr:
;ax = pointer
	sta mmc3_ptr
	stx mmc3_ptr+1
	rts

default_array: ;just an eof terminator
.byte $ff


irq:
	pha
	txa
	pha
	tya
	pha
	
	sta $e000	; disable mmc3 irq
				; any value will do
	
	jsr irq_parser
	
	pla
	tay
	pla
	tax
	pla
	rti
	
	
;format
;value < 0xf0, it's a scanline count
;zero is valid, it triggers an IRQ at the end of the current line

;if >= 0xf0...
;f0 = 2000 write, next byte is write value
;f1 = 2001 write, next byte is write value
;f2-f4 unused - future TODO ?
;f5 = 2005 write, next byte is H Scroll value
;f6 = 2006 write, next 2 bytes are write values


;f7 = change CHR mode 0, next byte is write value
;f8 = change CHR mode 1, next byte is write value
;f9 = change CHR mode 2, next byte is write value
;fa = change CHR mode 3, next byte is write value
;fb = change CHR mode 4, next byte is write value
;fc = change CHR mode 5, next byte is write value

;fd = very short wait, no following byte 
;fe = short wait, next byte is quick loop value
;(for fine tuning timing of things)

;ff = end of data set

	
irq_parser:
	ldy mmc3_index
;	ldx #0
@loop:
	lda (mmc3_ptr), y ; get value from array
	iny
	cmp #$fd ;very short wait
	beq @loop
	
	cmp #$fe ;fe-ff wait or exit
	bcs @wait
	
	cmp #$f0
	bcs @1
	jmp @scanline ;below f0
@1:	
	
	cmp #$f7
	bcs @chr_change
;f0-f6	
	tax
	lda (mmc3_ptr), y ; get value from array
	iny
	cpx #$f0
	bne @2
	sta $2000 ; f0
	jmp @loop
@2:
	cpx #$f1
	bne @3
	sta $2001 ; f1
	jmp @loop
@3:
	cpx #$f5 
	bne @4
	ldx #4
@better_timing: ; don't change till near the end of the line
	dex
	bne @better_timing
	
	sta $2005 ; f5
	sta $2005 ; second value doesn't matter
	jmp @loop
@4:
	sta $2006 ; f6
	lda (mmc3_ptr), y ; get 2nd value from array
	iny	
	sta $2006
	jmp @loop
	
@wait: ; fe-ff wait or exit
	cmp #$ff
	beq @exit	
	lda (mmc3_ptr), y ; get value from array
	iny
	tax
	beq @loop ; if zero, just exit
@wait_loop: ; the timing of this wait could change if this crosses a page boundary
	dex
	bne @wait_loop		
	beq @loop ; unconditional

@chr_change:
;f7-fc change a CHR set
	sec
	sbc #$f7 ;should result in 0-5
	ora #MMC3_BANK_FLAGS
	sta BANK_SELECT
	lda (mmc3_ptr), y ; get next value
	sta BANK_DATA
	iny
	jmp @loop
	
@scanline:
	nop ;trying to improve stability
	nop
	nop
	nop
	jsr set_scanline_count ;this terminates the set
	sty mmc3_index
	; before exiting the IRQ we must restore the previous value of the BANK_SELECT to prevent issues!
	lda bank_shadow
	sta BANK_SELECT
	rts
	
@exit:
	sta _irq_done ;value 0xff
	dey ; undo the previous iny, keep it pointed to ff
	sty mmc3_index
	; before exiting the IRQ we must restore the previous value of the BANK_SELECT to prevent issues!
	lda bank_shadow
	sta BANK_SELECT
	rts
	
	
set_scanline_count:
; any value will do for most of these registers
	sta $e000 ; disable mmc3 irq
	sta $c000 ; set the irq counter reload value
	sta $c001 ; reload the reload value
	sta $e001 ; enable the mmc3 irq
    cli ;make sure irqs are enabled
	rts

.popseg ; "LOWCODE"
	