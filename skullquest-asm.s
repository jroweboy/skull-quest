
.define BANK_SELECT $8000
.define BANK_DATA $8001

.importzp CODE_BANK_SELECT, ALT_BANK_SELECT, _current_code_bank
.import music_init, music_update, music_data_the_trial_of_kharzoid_ost, music_samples, _init, sfx_init, sounds
.export init_callback, nmi_callback

init_callback:
    jsr _init
    lda _current_code_bank
    pha
        lda #CODE_BANK_SELECT
        sta BANK_SELECT
        lda #<.bank(music_init)
        sta BANK_DATA
        lda #ALT_BANK_SELECT
        sta BANK_SELECT
        lda #<.bank(music_samples)
        sta BANK_DATA
        lda #1 ; PLATFORM_NTSC
        ldx #<music_data_the_trial_of_kharzoid_ost
        ldy #>music_data_the_trial_of_kharzoid_ost
        jsr music_init
        ; SFX
        lda #1 ; NTSC
        ldx #<sounds
        ldy #>sounds
        jsr sfx_init
        lda #CODE_BANK_SELECT
        sta BANK_SELECT
    pla
    sta BANK_DATA
    rts

nmi_callback:
    lda _current_code_bank
    pha
        lda #CODE_BANK_SELECT
        sta BANK_SELECT
        lda #<.bank(music_update)
        sta BANK_DATA
        jsr music_update
        lda #CODE_BANK_SELECT
        sta BANK_SELECT
    pla
    sta BANK_DATA
    rts
