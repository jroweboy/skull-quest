
.pushseg
.segment "LOWCODE"

; Common music defines for the different sound engines
NO_MUSIC_ENGINE = 0
FAMITONE2 = 1
FAMISTUDIO = 2

.ifdef USE_FAMITONE2

MUSIC_ENABLED = 1

FT_BASE_ADR		= $0100		;page in RAM, should be $xx00
.ifdef USE_BANKABLE_DPCM
FT_DPCM_OFF     = $c000
.else
FT_DPCM_OFF		= $f000		;$c000..$ffc0, 64-byte steps
.endif
FT_SFX_STREAMS	= 1			;number of sound effects played at once, 1..4

FT_THREAD       = 1		;undefine if you call sound effects in the same thread as sound update
FT_PAL_SUPPORT	= 1		;undefine to exclude PAL support
FT_NTSC_SUPPORT	= 1		;undefine to exclude NTSC support
FT_DPCM_ENABLE  = 1		;undefine to exclude all DMC code
FT_SFX_ENABLE   = 1		;undefine to exclude all sound effects code

.export _set_music_speed
;void __fastcall__ set_music_speed(unsigned char tempo);
_set_music_speed:
	sta FT_SONG_SPEED
	rts


.segment "AUDIO"

.include "../MUSIC/famitone2.s"

.export music_init:=FamiToneInit
.export music_update:=FamiToneUpdate
.export sfx_init:=FamiToneSfxInit

;void __fastcall__ music_play(unsigned char song);
;a = song #
.export _music_play:= FamiToneMusicPlay


;void __fastcall__ music_stop(void);
.export _music_stop:=FamiToneMusicStop

;void __fastcall__ music_pause(unsigned char pause);
;a = pause or not
.export _music_pause:=FamiToneMusicPause

;void __fastcall__ sfx_play(unsigned char sound,unsigned char channel);
.export _sfx_play
_sfx_play:
.if(FT_SFX_ENABLE)
.define MUSIC_HAS_SFX 1
; a = channel
	and #$03
	tax
	lda @sfxPriority,x
	tax
	jsr popa ;a = sound
	;x = channel offset
	jmp FamiToneSfxPlay
	;rts

@sfxPriority:
	.byte FT_SFX_CH0,FT_SFX_CH1,FT_SFX_CH2,FT_SFX_CH3
.else
	rts
.endif


;void __fastcall__ sample_play(unsigned char sample);
;a = sample #
.if(FT_DPCM_ENABLE)
	.define MUSIC_HAS_DPCM 1
	.export _sample_play:= FamiToneSamplePlay
.else
	.export _sample_play
	_sample_play:
		rts
.endif


.export music_update=FamiToneUpdate
.else
.out "Famitone2 music driver not used"
.endif

.ifdef USE_FAMISTUDIO

MUSIC_ENABLED = 1

FAMISTUDIO_CFG_EXTERNAL = 1
FAMISTUDIO_CFG_C_BINDINGS = 1
FAMISTUDIO_CFG_THREAD = 1
FAMISTUDIO_CFG_DPCM_SUPPORT = 1

.define FAMISTUDIO_CA65_ZP_SEGMENT   ZEROPAGE
.define FAMISTUDIO_CA65_RAM_SEGMENT  XRAM
.define FAMISTUDIO_CA65_CODE_SEGMENT AUDIO

; NOTICE: Additional settings must be added here depending on the features your music uses


.segment "AUDIO"
.include "../MUSIC/famistudio_ca65.s"
.include "../MUSIC/music_data.s"

; export the common init/update functions for use in C. We have to use the C versions of the
; famistudio exports for this to line up right.
.export music_init:=famistudio_init
.export music_update:=famistudio_update

.if(FAMISTUDIO_CFG_SFX_SUPPORT)
.export sfx_init:=famistudio_sfx_init
.endif

; re-export the famistudio C bindings to the common music_driver.h bindings
.export _music_play:=_famistudio_music_play
.export _music_pause:=_famistudio_music_pause
.export _music_stop:=_famistudio_music_stop

.if(FAMISTUDIO_CFG_SFX_SUPPORT)
	.define MUSIC_HAS_SFX 1
	.export _sfx_play:=_famistudio_sfx_play
.endif

.if(FAMISTUDIO_CFG_DPCM_SUPPORT) && (FAMISTUDIO_CFG_SFX_SUPPORT)
	.define MUSIC_HAS_DPCM 1
	.export _sample_play:=_famistudio_sfx_sample_play
.endif

.segment "SAMPLES"
.export music_samples
music_samples:
.incbin "../MUSIC/music_data.dmc"

.else
.out "Famistudio music driver not used"
.endif

.if .defined(USE_FAMITONE2) || .defined(USE_FAMISTUDIO)
.else
.out "No audio driver selected."
MUSIC_ENABLED = 0
.endif

.popseg
