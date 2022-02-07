;
; File generated by cc65 v 2.19 - Git 4f87c7c
;
	.fopt		compiler,"cc65 v 2.19 - Git 4f87c7c"
	.setcpu		"6502"
	.smart		on
	.autoimport	on
	.case		on
	.debuginfo	off
	.importzp	sp, sreg, regsave, regbank
	.importzp	tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
	.macpack	longbranch
	.forceimport	__STARTUP__
	.import		_set_vram_buffer
	.import		_one_vram_buffer
	.import		_get_pad_new
	.import		_pal_bg
	.import		_pal_spr
	.import		_ppu_wait_nmi
	.import		_ppu_off
	.import		_ppu_on_all
	.import		_oam_clear
	.import		_oam_meta_spr
	.import		_pad_poll
	.import		_bank_spr
	.import		_vram_adr
	.import		_vram_fill
	.import		_vram_unrle
	.import		_memcpy
	.export		_forest_col_01
	.export		_forest_level_01
	.export		_title_screen
	.export		_PaddleSpr
	.export		_SkullSpr
	.export		_pal_forest_bg
	.export		_pal_spr_01
	.export		_level_list
	.export		_dx
	.export		_dy
	.export		_debug1
	.export		_debug2
	.export		_pad1
	.export		_pad1_new
	.export		_c_map
	.export		_collision_index
	.export		_i
	.export		_j
	.export		_temp
	.export		_temp_x
	.export		_temp_y
	.export		_next_x
	.export		_next_y
	.export		_collision_type
	.export		_skull_launched
	.export		_p1_health
	.export		_p1_max_health
	.export		_game_state
	.export		_current_level
	.export		_Paddle
	.export		_Skull
	.export		_update_health
	.export		_show_HUD
	.export		_show_title_screen
	.export		_show_game_over
	.export		_show_screen
	.export		_load_title_screen
	.export		_get_collision_type
	.export		_do_collision
	.export		_check_paddle_input
	.export		_update_skull
	.export		_draw_sprites
	.export		_main

.segment	"DATA"

_Paddle:
	.byte	$75
	.byte	$CF
	.byte	$1F
	.byte	$07
	.byte	$00
	.byte	$01
	.byte	$00
	.byte	$00
	.res	1,$00
_Skull:
	.byte	$FF
	.byte	$FF
	.byte	$07
	.byte	$07
	.byte	$02
	.byte	$02
	.byte	$00
	.byte	$00
	.res	1,$00

.segment	"RODATA"

_forest_col_01:
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$06
	.byte	$66
	.byte	$66
	.byte	$60
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$06
	.byte	$66
	.byte	$66
	.byte	$60
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$06
	.byte	$66
	.byte	$66
	.byte	$60
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$06
	.byte	$66
	.byte	$66
	.byte	$60
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$10
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$01
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$34
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$10
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$01
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$00
	.byte	$66
	.byte	$66
	.byte	$66
	.byte	$66
	.byte	$66
	.byte	$66
	.byte	$66
	.byte	$66
	.byte	$00
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$00
	.byte	$00
	.byte	$66
	.byte	$66
	.byte	$66
	.byte	$66
	.byte	$66
	.byte	$66
	.byte	$66
	.byte	$66
	.byte	$00
	.byte	$00
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$00
	.byte	$00
	.byte	$55
	.byte	$55
	.byte	$55
	.byte	$55
	.byte	$55
	.byte	$55
	.byte	$55
	.byte	$55
	.byte	$00
	.byte	$00
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$11
	.byte	$22
	.byte	$22
	.byte	$22
	.byte	$22
	.byte	$22
	.byte	$22
	.byte	$22
	.byte	$22
	.byte	$22
	.byte	$22
	.byte	$22
	.byte	$22
	.byte	$11
	.byte	$11
