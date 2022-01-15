#include "LIB/neslib.h"
#include "LIB/nesdoug.h"

// LEVEL NAMETABLES AND COLLISION
#include "Nametable/Forest/level01.h"
#include "Collision/master_collision.h"
#include "Nametable/title_screen.h"

#define LEVEL_TOTAL 1
#define MAX_HEALTH 8

#define TILE_BACK 0x10
#define TILE_HEART_FULL 0x5b
#define TILE_HEART_EMPTY 0x5c

const unsigned char pal_forest_bg[16] = {0x0f, 0x15, 0x20, 0x09, 0x0f, 0x29, 0x1a, 0x09, 0x0f, 0x08, 0x17, 0x27, 0x0f, 0x37, 0x17, 0x09};
const unsigned char pal_palette_spr[16] = {0x0f, 0x20, 0x15, 0x12, 0x0f, 0x11, 0x22, 0x32, 0x0f, 0x13, 0x23, 0x33, 0x0f, 0x14, 0x24, 0x34};

const unsigned char *const level_list[LEVEL_TOTAL * 3] = {
	forest_level_01, forest_col_01, pal_forest_bg // LEVEL 01
};

unsigned char pad1;
unsigned char pad1_new;

static unsigned char i, j;

static unsigned char current_level;
static unsigned char p1_health;
static unsigned char p1_max_health;

static unsigned char game_state;

enum game_states
{
	TITLE,
	MAIN,
	STORY,
	PAUSE,
	GAME_OVER
};

void update_health()
{
	for (i = 0; i < p1_max_health; ++i)
	{
		one_vram_buffer((i > p1_health) ? TILE_HEART_EMPTY : TILE_HEART_FULL, NTADR_A(i + 2, 2));
	}
}

void show_HUD()
{
	// HEALTH
	update_health();

	// XP
	// update_xp();

	// ITEM BOX:
	one_vram_buffer(0x5d, NTADR_A(22, 2));
	one_vram_buffer(0x5e, NTADR_A(23, 2));
	one_vram_buffer(0x5f, NTADR_A(24, 2));
	one_vram_buffer(0x7e, NTADR_A(23, 3));
	one_vram_buffer(0x7f, NTADR_A(23, 4));
	// show sprite

	// COINS
	// update_coins();

	// OTHER??
}

void show_title_screen()
{
	// show press start
	// wait for input
}

void show_game_over()
{
	// TODO
}

void show_screen()
{
	ppu_off();

	vram_adr(NAMETABLE_A);
	vram_unrle(level_list[current_level]);

	ppu_on_all();

	if (game_state == MAIN)
		show_HUD();
}

void load_title_screen()
{
	pal_bg(pal_forest_bg);
	pal_spr(pal_palette_spr);
	vram_adr(NAMETABLE_A);
	vram_unrle(title_screen);
	game_state = TITLE;
}

void main()
{
	// famitone_init(&music_data);
	// sfx_init(&sound_data);
	// nmi_set_callback(famitone_update);

	ppu_off();

	bank_spr(1);
	set_vram_buffer();

	load_title_screen();

	ppu_on_all();

	while (1)
	{
		ppu_wait_nmi();

		while (game_state == TITLE)
		{
			pad1 = pad_poll(0); // read the first controller
			pad1_new = get_pad_new(0);
			if (pad1_new & PAD_START)
			{
				game_state = MAIN;
				current_level = 0;
				p1_health = 3;
				p1_max_health = 3;

				show_screen();
			}
		}

		while (p1_health)
		{

			// game_loop();

			// if(game_over) ++current_level; else show_game_over();
		}

		// show_screen(!game_lives?SCREEN_GAMEOVER:SCREEN_WELLDONE);//show game results
	}
}
