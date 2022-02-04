#include "LIB/nesdoug.h"
#include "LIB/neslib.h"

// LEVEL NAMETABLES AND COLLISION
#include "Collision/master_collision.h"
#include "Nametable/Forest/level01.h"
#include "Nametable/title_screen.h"
#include "sprites.h"

#define LEVEL_TOTAL 1
#define MAX_HEALTH 8
#define MAX_SPEED 7

#define TILE_BACK 0x10
#define TILE_HEART_FULL 0x5b
#define TILE_HEART_EMPTY 0x5c

const unsigned char pal_forest_bg[16] = {
    0x0f, 0x15, 0x20, 0x09,  // GUID Palette
    0x0f, 0x29, 0x1a, 0x09,  // Grass Foliage
    0x0f, 0x08, 0x17, 0x27,  // Trunk
    0x0f, 0x37, 0x17, 0x09   // Bricks
};

const unsigned char pal_spr_01[16] = {
    0x0f, 0x20, 0x15, 0x12, 0x0f, 0x11, 0x22, 0x32,
    0x0f, 0x13, 0x23, 0x33, 0x0f, 0x14, 0x24, 0x34  //
};


const unsigned char *const level_list[LEVEL_TOTAL * 3] = {
    forest_level_01, forest_col_01, pal_forest_bg  // LEVEL 01
};

signed char dx, dy, direction;

unsigned char debug[16];
unsigned char pad1;
unsigned char pad1_new;
unsigned char c_map[368];

unsigned char i, j, temp, temp_x, temp_y, next_x, next_y, tile_index;
int col_index;
unsigned char p1_health;
unsigned char p1_max_health;

char current_level;
unsigned char game_state;

struct Actor {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
    unsigned char bbox_x;
    unsigned char bbox_y;
    signed char xSpeed;
    signed char ySpeed;
};

struct Actor Paddle = {0x75, 0xCF, 0x1F, 7, 0, 1, 0, 0};
struct Actor Skull = {0xFF, 0xFF, 5, 5};

enum GameStates { TITLE, MAIN, STORY, PAUSE, GAME_OVER };

enum ActorTypes { BALL, PADDLE, MONSTER };

enum ActorStates { IDLE, WALKING, ATTACKING };

void update_health() {
    for (i = 0; i < p1_max_health; ++i) {
        one_vram_buffer((i > p1_health) ? TILE_HEART_EMPTY : TILE_HEART_FULL,
                        NTADR_A(i + 2, 2));
    }
}

void show_HUD() {
    vram_adr(0x23C0);
    vram_fill(0x00, 8);
    // HEALTH
    update_health();

    // XP
    // update_xp();

    // ITEM BOX:
    one_vram_buffer(0x5d, NTADR_A(22, 2));
    one_vram_buffer(0x5e, NTADR_A(23, 2));
    one_vram_buffer(0x5f, NTADR_A(24, 2));
    one_vram_buffer(0x7e, NTADR_A(23, 3));
    one_vram_buffer(0x7f, NTADR_A(24, 3));

    // show item sprite

    // COINS
    // update_coins();

    // OTHER??
}

void show_title_screen() {
    // show press start
    // wait for input
}

void show_game_over() {
    // TODO
}

void show_screen() {
    ppu_off();

    vram_adr(NAMETABLE_A);
    vram_unrle(level_list[current_level]);
    memcpy(c_map, level_list[current_level + 1], 368);

    ppu_on_all();

    if (game_state == MAIN) show_HUD();
}

void load_title_screen() {
    pal_bg(pal_forest_bg);
    pal_spr(pal_spr_01);
    vram_adr(NAMETABLE_A);
    vram_unrle(title_screen);
    game_state = TITLE;
}

int get_collision_type() {
    col_index = (temp_x >> 4) + (((temp_y >> 3) - 5) * 16);
    // tile_index = (temp_x >> 3) % 2 ? c_map[col_index] >> 4 : c_map[col_index] << 4;	
	// return tile_index;

    return (temp_x >> 3) % 2 ? c_map[col_index] & 0x0F : c_map[col_index] & 0xF0;
}

void check_input() {
    dx = 0;

    if ((pad1 & PAD_LEFT) && (Paddle.xSpeed > -MAX_SPEED)) {
        dx -= 2;
    }
    if ((pad1 & PAD_RIGHT) && (Paddle.xSpeed < MAX_SPEED)) {
        dx += 2;
    }

    Paddle.xSpeed += dx;
    next_x = Paddle.x + Paddle.xSpeed;
    temp_x = next_x; // Bounding box?
    temp_y = Paddle.y; // Bounding box?

    // Check collision:
    if (Paddle.xSpeed > 0) {
        temp_x += Paddle.width;
        while (get_collision_type()) {
            --temp_x;
			--next_x;
        }
    } else if (Paddle.xSpeed < 0) {
        while (get_collision_type()) {
            ++temp_x;
			++next_x;
        }
    }

    Paddle.x = next_x;
    // FRICTION
    if (Paddle.xSpeed) {
        Paddle.xSpeed += Paddle.xSpeed > 0 ? -1 : 1;
    }

    // If skull not launched update skull x and y as well
}

void draw_sprites(void) {
    // clear all sprites from sprite buffer
    oam_clear();

    // draw 2 metasprites
    oam_meta_spr(Paddle.x, Paddle.y, PaddleSpr);
    oam_meta_spr(Skull.x, Skull.y, SkullSpr);
}

void main() {
    // famitone_init(&music_data);
    // sfx_init(&sound_data);
    // nmi_set_callback(famitone_update);

    ppu_off();

    bank_spr(1);
    set_vram_buffer();

    load_title_screen();

    ppu_on_all();

    while (1) {
        while (game_state == TITLE) {
            ppu_wait_nmi();

            // PRESS START TO PLAY!
            pad1 = pad_poll(0);
            pad1_new = get_pad_new(0);
            if (pad1_new & PAD_START) {
                game_state = MAIN;
                current_level = 0;
                p1_health = 3;
                p1_max_health = 3;

                show_screen();
            }
        }

        while (game_state == MAIN) {
            ppu_wait_nmi();

            pad1 = pad_poll(0);
            pad1_new = get_pad_new(0);

            check_input();
            draw_sprites();

            // game_loop();

            // if(game_over) ++current_level; else show_game_over();
        }

        // show_screen(!game_lives?SCREEN_GAMEOVER:SCREEN_WELLDONE);//show game
        // results
    }
}