_forest_level_01:
	.byte	$01
	.byte	$00
	.byte	$01
	.byte	$A0
	.byte	$05
	.byte	$05
	.byte	$09
	.byte	$18
	.byte	$19
	.byte	$18
	.byte	$19
	.byte	$18
	.byte	$19
	.byte	$18
	.byte	$19
	.byte	$18
	.byte	$19
	.byte	$18
	.byte	$19
	.byte	$18
	.byte	$19
	.byte	$18
	.byte	$19
	.byte	$18
	.byte	$19
	.byte	$18
	.byte	$19
	.byte	$18
	.byte	$19
	.byte	$18
	.byte	$19
	.byte	$08
	.byte	$08
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$19
	.byte	$06
	.byte	$07
	.byte	$06
	.byte	$07
	.byte	$06
	.byte	$07
	.byte	$06
	.byte	$07
	.byte	$08
	.byte	$09
	.byte	$08
	.byte	$09
	.byte	$08
	.byte	$09
	.byte	$08
	.byte	$09
	.byte	$06
	.byte	$07
	.byte	$06
	.byte	$07
	.byte	$06
	.byte	$07
	.byte	$06
	.byte	$07
	.byte	$18
	.byte	$18
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$09
	.byte	$16
	.byte	$17
	.byte	$16
	.byte	$17
	.byte	$16
	.byte	$17
	.byte	$16
	.byte	$17
	.byte	$18
	.byte	$19
	.byte	$18
	.byte	$19
	.byte	$18
	.byte	$19
	.byte	$18
	.byte	$19
	.byte	$16
	.byte	$17
	.byte	$16
	.byte	$17
	.byte	$16
	.byte	$17
	.byte	$16
	.byte	$17
	.byte	$16
	.byte	$05
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$19
	.byte	$10
	.byte	$01
	.byte	$07
	.byte	$06
	.byte	$07
	.byte	$06
	.byte	$07
	.byte	$06
	.byte	$07
	.byte	$06
	.byte	$07
	.byte	$10
	.byte	$01
	.byte	$07
	.byte	$08
	.byte	$05
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$09
	.byte	$10
	.byte	$02
	.byte	$03
	.byte	$02
	.byte	$03
	.byte	$02
	.byte	$03
	.byte	$10
	.byte	$16
	.byte	$17
	.byte	$16
	.byte	$17
	.byte	$16
	.byte	$17
	.byte	$16
	.byte	$17
	.byte	$10
	.byte	$02
	.byte	$03
	.byte	$02
	.byte	$03
	.byte	$02
	.byte	$03
	.byte	$10
	.byte	$18
	.byte	$05
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$19
	.byte	$10
	.byte	$12
	.byte	$13
	.byte	$12
	.byte	$13
	.byte	$12
	.byte	$13
	.byte	$10
	.byte	$01
	.byte	$09
	.byte	$12
	.byte	$13
	.byte	$12
	.byte	$13
	.byte	$12
	.byte	$13
	.byte	$10
	.byte	$08
	.byte	$05
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$09
	.byte	$10
	.byte	$01
	.byte	$02
	.byte	$11
	.byte	$01
	.byte	$11
	.byte	$10
	.byte	$01
	.byte	$02
	.byte	$18
	.byte	$05
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$01
	.byte	$02
	.byte	$08
	.byte	$09
	.byte	$10
	.byte	$10
	.byte	$11
	.byte	$01
	.byte	$0F
	.byte	$10
	.byte	$10
	.byte	$08
	.byte	$09
	.byte	$05
	.byte	$01
	.byte	$02
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$08
	.byte	$18
	.byte	$19
	.byte	$09
	.byte	$10
	.byte	$01
	.byte	$11
	.byte	$08
	.byte	$18
	.byte	$19
	.byte	$09
	.byte	$05
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$08
	.byte	$18
	.byte	$08
	.byte	$09
	.byte	$19
	.byte	$09
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$08
	.byte	$18
	.byte	$08
	.byte	$09
	.byte	$19
	.byte	$09
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$18
	.byte	$08
	.byte	$18
	.byte	$19
	.byte	$09
	.byte	$19
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$18
	.byte	$08
	.byte	$18
	.byte	$19
	.byte	$09
	.byte	$19
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$08
	.byte	$18
	.byte	$08
	.byte	$09
	.byte	$19
	.byte	$09
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$14
	.byte	$15
	.byte	$08
	.byte	$18
	.byte	$08
	.byte	$09
	.byte	$19
	.byte	$09
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$08
	.byte	$18
	.byte	$19
	.byte	$09
	.byte	$10
	.byte	$01
	.byte	$11
	.byte	$08
	.byte	$18
	.byte	$19
	.byte	$09
	.byte	$05
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$01
	.byte	$02
	.byte	$06
	.byte	$07
	.byte	$10
	.byte	$10
	.byte	$11
	.byte	$01
	.byte	$0F
	.byte	$10
	.byte	$10
	.byte	$06
	.byte	$07
	.byte	$05
	.byte	$01
	.byte	$02
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$01
	.byte	$02
	.byte	$16
	.byte	$17
	.byte	$10
	.byte	$11
	.byte	$02
	.byte	$03
	.byte	$02
	.byte	$03
	.byte	$02
	.byte	$03
	.byte	$02
	.byte	$03
	.byte	$02
	.byte	$03
	.byte	$02
	.byte	$03
	.byte	$02
	.byte	$03
	.byte	$02
	.byte	$03
	.byte	$11
	.byte	$10
	.byte	$16
	.byte	$17
	.byte	$05
	.byte	$01
	.byte	$02
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$09
	.byte	$10
	.byte	$01
	.byte	$02
	.byte	$11
	.byte	$12
	.byte	$13
	.byte	$12
	.byte	$13
	.byte	$12
	.byte	$13
	.byte	$12
	.byte	$13
	.byte	$12
	.byte	$13
	.byte	$12
	.byte	$13
	.byte	$12
	.byte	$13
	.byte	$12
	.byte	$13
	.byte	$11
	.byte	$10
	.byte	$01
	.byte	$02
	.byte	$08
	.byte	$05
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$19
	.byte	$10
	.byte	$11
	.byte	$01
	.byte	$02
	.byte	$04
	.byte	$01
	.byte	$0F
	.byte	$11
	.byte	$01
	.byte	$02
	.byte	$10
	.byte	$18
	.byte	$05
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$09
	.byte	$10
	.byte	$11
	.byte	$01
	.byte	$15
	.byte	$10
	.byte	$08
	.byte	$05
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$19
	.byte	$10
	.byte	$11
	.byte	$01
	.byte	$15
	.byte	$10
	.byte	$18
	.byte	$05
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$09
	.byte	$10
	.byte	$11
	.byte	$01
	.byte	$15
	.byte	$10
	.byte	$08
	.byte	$05
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$19
	.byte	$10
	.byte	$11
	.byte	$01
	.byte	$15
	.byte	$10
	.byte	$18
	.byte	$05
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$09
	.byte	$10
	.byte	$01
	.byte	$17
	.byte	$08
	.byte	$05
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$05
	.byte	$19
	.byte	$1A
	.byte	$1B
	.byte	$1A
	.byte	$1B
	.byte	$1A
	.byte	$1B
	.byte	$1A
	.byte	$1B
	.byte	$1A
	.byte	$1B
	.byte	$1A
	.byte	$1B
	.byte	$1A
	.byte	$1B
	.byte	$1A
	.byte	$1B
	.byte	$1A
	.byte	$1B
	.byte	$1A
	.byte	$1B
	.byte	$1A
	.byte	$1B
	.byte	$1A
	.byte	$1B
	.byte	$18
	.byte	$05
	.byte	$05
	.byte	$00
	.byte	$00
	.byte	$05
	.byte	$01
	.byte	$1D
	.byte	$00
	.byte	$01
	.byte	$20
	.byte	$55
	.byte	$01
	.byte	$06
	.byte	$51
	.byte	$55
	.byte	$A5
	.byte	$A5
	.byte	$55
	.byte	$55
	.byte	$A5
	.byte	$A5
	.byte	$55
	.byte	$01
	.byte	$03
	.byte	$5A
	.byte	$5A
	.byte	$55
	.byte	$01
	.byte	$04
	.byte	$F5
	.byte	$01
	.byte	$03
	.byte	$55
	.byte	$01
	.byte	$02
	.byte	$65
	.byte	$5F
	.byte	$01
	.byte	$03
	.byte	$95
	.byte	$55
	.byte	$01
	.byte	$09
	.byte	$F5
	.byte	$01
	.byte	$05
	.byte	$55
	.byte	$05
	.byte	$01
	.byte	$06
	.byte	$05
	.byte	$01
	.byte	$00
