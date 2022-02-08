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
#define FALSE 0
#define TRUE 1
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

unsigned char debug1, debug2;
unsigned char pad1;
unsigned char pad1_new;
unsigned char c_map[368];

int collision_index, nametable_index, backup_col_index, backup_nt_index;
unsigned char collision_mask, backup_col_mask;
unsigned char i, j, temp, temp_x, temp_y, collision_type, backup_col_type, skull_launched;
unsigned char p1_health;
unsigned char p1_max_health;

unsigned char game_state, current_level;

struct Actor {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
    unsigned char bbox_x;
    unsigned char bbox_y;
    signed char xSpeed;
    signed char ySpeed;
    unsigned char col_direction;
};

struct Actor Paddle = {
    0x75,  // x
    0xCF,  // y
    0x1F,  // width
    7,     // height
    0,     // bbox_x
    1,     // bbox_y
    0,     // xSpeed
    0      // ySpeed
};

struct Actor Skull = {
    0xFF,  // x
    0xFF,  // y
    7,     // width
    7,     // height
    2,     // bbox_x
    2,     // bbox_y
    0,     // xSpeed
    0      // ySpeed
};

enum GameStates { TITLE,
                  MAIN,
                  STORY,
                  PAUSE,
                  GAME_OVER };

enum ActorTypes { BALL,
                  PADDLE,
                  MONSTER };

enum ActorStates { IDLE,
                   WALKING,
                   ATTACKING };

