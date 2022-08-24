; Startup code for cc65 and Shiru's NES library
; based on code by Groepaz/Hitmen <groepaz@gmx.net>, Ullrich von Bassewitz <uz@cc65.org>


.include "MMC3/mmc3_code.asm"

;REMOVED initlib
;this called the CONDES function

.export _exit,__STARTUP__:absolute=1
.import push0,popa,popax,_main,zerobss,copydata

; Linker generated symbols
.import __STACK_START__   ,__STACKSIZE__ ;changed
.import __ROM0_START__  ,__ROM0_SIZE__
.import __STARTUP_LOAD__,__STARTUP_RUN__,__STARTUP_SIZE__
.import	__CODE_LOAD__   ,__CODE_RUN__   ,__CODE_SIZE__
.import	__RODATA_LOAD__ ,__RODATA_RUN__ ,__RODATA_SIZE__
.import NES_MAPPER, NES_PRG_BANKS, NES_CHR_BANKS, NES_MIRRORING

.include "zeropage.inc"


PPU_CTRL	=$2000
PPU_MASK	=$2001
PPU_STATUS	=$2002
PPU_OAM_ADDR=$2003
PPU_OAM_DATA=$2004
PPU_SCROLL	=$2005
PPU_ADDR	=$2006
PPU_DATA	=$2007
PPU_OAM_DMA	=$4014
PPU_FRAMECNT=$4017
DMC_FREQ	=$4010
CTRL_PORT1	=$4016
CTRL_PORT2	=$4017

OAM_BUF		=$0200
PAL_BUF		=$01c0
VRAM_BUF	=$0700



.segment "ZEROPAGE"

NTSC_MODE: 			.res 1
FRAME_CNT1: 		.res 1
FRAME_CNT2: 		.res 1
VRAM_UPDATE: 		.res 1
NAME_UPD_ADR: 		.res 2
NAME_UPD_ENABLE: 	.res 1
PAL_UPDATE: 		.res 1
PAL_BG_PTR: 		.res 2
PAL_SPR_PTR: 		.res 2
SCROLL_X: 			.res 1
SCROLL_Y: 			.res 1
SCROLL_X1: 			.res 1
SCROLL_Y1: 			.res 1
PAD_STATE: 			.res 2		;one byte per controller
PAD_STATEP: 		.res 2
PAD_STATET: 		.res 2
PPU_CTRL_VAR: 		.res 1
PPU_CTRL_VAR1: 		.res 1
PPU_MASK_VAR: 		.res 1
RAND_SEED: 			.res 2
FT_TEMP: 			.res 3

TEMP: 				.res 11
SPRID:				.res 1

PAD_BUF		=TEMP+1

PTR			=TEMP	;word
LEN			=TEMP+2	;word
NEXTSPR		=TEMP+4
SCRX		=TEMP+5
SCRY		=TEMP+6
SRC			=TEMP+7	;word
DST			=TEMP+9	;word

RLE_LOW		=TEMP
RLE_HIGH	=TEMP+1
RLE_TAG		=TEMP+2
RLE_BYTE	=TEMP+3

;nesdoug code requires
VRAM_INDEX:			.res 1
META_PTR:			.res 2
DATA_PTR:			.res 2





.segment "HEADER"

    .byte $4e,$45,$53,$1a
	.byte <NES_PRG_BANKS
	.byte <NES_CHR_BANKS
	.byte <NES_MIRRORING|(<NES_MAPPER<<4)
	.byte <NES_MAPPER&$f0
	.byte 1 ;8k of PRG RAM
	.res 7,0

.segment "LOWCODE"
	.include "LIB/neslib.s"
	.include "LIB/nesdoug.s"

.segment "STARTUP"

; Maps the CHR bank select to the CHR bank value
.ifdef USE_CHR_A12_INVERT
BankInitializationTable:
	.byte $04, $06, $00, $01, $02, $03
.else
BankInitializationTable:
	.byte $00, $01, $02, $03, $04, $06
.endif

; this should be mapped to the last PRG bank

start:
_exit:

    sei
	cld
	ldx #$40
	stx CTRL_PORT2
    ldx #$ff
    txs
    inx
    stx PPU_MASK
    stx DMC_FREQ
    stx PPU_CTRL		;no NMI
	
	jsr _disable_irq ;disable mmc3 IRQ
	
	;x is zero

initPPU:
    bit PPU_STATUS
@1:
    bit PPU_STATUS
    bpl @1
@2:
    bit PPU_STATUS
    bpl @2

clearPalette:
	lda #$3f
	sta PPU_ADDR
	stx PPU_ADDR
	lda #$0f
	ldx #$20
@1:
	sta PPU_DATA
	dex
	bne @1
	
	
	lda #$01				; DEBUGGING
	sta PPU_DATA
	lda #$11
	sta PPU_DATA
	lda #$21
	sta PPU_DATA
	lda #$30
	sta PPU_DATA

clearVRAM:
	txa
	ldy #$20
	sty PPU_ADDR
	sta PPU_ADDR
	ldy #$10
@1:
	sta PPU_DATA
	inx
	bne @1
	dey
	bne @1

clearRAM:
    txa
@1:
    sta $000,x
    sta $100,x
    sta $200,x
    sta $300,x
    sta $400,x
    sta $500,x
    sta $600,x
    sta $700,x
    inx
    bne @1
	