_title_screen:
	.byte	$01
	.byte	$00
	.byte	$01
	.byte	$FE
	.byte	$00
	.byte	$01
	.byte	$29
	.byte	$53
	.byte	$6B
	.byte	$75
	.byte	$6C
	.byte	$6C
	.byte	$00
	.byte	$01
	.byte	$03
	.byte	$51
	.byte	$75
	.byte	$65
	.byte	$73
	.byte	$74
	.byte	$00
	.byte	$01
	.byte	$B3
	.byte	$50
	.byte	$72
	.byte	$65
	.byte	$73
	.byte	$73
	.byte	$00
	.byte	$53
	.byte	$74
	.byte	$61
	.byte	$72
	.byte	$74
	.byte	$00
	.byte	$01
	.byte	$FE
	.byte	$00
	.byte	$01
	.byte	$32
	.byte	$54
	.byte	$68
	.byte	$65
	.byte	$00
	.byte	$6F
	.byte	$6C
	.byte	$64
	.byte	$00
	.byte	$67
	.byte	$61
	.byte	$6D
	.byte	$65
	.byte	$00
	.byte	$6D
	.byte	$61
	.byte	$6B
	.byte	$65
	.byte	$72
	.byte	$00
	.byte	$01
	.byte	$33
	.byte	$40
	.byte	$32
	.byte	$30
	.byte	$32
	.byte	$32
	.byte	$00
	.byte	$01
	.byte	$8B
	.byte	$00
	.byte	$01
	.byte	$00
_PaddleSpr:
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$08
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$10
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$18
	.byte	$00
	.byte	$00
	.byte	$00
	.byte	$80
_SkullSpr:
	.byte	$00
	.byte	$00
	.byte	$01
	.byte	$00
	.byte	$80
_pal_forest_bg:
	.byte	$0F
	.byte	$15
	.byte	$20
	.byte	$09
	.byte	$0F
	.byte	$29
	.byte	$1A
	.byte	$09
	.byte	$0F
	.byte	$08
	.byte	$17
	.byte	$27
	.byte	$0F
	.byte	$37
	.byte	$17
	.byte	$09
_pal_spr_01:
	.byte	$0F
	.byte	$20
	.byte	$15
	.byte	$12
	.byte	$0F
	.byte	$11
	.byte	$22
	.byte	$32
	.byte	$0F
	.byte	$13
	.byte	$23
	.byte	$33
	.byte	$0F
	.byte	$14
	.byte	$24
	.byte	$34
_level_list:
	.addr	_forest_level_01
	.addr	_forest_col_01
	.addr	_pal_forest_bg

.segment	"BSS"

_dx:
	.res	1,$00
_dy:
	.res	1,$00
_debug1:
	.res	1,$00
_debug2:
	.res	1,$00
_pad1:
	.res	1,$00
_pad1_new:
	.res	1,$00
_c_map:
	.res	368,$00
_collision_index:
	.res	2,$00
_i:
	.res	1,$00
_j:
	.res	1,$00
_temp:
	.res	1,$00
_temp_x:
	.res	1,$00
_temp_y:
	.res	1,$00
_next_x:
	.res	1,$00
_next_y:
	.res	1,$00
_collision_type:
	.res	1,$00
_skull_launched:
	.res	1,$00
_p1_health:
	.res	1,$00
_p1_max_health:
	.res	1,$00
_game_state:
	.res	1,$00
_current_level:
	.res	1,$00