void update_health() {
    if (p1_health) {
        for (i = 0; i < p1_max_health; ++i) {
            one_vram_buffer((i > p1_health) ? TILE_HEART_EMPTY : TILE_HEART_FULL, NTADR_A(i + 2, 2));
        }
    } else {
        // DEATH!!!
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

    if (game_state == MAIN) {
        show_HUD();
        skull_launched = FALSE;
    }
}

void load_title_screen() {
    pal_bg(pal_forest_bg);
    pal_spr(pal_spr_01);
    vram_adr(NAMETABLE_A);
    vram_unrle(title_screen);
    game_state = TITLE;
}

void remove_brick(char tile_type) {
    one_vram_buffer(tile_type, backup_nt_index);
    c_map[backup_col_index] &= backup_nt_index % 2 ? 0b11110000 : 0b00001111;
}

void hit_brick(char tile_type) {
    remove_brick(tile_type);
    // Play sound
    // Update score
    // Spawn random coin ??
}

char set_collision_data(char param_x, char param_y) {
    collision_index = (param_x >> 4) + (((param_y >> 3) - 5) * 16);
    nametable_index = NTADR_A((param_x >> 3), (param_y >> 3));
    collision_type = (param_x >> 3) % 2 ? c_map[collision_index] & 0x0F : c_map[collision_index] >> 4;

    return collision_type;
}

void backup_collision_info() {
    backup_col_index = collision_index;
    backup_nt_index = nametable_index;
    backup_col_type = collision_type;
}

void do_collision() {
    switch (backup_col_type) {
        case 0x01:
            // Just solid
            break;
        case 0x02:
            // Hurt
            --p1_health;
            update_health();
            // Check death
            // play hurt or death sound ?
            // shake screen ??
            // invincible for a few frames ??
            break;
        case 0x03:
            // Left brick
            remove_brick(TILE_BACK);
            ++backup_nt_index;
            hit_brick(TILE_BACK);
            break;
        case 0x04:
            // Right brick
            remove_brick(TILE_BACK);
            --backup_nt_index;
            hit_brick(TILE_BACK);
            break;
        case 0x05:
            // Dot brick
            hit_brick(0x11);
            break;
        case 0x06:
            // Foliage
            hit_brick(0x11);
            break;
        case 0x07:
            break;
        case 0x08:
            break;
        case 0x09:
            break;
        case 0x0A:
            break;
        case 0x0B:
            break;
        case 0x0C:
            break;
        case 0x0D:
            break;
        case 0x0E:
            break;
        case 0x0F:
            break;
    }
}

void check_paddle_input() {
    if ((pad1 & PAD_LEFT) && (Paddle.xSpeed > -MAX_SPEED)) {
        Paddle.xSpeed -= 2;
    }
    if ((pad1 & PAD_RIGHT) && (Paddle.xSpeed < MAX_SPEED)) {
        Paddle.xSpeed += 2;
    }
    if (pad1 & PAD_A) {
        if (!skull_launched) {
            skull_launched = TRUE;
            Skull.xSpeed = 1;
            Skull.ySpeed = -1;
        }

        // WHAT DOES BUTTON A DO?? WEAPON? SPEED_UP THE PADDLE?
    }
    if (pad1 & PAD_B) {
        // WHAT DOES BUTTON B DO?? WEAPON? SPEED_UP THE PADDLE?
    }

    temp_x = Paddle.x + Paddle.xSpeed;  // Bounding box?
    temp_y = Paddle.y;                  // Bounding box?

    // Check collision:
    if (Paddle.xSpeed > 0) {
        while (set_collision_data(temp_x + Paddle.width, temp_y)) {
            --temp_x;
        }
    } else if (Paddle.xSpeed < 0) {
        while (set_collision_data(temp_x, temp_y)) {
            ++temp_x;
        }
    }

    Paddle.x = temp_x;
    // FRICTION
    if (Paddle.xSpeed) {
        Paddle.xSpeed += Paddle.xSpeed > 0 ? -1 : 1;
    }
}

void update_skull() {
    if (skull_launched) {
        if (Skull.xSpeed == 0) {
            ++Skull.xSpeed;
        }
        if (Skull.ySpeed == 0) {
            ++Skull.ySpeed;
        }

        temp_x = Skull.x + Skull.xSpeed;
        temp_y = Skull.y + Skull.ySpeed;
        Skull.col_direction = 0;

        if (Skull.xSpeed > 0) {
            // Going right
            if (Skull.ySpeed > 0) {
                // Check down
                if (set_collision_data(temp_x, temp_y + Skull.height)) {
                    Skull.col_direction |= PAD_DOWN;
                    backup_collision_info();
                    while (set_collision_data(temp_x, temp_y + Skull.height)) {
                        --temp_y;
                    }
                    do_collision();
                }
                // Check right
                if (set_collision_data(temp_x + Skull.width, temp_y)) {
                    Skull.col_direction |= PAD_RIGHT;
                    backup_collision_info();
                    while (set_collision_data(temp_x + Skull.width, temp_y)) {
                        --temp_x;
                    }
                    do_collision();
                    // Check down-right
                } else if (Skull.col_direction == 0 && set_collision_data(temp_x + Skull.width, temp_y + Skull.height)) {
                    Skull.col_direction |= PAD_RIGHT;
                    // Skull.col_direction |= PAD_DOWN;
                    backup_collision_info();
                    while (set_collision_data(temp_x + Skull.width, temp_y + Skull.height)) {
                        --temp_x;
                        --temp_y;
                    }
                    do_collision();
                }
            } else {
                // Check up
                if (set_collision_data(temp_x, temp_y)) {
                    Skull.col_direction |= PAD_UP;
                    backup_collision_info();
                    while (set_collision_data(temp_x, temp_y)) {
                        ++temp_y;
                    }
                    do_collision();
                }
                // Check right
                if (set_collision_data(temp_x + Skull.width, temp_y + Skull.height)) {
                    Skull.col_direction |= PAD_RIGHT;
                    backup_collision_info();
                    while (set_collision_data(temp_x + Skull.width, temp_y + Skull.height)) {
                        --temp_x;
                    }
                    do_collision();
                    // Check up-right
                } else if (Skull.col_direction == 0 && set_collision_data(temp_x + Skull.width, temp_y)) {
                    // Skull.col_direction |= PAD_UP; SET TO RANDOM
                    Skull.col_direction |= PAD_RIGHT;
                    backup_collision_info();
                    while (
                        set_collision_data(temp_x + Skull.width, temp_y)) {
                        --temp_x;
                        ++temp_y;
                    }
                    do_collision();
                }
            }
        } else {
            // Going left
            if (Skull.ySpeed > 0) {
                // Check down
                if (set_collision_data(temp_x + Skull.width, temp_y + Skull.height)) {
                    Skull.col_direction |= PAD_DOWN;
                    backup_collision_info();
                    while (set_collision_data(temp_x + Skull.width, temp_y + Skull.height)) {
                        --temp_y;
                    }
                    do_collision();
                }
                // Check left
                if (set_collision_data(temp_x, temp_y)) {
                    Skull.col_direction |= PAD_LEFT;
                    backup_collision_info();
                    while (set_collision_data(temp_x, temp_y)) {
                        ++temp_x;
                    }
                    do_collision();
                    // Check down-left
                } else if (Skull.col_direction == 0 && set_collision_data(temp_x, temp_y + Skull.height)) {
                    Skull.col_direction |= PAD_DOWN;
                    // Skull.col_direction |= PAD_LEFT;
                    backup_collision_info();
                    while (
                        set_collision_data(temp_x, temp_y + Skull.height)) {
                        ++temp_x;
                        ++temp_y;
                    }
                    do_collision();
                }
            } else {
                // Check up
                if (set_collision_data(temp_x + Skull.width, temp_y)) {
                    Skull.col_direction |= PAD_UP;
                    backup_collision_info();
                    while (set_collision_data(temp_x + Skull.width, temp_y)) {
                        ++temp_y;
                    }
                    do_collision();
                }
                // Check left
                if (set_collision_data(temp_x, temp_y + Skull.height)) {
                    Skull.col_direction |= PAD_LEFT;
                    backup_collision_info();
                    while (set_collision_data(temp_x, temp_y + Skull.height)) {
                        ++temp_x;
                    }
                    do_collision();
                    // Check up-left
                } else if (Skull.col_direction == 0 && set_collision_data(temp_x, temp_y)) {
                    // Skull.col_direction |= PAD_UP; SET TO RANDOM
                    Skull.col_direction |= PAD_LEFT;
                    backup_collision_info();
                    while (set_collision_data(temp_x, temp_y)) {
                        ++temp_x;
                        ++temp_y;
                    }
                    do_collision();
                }
            }
        }

        if ((Skull.col_direction & PAD_DOWN) || (Skull.col_direction & PAD_UP)) {
            Skull.ySpeed = -Skull.ySpeed;
        }
        if ((Skull.col_direction & PAD_RIGHT) || (Skull.col_direction & PAD_LEFT)) {
            Skull.xSpeed = -Skull.xSpeed;
        }

        Skull.x = temp_x;
        Skull.y = temp_y;
    } else {
        // Skull not launched, follow the paddle
        Skull.x = Paddle.x + (Paddle.width >> 1) - (Skull.width >> 1);
        Skull.y = Paddle.y - Skull.height;
    }
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

            check_paddle_input();
            update_skull();
            draw_sprites();

            // game_loop();

            // if(game_over) ++current_level; else show_game_over();
        }

        // show_screen(!game_lives?SCREEN_GAMEOVER:SCREEN_WELLDONE);//show
        // game results
    }
}