; don't call any subroutines until the banks are in place	
	
; MMC3 configure the PRG and CHR banks. We don't need to do any special shadow management because this is in reset
.ifdef USE_BANKABLE_DPCM
	; If DPCM is bankable, then we need to put the main code in $a000
	lda #7
	sta BANK_SELECT
	lda #0
	sta mmc3_bank_7
	sta BANK_DATA
	lda #6
	sta BANK_SELECT
	lda #13
	sta mmc3_bank_6
	sta BANK_DATA
.else
	; otherwise we need to put the main code in $8000
	lda #6
	sta BANK_SELECT
	lda #0
	sta mmc3_bank_6
	sta BANK_DATA
	lda #7
	sta BANK_SELECT
	lda #13
	sta mmc3_bank_7
	sta BANK_DATA
.endif

; Initialize the CHR banks differently depending on if A12 inversion is enabled.
	ldx #5
@chr_init_loop:
		stx BANK_SELECT
		lda BankInitializationTable, x
		sta BANK_DATA
		dex
		bpl @chr_init_loop

;set mirroring to vertical, no good reason	
	lda #0
	jsr _set_mirroring
;allow reads and writes to WRAM	
	lda #$80 ;WRAM_ON 0x80
	jsr _set_wram_mode
	
	cli ;allow irq's to happen on the 6502 chip	
		;however, the mmc3 IRQ was disabled above
	
	

	lda #4
	jsr _pal_bright
	jsr _pal_clear
	jsr _oam_clear

    jsr zerobss
	jsr	copydata

    lda #<(__STACK_START__+__STACKSIZE__) ;changed
    sta	sp
    lda	#>(__STACK_START__+__STACKSIZE__)
    sta	sp+1            ; Set argument stack ptr

;	jsr	initlib
; removed. this called the CONDES function

	lda #%10000000
	sta <PPU_CTRL_VAR
	sta PPU_CTRL		;enable NMI
	lda #%00000110
	sta <PPU_MASK_VAR

waitSync3:
	lda <FRAME_CNT1
@1:
	cmp <FRAME_CNT1
	beq @1

detectNTSC:
	ldx #52				;blargg's code
	ldy #24
@1:
	dex
	bne @1
	dey
	bne @1

	lda PPU_STATUS
	and #$80
	sta <NTSC_MODE

	jsr _ppu_off

	lda #0
	ldx #0
	jsr _set_vram_update

	lda #$fd
	sta <RAND_SEED
	sta <RAND_SEED+1

	lda #0
	sta PPU_SCROLL
	sta PPU_SCROLL
	
 	;swap the music in place before using
	lda #CODE_BANK_SELECT
	sta BANK_SELECT
	lda #<.bank(music_init)
	sta BANK_DATA
.if .defined(USE_BANKABLE_DPCM) && .defined(MUSIC_HAS_DPCM)
	; switch the $c000 address to the banked DPCM
	lda #(6 | MMC3_BANK_FLAGS)
	sta BANK_SELECT
	lda #<.bank(samples)
	sta BANK_DATA
.endif
	ldx #<music_data
	ldy #>music_data
	lda <NTSC_MODE
	jsr music_init
.ifdef MUSIC_HAS_SFX
	ldx #<sounds_data
	ldy #>sounds_data
	jsr sfx_init
.endif
	jmp _main			;no parameters

.segment "VECTORS"

    .word nmi	;$fffa vblank nmi
    .word start	;$fffc reset
   	.word irq	;$fffe irq / brk


.segment "CHARS"
	 ; SPRITES
     ;mode 0 changes $1000-$17FF
     ;mode 1 changes $1800-$1FFF
     
	 ; BACKGROUND
     ;mode 2 changes $0000-$03FF
     ;mode 3 changes $0400-$07FF
     ;mode 4 changes $0800-$0BFF
     ;mode 5 changes $0C00-$0FFF

	; MAX size is 256kb
	.incbin "I-CHR/alpha-top.pngE/chrblock.chr" ; 2kb Alpha and common tiles  0x00  -  0x01
	.incbin "I-CHR/altar.pngE/chrblock.chr"		; 1kb Altar    0x02
	.incbin "I-CHR/cemetery.pngE/chrblock.chr"	; 1kb Graveyard    0x03

	.incbin "I-CHR/sprites01.pngE/chrblock-top1.chr"; 2kb Main TOP sprites   0x04  -  0x05
	.incbin "I-CHR/sprites01.pngE/chrblock-bottom1.chr"; 2kb Faces 0x06  -  0x07
	
	.incbin "I-CHR/map.pngE/chrblock.chr"		; 2kb Map   0x08  -  0x09
	.incbin "I-CHR/church-interior.pngE/chrblock-1k.chr" ; 1kb Temple - Town1 0x0A
	.incbin "I-CHR/town-ruins.pngE/chrblock.chr" ; 1kb Town2 0x0B
	.incbin "I-CHR/sprites01.pngE/chrblock-bottom2-staff.chr"; 2kb Staff - Lightning 0x0C - 0x0D
	.incbin "I-CHR/title_screen.pngE/title_screen.chr" ; 4kb Title Screen!!! 0x0E - 0x0F