; ---------------------------------------------------------------
; void __near__ update_health (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_update_health: near

.segment	"CODE"

;
; for (i = 0; i < p1_max_health; ++i) {
;
	lda     #$00
	sta     _i
L0009:	lda     _i
	cmp     _p1_max_health
	bcs     L0003
;
; one_vram_buffer((i > p1_health) ? TILE_HEART_EMPTY : TILE_HEART_FULL,
;
	cmp     _p1_health
	bcc     L000A
	beq     L000A
	lda     #$5C
	jmp     L000B
L000A:	lda     #$5B
L000B:	jsr     pusha
;
; NTADR_A(i + 2, 2));
;
	ldx     #$00
	lda     _i
	clc
	adc     #$02
	bcc     L0008
	inx
L0008:	ora     #$40
	pha
	txa
	ora     #$20
	tax
	pla
	jsr     _one_vram_buffer
;
; for (i = 0; i < p1_max_health; ++i) {
;
	inc     _i
	jmp     L0009
;
; }
;
L0003:	rts

.endproc

; ---------------------------------------------------------------
; void __near__ show_HUD (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_show_HUD: near

.segment	"CODE"

;
; vram_adr(0x23C0);
;
	ldx     #$23
	lda     #$C0
	jsr     _vram_adr
;
; vram_fill(0x00, 8);
;
	lda     #$00
	jsr     pusha
	tax
	lda     #$08
	jsr     _vram_fill
;
; update_health();
;
	jsr     _update_health
;
; one_vram_buffer(0x5d, NTADR_A(22, 2));
;
	lda     #$5D
	jsr     pusha
	ldx     #$20
	lda     #$56
	jsr     _one_vram_buffer
;
; one_vram_buffer(0x5e, NTADR_A(23, 2));
;
	lda     #$5E
	jsr     pusha
	ldx     #$20
	lda     #$57
	jsr     _one_vram_buffer
;
; one_vram_buffer(0x5f, NTADR_A(24, 2));
;
	lda     #$5F
	jsr     pusha
	ldx     #$20
	lda     #$58
	jsr     _one_vram_buffer
;
; one_vram_buffer(0x7e, NTADR_A(23, 3));
;
	lda     #$7E
	jsr     pusha
	ldx     #$20
	lda     #$77
	jsr     _one_vram_buffer
;
; one_vram_buffer(0x7f, NTADR_A(24, 3));
;
	lda     #$7F
	jsr     pusha
	ldx     #$20
	lda     #$78
	jmp     _one_vram_buffer

.endproc

; ---------------------------------------------------------------
; void __near__ show_title_screen (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_show_title_screen: near

.segment	"CODE"

;
; }
;
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ show_game_over (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_show_game_over: near

.segment	"CODE"

;
; }
;
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ show_screen (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_show_screen: near

.segment	"CODE"

;
; ppu_off();
;
	jsr     _ppu_off
;
; vram_adr(NAMETABLE_A);
;
	ldx     #$20
	lda     #$00
	jsr     _vram_adr
;
; vram_unrle(level_list[current_level]);
;
	ldx     #$00
	lda     _current_level
	asl     a
	bcc     L0005
	inx
	clc
L0005:	adc     #<(_level_list)
	sta     ptr1
	txa
	adc     #>(_level_list)
	sta     ptr1+1
	ldy     #$01
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	jsr     _vram_unrle
;
; memcpy(c_map, level_list[current_level + 1], 368);
;
	lda     #<(_c_map)
	ldx     #>(_c_map)
	jsr     pushax
	ldx     #$00
	lda     _current_level
	clc
	adc     #$01
	bcc     L0002
	inx
L0002:	stx     tmp1
	asl     a
	rol     tmp1
	clc
	adc     #<(_level_list)
	sta     ptr1
	lda     tmp1
	adc     #>(_level_list)
	sta     ptr1+1
	ldy     #$01
	lda     (ptr1),y
	tax
	dey
	lda     (ptr1),y
	jsr     pushax
	ldx     #$01
	lda     #$70
	jsr     _memcpy
;
; ppu_on_all();
;
	jsr     _ppu_on_all
;
; if (game_state == MAIN) {
;
	lda     _game_state
	cmp     #$01
	bne     L0003
;
; show_HUD();
;
	jsr     _show_HUD
;
; skull_launched = FALSE;
;
	lda     #$00
	sta     _skull_launched
;
; }
;
L0003:	rts

.endproc

; ---------------------------------------------------------------
; void __near__ load_title_screen (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_load_title_screen: near

.segment	"CODE"

;
; pal_bg(pal_forest_bg);
;
	lda     #<(_pal_forest_bg)
	ldx     #>(_pal_forest_bg)
	jsr     _pal_bg
;
; pal_spr(pal_spr_01);
;
	lda     #<(_pal_spr_01)
	ldx     #>(_pal_spr_01)
	jsr     _pal_spr
;
; vram_adr(NAMETABLE_A);
;
	ldx     #$20
	lda     #$00
	jsr     _vram_adr
;
; vram_unrle(title_screen);
;
	lda     #<(_title_screen)
	ldx     #>(_title_screen)
	jsr     _vram_unrle
;
; game_state = TITLE;
;
	lda     #$00
	sta     _game_state
;
; }
;
	rts

.endproc

; ---------------------------------------------------------------
; int __near__ get_collision_type (char param_x, char param_y)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_get_collision_type: near

.segment	"CODE"

;
; int get_collision_type(char param_x, char param_y) {
;
	jsr     pusha
;
; collision_index = (param_x >> 4) + (((param_y >> 3) - 5) * 16);
;
	ldy     #$01
	lda     (sp),y
	lsr     a
	lsr     a
	lsr     a
	lsr     a
	jsr     pusha0
	ldy     #$02
	lda     (sp),y
	lsr     a
	lsr     a
	lsr     a
	sec
	sbc     #$05
	bcs     L0002
	ldx     #$FF
L0002:	jsr     aslax4
	jsr     tosaddax
	sta     _collision_index
	stx     _collision_index+1
;
; collision_type = (param_x >> 3) % 2 ? c_map[collision_index] & 0x0F
;
	ldy     #$01
	lda     (sp),y
	lsr     a
	lsr     a
	lsr     a
	jsr     pusha0
	lda     #$02
	jsr     tosmoda0
	stx     tmp1
	ora     tmp1
	beq     L0003
	lda     _collision_index
	sta     ptr1
	lda     _collision_index+1
	clc
	adc     #>(_c_map)
	sta     ptr1+1
	ldy     #<(_c_map)
	lda     (ptr1),y
;
; : (c_map[collision_index] & 0xF0) >> 4;
;
	and     #$0F
	jmp     L0005
L0003:	lda     _collision_index
	sta     ptr1
	lda     _collision_index+1
	clc
	adc     #>(_c_map)
	sta     ptr1+1
	ldy     #<(_c_map)
	lda     (ptr1),y
	and     #$F0
	lsr     a
	lsr     a
	lsr     a
	lsr     a
L0005:	sta     _collision_type
;
; return collision_type;
;
	ldx     #$00
	lda     _collision_type
;
; }
;
	jmp     incsp2

.endproc

; ---------------------------------------------------------------
; void __near__ do_collision (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_do_collision: near

.segment	"CODE"

;
; switch (collision_type) {
;
	lda     _collision_type
;
; }
;
	cmp     #$01
	beq     L0003
	cmp     #$02
	beq     L0003
	cmp     #$03
	beq     L0003
	cmp     #$04
	beq     L0003
	cmp     #$05
	beq     L0003
	cmp     #$06
	beq     L0003
	cmp     #$07
	beq     L0003
	cmp     #$08
	beq     L0003
	cmp     #$09
	beq     L0003
	cmp     #$0A
	beq     L0003
	cmp     #$0B
	beq     L0003
	cmp     #$0C
	beq     L0003
	cmp     #$0D
	beq     L0003
	cmp     #$0E
	beq     L0003
	cmp     #$0F
;
; }
;
L0003:	rts

.endproc

; ---------------------------------------------------------------
; void __near__ check_paddle_input (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_check_paddle_input: near

.segment	"CODE"

;
; if ((pad1 & PAD_LEFT) && (Paddle.xSpeed > -MAX_SPEED)) {
;
	lda     _pad1
	and     #$02
	beq     L002C
	lda     _Paddle+6
	sec
	sbc     #$FA
	bvs     L0005
	eor     #$80
L0005:	bpl     L002C
;
; Paddle.xSpeed -= 2;
;
	lda     _Paddle+6
	sec
	sbc     #$02
	sta     _Paddle+6
;
; if ((pad1 & PAD_RIGHT) && (Paddle.xSpeed < MAX_SPEED)) {
;
L002C:	lda     _pad1
	and     #$01
	beq     L0030
	lda     _Paddle+6
	sec
	sbc     #$07
	bvc     L000C
	eor     #$80
L000C:	bpl     L0030
;
; Paddle.xSpeed += 2;
;
	lda     #$02
	clc
	adc     _Paddle+6
	sta     _Paddle+6
;
; if (pad1 & PAD_A) {
;
L0030:	lda     _pad1
	and     #$80
	beq     L0031
;
; if (!skull_launched) {
;
	lda     _skull_launched
	bne     L0031
;
; skull_launched = TRUE;
;
	lda     #$01
	sta     _skull_launched
;
; Skull.xSpeed = 1;
;
	sta     _Skull+6
;
; Skull.ySpeed = -1;
;
	lda     #$FF
	sta     _Skull+7
;
; if (pad1 & PAD_B) {
;
L0031:	lda     _pad1
	ldx     #$00
	and     #$40
	stx     tmp1
	ora     tmp1
;
; temp_x = Paddle.x + Paddle.xSpeed;  // Bounding box?
;
	lda     _Paddle+6
	clc
	adc     _Paddle
	sta     _temp_x
;
; temp_y = Paddle.y;                  // Bounding box?
;
	lda     _Paddle+1
	sta     _temp_y
;
; if (Paddle.xSpeed > 0) {
;
	lda     _Paddle+6
	sec
	sbc     #$01
	bvs     L0016
	eor     #$80
L0016:	bpl     L0033
;
; while (get_collision_type(temp_x + Paddle.width, temp_y)) {
;
	jmp     L0032
;
; --temp_x;
;
L0017:	dec     _temp_x
;
; while (get_collision_type(temp_x + Paddle.width, temp_y)) {
;
L0032:	lda     _temp_x
	clc
	adc     _Paddle+2
	jsr     pusha
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	bne     L0017
;
; } else if (Paddle.xSpeed < 0) {
;
	jmp     L0035
L0033:	lda     _Paddle+6
	asl     a
	bcc     L0035
;
; while (get_collision_type(temp_x, temp_y)) {
;
	jmp     L0034
;
; ++temp_x;
;
L001D:	inc     _temp_x
;
; while (get_collision_type(temp_x, temp_y)) {
;
L0034:	lda     _temp_x
	jsr     pusha
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	bne     L001D
;
; Paddle.x = temp_x;
;
L0035:	lda     _temp_x
	sta     _Paddle
;
; if (Paddle.xSpeed) {
;
	lda     _Paddle+6
	beq     L0026
;
; Paddle.xSpeed += Paddle.xSpeed > 0 ? -1 : 1;
;
	sec
	sbc     #$01
	bvs     L0022
	eor     #$80
L0022:	bpl     L0036
	lda     #$FF
	jmp     L0024
L0036:	lda     #$01
L0024:	cmp     #$80
	clc
	adc     _Paddle+6
	sta     _Paddle+6
;
; }
;
L0026:	rts

.endproc

; ---------------------------------------------------------------
; void __near__ update_skull (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_update_skull: near

.segment	"CODE"

;
; if (skull_launched) {
;
	lda     _skull_launched
	jeq     L0002
;
; if (Skull.xSpeed == 0) {
;
	lda     _Skull+6
	bne     L0074
;
; ++Skull.xSpeed;
;
	inc     _Skull+6
;
; if (Skull.ySpeed == 0) {
;
L0074:	lda     _Skull+7
	bne     L0076
;
; ++Skull.ySpeed;
;
	inc     _Skull+7
;
; temp_x = Skull.x + Skull.xSpeed;
;
L0076:	lda     _Skull+6
	clc
	adc     _Skull
	sta     _temp_x
;
; temp_y = Skull.y + Skull.ySpeed;
;
	lda     _Skull+7
	clc
	adc     _Skull+1
	sta     _temp_y
;
; collision_type = 0;
;
	lda     #$00
	sta     _collision_type
;
; Skull.col_direction = 0;
;
	sta     _Skull+8
;
; if (Skull.xSpeed > 0) {
;
	lda     _Skull+6
	sec
	sbc     #$01
	bvs     L000D
	eor     #$80
L000D:	jpl     L0079
;
; if (Skull.ySpeed > 0) {
;
	lda     _Skull+7
	sec
	sbc     #$01
	bvs     L0010
	eor     #$80
L0010:	jpl     L0077
;
; if (get_collision_type(temp_x, temp_y + Skull.height)) {
;
	lda     _temp_x
	jsr     pusha
	lda     _temp_y
	clc
	adc     _Skull+3
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	beq     L0011
;
; Skull.col_direction = Skull.col_direction | PAD_DOWN;
;
	lda     _Skull+8
	ora     #$04
	sta     _Skull+8
;
; do_collision();
;
	jsr     _do_collision
;
; while (get_collision_type(temp_x, temp_y + Skull.height)) {
;
	jmp     L0014
;
; --temp_x;
;
L0012:	dec     _temp_x
;
; --temp_y;
;
	dec     _temp_y
;
; while (get_collision_type(temp_x, temp_y + Skull.height)) {
;
L0014:	lda     _temp_x
	jsr     pusha
	lda     _temp_y
	clc
	adc     _Skull+3
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	bne     L0012
;
; } else {
;
	jmp     L004D
;
; if (get_collision_type(temp_x + Skull.width, temp_y)) {
;
L0011:	lda     _temp_x
	clc
	adc     _Skull+2
	jsr     pusha
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	beq     L0016
;
; Skull.col_direction = Skull.col_direction | PAD_RIGHT;
;
	lda     _Skull+8
	ora     #$01
	sta     _Skull+8
;
; do_collision();
;
	jsr     _do_collision
;
; while (
;
	jmp     L0019
;
; --temp_x;
;
L0017:	dec     _temp_x
;
; get_collision_type(temp_x + Skull.width, temp_y)) {
;
L0019:	lda     _temp_x
	clc
	adc     _Skull+2
	jsr     pusha
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	bne     L0017
;
; } else if (get_collision_type(temp_x + Skull.width, temp_y + Skull.height)) {
;
	jmp     L004D
L0016:	lda     _temp_x
	clc
	adc     _Skull+2
	jsr     pusha
	lda     _temp_y
	clc
	adc     _Skull+3
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	jeq     L004D
;
; Skull.col_direction = Skull.col_direction | PAD_RIGHT;
;
	lda     _Skull+8
	ora     #$01
	sta     _Skull+8
;
; Skull.col_direction = Skull.col_direction | PAD_DOWN;
;
	ora     #$04
	sta     _Skull+8
;
; do_collision();
;
	jsr     _do_collision
;
; while (
;
	jmp     L001E
;
; --temp_x;
;
L001C:	dec     _temp_x
;
; --temp_y;
;
	dec     _temp_y
;
; get_collision_type(temp_x + Skull.width, temp_y)) {
;
L001E:	lda     _temp_x
	clc
	adc     _Skull+2
	jsr     pusha
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	bne     L001C
;
; } else {
;
	jmp     L004D
;
; if (get_collision_type(temp_x, --temp_y)) {
;
L0077:	lda     _temp_x
	jsr     pusha
	dec     _temp_y
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	beq     L0020
;
; Skull.col_direction = Skull.col_direction | PAD_UP;
;
	lda     _Skull+8
	ora     #$08
	sta     _Skull+8
;
; do_collision();
;
	jsr     _do_collision
;
; while (get_collision_type(temp_x, --temp_y)) {
;
	jmp     L0023
;
; --temp_x;
;
L0021:	dec     _temp_x
;
; ++temp_y;
;
	inc     _temp_y
;
; while (get_collision_type(temp_x, --temp_y)) {
;
L0023:	lda     _temp_x
	jsr     pusha
	dec     _temp_y
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	bne     L0021
;
; } else {
;
	jmp     L004D
;
; if (get_collision_type(temp_x + Skull.width, temp_y + Skull.height)) {
;
L0020:	lda     _temp_x
	clc
	adc     _Skull+2
	jsr     pusha
	lda     _temp_y
	clc
	adc     _Skull+3
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	beq     L0025
;
; Skull.col_direction = Skull.col_direction | PAD_RIGHT;
;
	lda     _Skull+8
	ora     #$01
	sta     _Skull+8
;
; while (
;
	jmp     L0078
;
; --temp_x;
;
L0026:	dec     _temp_x
;
; get_collision_type(temp_x + Skull.width, temp_y + Skull.height)) {
;
L0078:	lda     _temp_x
	clc
	adc     _Skull+2
	jsr     pusha
	lda     _temp_y
	clc
	adc     _Skull+3
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	bne     L0026
;
; } else if (get_collision_type(temp_x + Skull.width, --temp_y)) {
;
	jmp     L004D
L0025:	lda     _temp_x
	clc
	adc     _Skull+2
	jsr     pusha
	dec     _temp_y
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	jeq     L004D
;
; Skull.col_direction = Skull.col_direction | PAD_RIGHT;
;
	lda     _Skull+8
	ora     #$01
	sta     _Skull+8
;
; Skull.col_direction = Skull.col_direction | PAD_UP;
;
	ora     #$08
	sta     _Skull+8
;
; do_collision();
;
	jsr     _do_collision
;
; while (
;
	jmp     L002D
;
; --temp_x;
;
L002B:	dec     _temp_x
;
; ++temp_y;
;
	inc     _temp_y
;
; get_collision_type(temp_x + Skull.width, temp_y)) {
;
L002D:	lda     _temp_x
	clc
	adc     _Skull+2
	jsr     pusha
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	bne     L002B
;
; } else {
;
	jmp     L004D
;
; if (Skull.ySpeed > 0) {
;
L0079:	lda     _Skull+7
	sec
	sbc     #$01
	bvs     L0031
	eor     #$80
L0031:	jpl     L007A
;
; if (get_collision_type(temp_x + Skull.width, temp_y + Skull.height)) {
;
	lda     _temp_x
	clc
	adc     _Skull+2
	jsr     pusha
	lda     _temp_y
	clc
	adc     _Skull+3
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	beq     L0032
;
; Skull.col_direction = Skull.col_direction | PAD_DOWN;
;
	lda     _Skull+8
	ora     #$04
	sta     _Skull+8
;
; do_collision();
;
	jsr     _do_collision
;
; while (get_collision_type(temp_x + Skull.width, temp_y + Skull.height)) {
;
	jmp     L0035
;
; ++temp_x;
;
L0033:	inc     _temp_x
;
; --temp_y;
;
	dec     _temp_y
;
; while (get_collision_type(temp_x + Skull.width, temp_y + Skull.height)) {
;
L0035:	lda     _temp_x
	clc
	adc     _Skull+2
	jsr     pusha
	lda     _temp_y
	clc
	adc     _Skull+3
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	bne     L0033
;
; } else {
;
	jmp     L004D
;
; if (get_collision_type(temp_x, temp_y)) {
;
L0032:	lda     _temp_x
	jsr     pusha
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	beq     L0037
;
; Skull.col_direction = Skull.col_direction | PAD_LEFT;
;
	lda     _Skull+8
	ora     #$02
	sta     _Skull+8
;
; do_collision();
;
	jsr     _do_collision
;
; while (get_collision_type(temp_x, temp_y)) {
;
	jmp     L003A
;
; ++temp_x;
;
L0038:	inc     _temp_x
;
; while (get_collision_type(temp_x, temp_y)) {
;
L003A:	lda     _temp_x
	jsr     pusha
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	bne     L0038
;
; } else if (get_collision_type(temp_x, temp_y + Skull.height)) {
;
	jmp     L004D
L0037:	lda     _temp_x
	jsr     pusha
	lda     _temp_y
	clc
	adc     _Skull+3
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	jeq     L004D
;
; Skull.col_direction = Skull.col_direction | PAD_LEFT;
;
	lda     _Skull+8
	ora     #$02
	sta     _Skull+8
;
; Skull.col_direction = Skull.col_direction | PAD_DOWN;
;
	ora     #$04
	sta     _Skull+8
;
; do_collision();
;
	jsr     _do_collision
;
; while (
;
	jmp     L003F
;
; ++temp_x;
;
L003D:	inc     _temp_x
;
; ++temp_y;
;
	inc     _temp_y
;
; get_collision_type(temp_x, temp_y + Skull.height)) {
;
L003F:	lda     _temp_x
	jsr     pusha
	lda     _temp_y
	clc
	adc     _Skull+3
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	bne     L003D
;
; } else {
;
	jmp     L004D
;
; if (get_collision_type(temp_x + Skull.width, temp_y)) {
;
L007A:	lda     _temp_x
	clc
	adc     _Skull+2
	jsr     pusha
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	beq     L0041
;
; Skull.col_direction = Skull.col_direction | PAD_UP;
;
	lda     _Skull+8
	ora     #$08
	sta     _Skull+8
;
; do_collision();
;
	jsr     _do_collision
;
; while (get_collision_type(temp_x + Skull.width, temp_y)) {
;
	jmp     L0044
;
; ++temp_x;
;
L0042:	inc     _temp_x
;
; ++temp_y;
;
	inc     _temp_y
;
; while (get_collision_type(temp_x + Skull.width, temp_y)) {
;
L0044:	lda     _temp_x
	clc
	adc     _Skull+2
	jsr     pusha
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	bne     L0042
;
; } else {
;
	jmp     L004D
;
; if (get_collision_type(temp_x, temp_y + Skull.height)) {
;
L0041:	lda     _temp_x
	jsr     pusha
	lda     _temp_y
	clc
	adc     _Skull+3
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	beq     L0046
;
; Skull.col_direction = Skull.col_direction | PAD_LEFT;
;
	lda     _Skull+8
	ora     #$02
	sta     _Skull+8
;
; do_collision();
;
	jsr     _do_collision
;
; while (get_collision_type(temp_x, temp_y)) {
;
	jmp     L0049
;
; ++temp_x;
;
L0047:	inc     _temp_x
;
; while (get_collision_type(temp_x, temp_y)) {
;
L0049:	lda     _temp_x
	jsr     pusha
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	bne     L0047
;
; } else if (get_collision_type(temp_x, temp_y)) {
;
	jmp     L004D
L0046:	lda     _temp_x
	jsr     pusha
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	beq     L004D
;
; Skull.col_direction = Skull.col_direction | PAD_LEFT;
;
	lda     _Skull+8
	ora     #$02
	sta     _Skull+8
;
; Skull.col_direction = Skull.col_direction | PAD_DOWN;
;
	ora     #$04
	sta     _Skull+8
;
; do_collision();
;
	jsr     _do_collision
;
; while (get_collision_type(temp_x, temp_y)) {
;
	jmp     L004E
;
; ++temp_y;
;
L004C:	inc     _temp_y
;
; while (get_collision_type(temp_x, temp_y)) {
;
L004E:	lda     _temp_x
	jsr     pusha
	lda     _temp_y
	jsr     _get_collision_type
	stx     tmp1
	ora     tmp1
	bne     L004C
;
; if ((Skull.col_direction & PAD_DOWN) ||
;
L004D:	lda     _Skull+8
	and     #$04
	bne     L007B
;
; (Skull.col_direction & PAD_UP)) {
;
	lda     _Skull+8
	and     #$08
	beq     L007C
;
; Skull.ySpeed = -Skull.ySpeed;
;
L007B:	lda     _Skull+7
	eor     #$FF
	clc
	adc     #$01
	cmp     #$80
	sta     _Skull+7
;
; if ((Skull.col_direction & PAD_RIGHT) ||
;
L007C:	lda     _Skull+8
	and     #$01
	bne     L007D
;
; (Skull.col_direction & PAD_LEFT)) {
;
	lda     _Skull+8
	and     #$02
	beq     L007E
;
; Skull.xSpeed = -Skull.xSpeed;
;
L007D:	lda     _Skull+6
	eor     #$FF
	clc
	adc     #$01
	cmp     #$80
	sta     _Skull+6
;
; Skull.x = temp_x;
;
L007E:	lda     _temp_x
	sta     _Skull
;
; Skull.y = temp_y;
;
	lda     _temp_y
;
; } else {
;
	jmp     L0073
;
; Skull.x = Paddle.x + (Paddle.width >> 1) - (Skull.width >> 1);
;
L0002:	tax
	lda     _Paddle+2
	lsr     a
	clc
	adc     _Paddle
	bcc     L0072
	inx
L0072:	jsr     pushax
	lda     _Skull+2
	lsr     a
	jsr     tossuba0
	sta     _Skull
;
; Skull.y = Paddle.y - Skull.height;
;
	lda     _Paddle+1
	sec
	sbc     _Skull+3
L0073:	sta     _Skull+1
;
; }
;
	rts

.endproc

; ---------------------------------------------------------------
; void __near__ draw_sprites (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_draw_sprites: near

.segment	"CODE"

;
; oam_clear();
;
	jsr     _oam_clear
;
; oam_meta_spr(Paddle.x, Paddle.y, PaddleSpr);
;
	jsr     decsp2
	lda     _Paddle
	ldy     #$01
	sta     (sp),y
	lda     _Paddle+1
	dey
	sta     (sp),y
	lda     #<(_PaddleSpr)
	ldx     #>(_PaddleSpr)
	jsr     _oam_meta_spr
;
; oam_meta_spr(Skull.x, Skull.y, SkullSpr);
;
	jsr     decsp2
	lda     _Skull
	ldy     #$01
	sta     (sp),y
	lda     _Skull+1
	dey
	sta     (sp),y
	lda     #<(_SkullSpr)
	ldx     #>(_SkullSpr)
	jmp     _oam_meta_spr

.endproc

; ---------------------------------------------------------------
; void __near__ main (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_main: near

.segment	"CODE"

;
; ppu_off();
;
	jsr     _ppu_off
;
; bank_spr(1);
;
	lda     #$01
	jsr     _bank_spr
;
; set_vram_buffer();
;
	jsr     _set_vram_buffer
;
; load_title_screen();
;
	jsr     _load_title_screen
;
; ppu_on_all();
;
	jsr     _ppu_on_all
;
; while (game_state == TITLE) {
;
	jmp     L000C
;
; ppu_wait_nmi();
;
L0005:	jsr     _ppu_wait_nmi
;
; pad1 = pad_poll(0);
;
	lda     #$00
	jsr     _pad_poll
	sta     _pad1
;
; pad1_new = get_pad_new(0);
;
	lda     #$00
	jsr     _get_pad_new
	sta     _pad1_new
;
; if (pad1_new & PAD_START) {
;
	and     #$10
	beq     L000C
;
; game_state = MAIN;
;
	lda     #$01
	sta     _game_state
;
; current_level = 0;
;
	lda     #$00
	sta     _current_level
;
; p1_health = 3;
;
	lda     #$03
	sta     _p1_health
;
; p1_max_health = 3;
;
	sta     _p1_max_health
;
; show_screen();
;
	jsr     _show_screen
;
; while (game_state == TITLE) {
;
L000C:	lda     _game_state
	beq     L0005
;
; while (game_state == MAIN) {
;
	jmp     L000D
;
; ppu_wait_nmi();
;
L0009:	jsr     _ppu_wait_nmi
;
; pad1 = pad_poll(0);
;
	lda     #$00
	jsr     _pad_poll
	sta     _pad1
;
; pad1_new = get_pad_new(0);
;
	lda     #$00
	jsr     _get_pad_new
	sta     _pad1_new
;
; check_paddle_input();
;
	jsr     _check_paddle_input
;
; update_skull();
;
	jsr     _update_skull
;
; draw_sprites();
;
	jsr     _draw_sprites
;
; while (game_state == MAIN) {
;
L000D:	lda     _game_state
	cmp     #$01
	beq     L0009
;
; while (1) {
;
	jmp     L000C

.endproc

