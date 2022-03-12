#include "LIB/nesdoug.h"
#include "LIB/neslib.h"

// LEVEL NAMETABLES AND COLLISION
#include "Collision/master_collision.h"
#include "Nametable/Forest/level01.h"
#include "Nametable/Forest/level02.h"
#include "Nametable/title_screen.h"
#include "sprites.h"

#define ONES 7
#define TENS 6
#define HUNDREDS 5
#define THOUSANDS 4
#define TEN_THOUSANDS 3
#define HUNDRED_THOUSANDS 2
#define MILLIONS 1
#define TEN_MILLIONS 0

#define LEVEL_TOTAL 2
#define MAX_HEALTH 8
#define SPEED_STEP 1
#define TILE_BACK 0x10
#define TILE_BACK_GRASS 0x23
#define TILE_HEART_FULL 0x5B
#define TILE_HEART_EMPTY 0x5C
#define COL_TYPE_SOFT 0x06
#define COL_OFFSET 5

#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1

#define TITLE 0
#define MAIN 1
#define STORY 2
#define PAUSE 3
#define GAME_OVER 4

#define SKULL 4
#define PADDLE 0

const unsigned char pal_forest_bg[16] = {
    0x0f, 0x15, 0x20, 0x09,  // GUID Palette
    0x0f, 0x29, 0x1a, 0x09,  // Grass Foliage
    0x0f, 0x08, 0x17, 0x27,  // Trunk
    0x0f, 0x37, 0x17, 0x09   // Bricks
};

const char pal_spr_01[16] = {
    0x0f, 0x20, 0x15, 0x12, 0x0f, 0x11, 0x22, 0x32, 0x0f, 0x13, 0x23, 0x33, 0x0f, 0x14, 0x24, 0x34  //
};

const unsigned char* level_list[LEVEL_TOTAL * 3] = {
    forest_level_01, forest_col_01, pal_forest_bg,  // LEVEL 01
    forest_level_02, forest_col_02, pal_forest_bg   // LEVEL 02
};

static char exp[] = "00000000";

static unsigned char debug1, debug2;
static unsigned char pad1;
static unsigned char pad1_new;
static unsigned char c_map[368];
static unsigned char skull_index = 0;

static int collision_index, backup_col_index, backup_nt_index;
static unsigned char pad_index, temp_y_col, temp_x_col;
static unsigned char i, z, temp, temp2, temp_x, temp_y, backup_col_type, skull_launched;
static unsigned char p1_health;
static unsigned char p1_max_health;
static unsigned char game_state, current_level, paddle_count;

#define ACTOR_NUMBER 5
typedef struct {
    unsigned char x[ACTOR_NUMBER];
    unsigned char y[ACTOR_NUMBER];
    unsigned char width[ACTOR_NUMBER];
    unsigned char height[ACTOR_NUMBER];
    unsigned char bbox_x[ACTOR_NUMBER];
    unsigned char bbox_y[ACTOR_NUMBER];
    signed char xSpeed[ACTOR_NUMBER];
    signed char ySpeed[ACTOR_NUMBER];
    signed char xDir[ACTOR_NUMBER];
    signed char yDir[ACTOR_NUMBER];
    unsigned char xSpeedFloat[ACTOR_NUMBER];
    unsigned char ySpeedFloat[ACTOR_NUMBER];
    unsigned char xRemain[ACTOR_NUMBER];
    unsigned char yRemain[ACTOR_NUMBER];
    unsigned char xVelocity[ACTOR_NUMBER];
    unsigned char yVelocity[ACTOR_NUMBER];
    unsigned char minSpeed[ACTOR_NUMBER];
    unsigned char maxSpeed[ACTOR_NUMBER];
} Actors;

// index 0 - 3 paddles
// index 4 - 5 skulls
// index 7 ... ennemies
Actors actors;

void update_health() {
    if (p1_health > 0) {
        for (i = 0; i < p1_max_health; ++i) {
            one_vram_buffer((i < p1_health) ? TILE_HEART_FULL : TILE_HEART_EMPTY, NTADR_A(i + 2, 2));
        }
    } else {
        // DEATH!!!
        one_vram_buffer(TILE_HEART_EMPTY, NTADR_A(2, 2));
        // TODO
        // play death sound / animation
        // change game_state
    }
}

void update_xp() {
    multi_vram_buffer_horz(exp, sizeof(exp), NTADR_A(12, 2));
}

void add_xp(unsigned char value, unsigned char pos) {
    unsigned char result = exp[pos] + value - 0x30;
    if (result > 9) {
        if (pos == TEN_MILLIONS) {
            result = 9;
        } else {
            add_xp(1, pos - 1);
            result -= 10;
        }
    }
    exp[pos] = 0x30 + result;
    update_xp();
}

void show_HUD() {
    vram_adr(0x23C0);
    vram_fill(0x00, 8);

    // HEALTH
    update_health();

    // XP
    update_xp();

    // ITEM BOX:
    one_vram_buffer(0x5d, NTADR_A(22, 2));
    one_vram_buffer(0x5e, NTADR_A(23, 2));
    one_vram_buffer(0x5f, NTADR_A(24, 2));
    one_vram_buffer(0x7e, NTADR_A(23, 3));
    one_vram_buffer(0x7f, NTADR_A(24, 3));

    // TODO show item sprite

    // COINS
    // TODO update_coins();

    // OTHER??
}

void show_title_screen() {
    // show press start
    // wait for input
}

void show_game_over() {
    // TODO
}

void load_paddles() {
    switch (current_level) {
        case 0:
            paddle_count = 2;
            actors.x[0] = 0x78;
            actors.y[0] = 0xD0;
            actors.x[1] = 0x78;
            actors.y[1] = 0x50;
            break;
        case 1:
            paddle_count = 4;
            actors.x[0] = 0x70;  // 14
            actors.y[0] = 0xD0;  // 26
            actors.x[1] = 0x70;  // 14
            actors.y[1] = 0x38;  // 7
            actors.x[2] = 0x18;  // 3
            actors.y[2] = 0x70;  // 14
            actors.x[3] = 0xE0;  // 28
            actors.y[3] = 0x70;  // 14
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        case 11:
            break;
        case 12:
            break;
        case 13:
            break;
        case 14:
            break;
        case 15:
            break;
        case 16:
            break;
        case 17:
            break;
        case 18:
            break;
        case 19:
            break;
        case 20:
            break;
        case 21:
            break;
        case 22:
            break;
        case 23:
            break;
        case 24:
            break;
        case 25:
            break;
        case 26:
            break;
        case 27:
            break;
        case 28:
            break;
        case 29:
            break;
        case 30:
            break;
        case 31:
            break;
        case 32:
            break;
        case 33:
            break;
    }

    for (i = 0; i < paddle_count; ++i) {
        // The first 2 paddles are horizontal, the others vertical
        if (i < 2) {
            actors.width[i] = 0x20;   // 32
            actors.height[i] = 0x04;  // 4
            actors.bbox_x[i] = 0x00;  // 2
            actors.bbox_y[i] = 0x02;
        } else {
            actors.width[i] = 0x04;
            actors.height[i] = 0x20;
            actors.bbox_x[i] = 0x02;
            actors.bbox_y[i] = 0x00;
        }
        actors.xSpeed[i] = 0;
        actors.ySpeed[i] = 0;
        actors.xDir[i] = 0;
        actors.yDir[i] = 0;
        actors.xSpeedFloat[i] = 0;
        actors.ySpeedFloat[i] = 0;
        actors.xVelocity[i] = 0;
        actors.yVelocity[i] = 0;
        actors.minSpeed[i] = 0;
        actors.maxSpeed[i] = 250;
    }
}

void init_skull() {
    actors.x[SKULL] = 0xFF;
    actors.y[SKULL] = 0xFF;
    actors.width[SKULL] = 6;
    actors.height[SKULL] = 6;
    actors.bbox_x[SKULL] = 1;
    actors.bbox_y[SKULL] = 1;
    actors.xSpeed[SKULL] = 0;
    actors.ySpeed[SKULL] = 0;
    actors.xDir[SKULL] = RIGHT;
    actors.yDir[SKULL] = UP;
    actors.xSpeedFloat[SKULL] = 80;
    actors.ySpeedFloat[SKULL] = 80;
    actors.xRemain[SKULL] = 0;
    actors.yRemain[SKULL] = 0;
    actors.xVelocity[SKULL] = 0;
    actors.yVelocity[SKULL] = 0;
    actors.minSpeed[SKULL] = 64;
    actors.maxSpeed[SKULL] = 250;
}

void load_level() {
    ppu_off();

    vram_adr(NAMETABLE_A);
    vram_unrle(level_list[current_level * 3]);

    // TODO: compress col data! and decompress here:
    memcpy(c_map, level_list[current_level * 3 + 1], 368);

    load_paddles();

    init_skull();

    ppu_on_all();

    if (game_state == MAIN) {
        show_HUD();
        skull_launched = FALSE;
    }
}

void load_title_screen() {
    pal_bg((const char*)pal_forest_bg);
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

// z index must be defined first
// not really decimal.... I know it's a mess...
signed char get_x_speed() {
    actors.xSpeed[z] = actors.xSpeedFloat[z] >> 7;
    actors.xRemain[z] += actors.xSpeedFloat[z] & 0b01111111;  // MODULO 128
    temp = 0;
    temp2 = 0;

    // Remain of Speed Float
    if (actors.xRemain[z] > 127) {
        actors.xRemain[z] &= 0b01111111;
        temp = 1;
    }

    // Velocity. (For exemple: when paddle hits the skull, it sets xVelocity to 80)
    temp2 = ((actors.xVelocity[z] > 40) || (actors.xVelocity[z] % 2)) ? 1 : 0;

    if (actors.xVelocity[z]) {
        --actors.xVelocity[z];
    }

    return (actors.xSpeed[z] + temp + temp2) * actors.xDir[z];
}

signed char get_y_speed() {
    actors.ySpeed[z] = actors.ySpeedFloat[z] >> 7;
    actors.yRemain[z] += actors.ySpeedFloat[z] & 0b01111111;  // MODULO 128
    temp = 0;
    temp2 = 0;

    if (actors.yRemain[z] > 127) {
        actors.yRemain[z] &= 0b01111111;
        temp = 1;
    }

    temp2 = ((actors.yVelocity[z] > 40) || (actors.yVelocity[z] % 2)) ? 1 : 0;

    if (actors.yVelocity[z]) {
        --actors.yVelocity[z];
    }

    return (actors.ySpeed[z] + temp + temp2) * actors.yDir[z];
}

void add_x_speed(unsigned char val) {
    temp = actors.xSpeedFloat[z];
    actors.xSpeedFloat[z] += val;
    if (actors.xSpeedFloat[z] > actors.maxSpeed[z] || actors.xSpeedFloat[z] < temp) {
        actors.xSpeedFloat[z] = actors.maxSpeed[z];
    }
}

void subtract_x_speed(unsigned char val) {
    temp = actors.xSpeedFloat[z];
    actors.xSpeedFloat[z] -= val;
    if (actors.xSpeedFloat[z] < actors.minSpeed[z] || actors.xSpeedFloat[z] > temp) {
        actors.xSpeedFloat[z] = actors.minSpeed[z];
    }
}

void add_y_speed(unsigned char val) {
    temp = actors.ySpeedFloat[z];
    actors.ySpeedFloat[z] += val;
    if (actors.ySpeedFloat[z] > actors.maxSpeed[z] || actors.ySpeedFloat[z] < temp) {
        actors.ySpeedFloat[z] = actors.maxSpeed[z];
    }
}

void subtract_y_speed(unsigned char val) {
    temp = actors.ySpeedFloat[z];
    actors.ySpeedFloat[z] -= val;
    if (actors.ySpeedFloat[z] < actors.minSpeed[z] || actors.ySpeedFloat[z] > temp) {
        actors.ySpeedFloat[z] = actors.minSpeed[z];
    }
}

char get_collision_type() {
    collision_index = (temp_x_col >> 4) + (((temp_y_col >> 3) - 5) * 16);
    return (temp_x_col >> 3) % 2 ? c_map[collision_index] & 0x0F : c_map[collision_index] >> 4;
}

char set_collision_data() {
    backup_nt_index = NTADR_A((temp_x_col >> 3), (temp_y_col >> 3));
    backup_col_type = get_collision_type();
    backup_col_index = collision_index;
    return backup_col_type;
}

void do_skull_tile_collision() {
    switch (backup_col_type) {
        case 0x01:
            // Just solid
            break;
        case 0x02:
            // Hurt
            if (p1_health) {
                --p1_health;
            }
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
            add_xp(1, HUNDREDS);
            break;
        case 0x04:
            // Right brick
            remove_brick(TILE_BACK);
            --backup_nt_index;
            hit_brick(TILE_BACK);
            add_xp(1, HUNDREDS);
            break;
        case 0x05:
            // Dot brick
            hit_brick(0x11);
            add_xp(5, TENS);
            break;
        case 0x06:
            // Foliage
            hit_brick(TILE_BACK_GRASS);
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

char is_skull_h_beside() {
    return !(actors.y[SKULL] + 1 < actors.y[pad_index] + 6) && (actors.y[SKULL] + 7 > actors.y[pad_index] + 2);
}

char is_skull_v_beside() {
    return !(actors.x[SKULL] + 1 < actors.x[pad_index] + 6) && (actors.x[SKULL] + 7 > actors.x[pad_index] + 2);
}

char is_skull_collision_paddle() {
    // Skull temp_x and temp_y already include the bbox
    return (temp_x < actors.x[pad_index] + actors.width[pad_index] + actors.bbox_x[pad_index] &&
            temp_x + actors.width[SKULL] > actors.x[pad_index] + actors.bbox_x[pad_index] &&
            temp_y < actors.y[pad_index] + actors.height[pad_index] + actors.bbox_y[pad_index] &&
            temp_y + actors.height[SKULL] > actors.y[pad_index] + actors.bbox_y[pad_index]);
}

// TODO Will probably have to merge with above function...
char is_paddle_collision_skull() {
    // Skull bbox is hard-coded here
    return (actors.x[SKULL] + 7 > actors.x[pad_index] + actors.bbox_x[pad_index] &&
            actors.y[SKULL] + 7 > actors.y[pad_index] + actors.bbox_y[pad_index] &&
            actors.x[SKULL] + 1 < actors.x[pad_index] + actors.width[pad_index] + actors.bbox_x[pad_index] &&
            actors.y[SKULL] + 1 < actors.y[pad_index] + actors.height[pad_index] + actors.bbox_y[pad_index]);
}

// Paddle collision with wall and skull
void move_horizontal_paddle() {
    temp_x_col = actors.x[pad_index];
    temp_y_col = actors.y[pad_index];

    // Check wall collision:
    if (actors.xDir[pad_index] == LEFT) {
        --temp_x_col;
        if (get_collision_type()) {
            ++temp_x_col;
        } else {
            temp_x_col = actors.x[pad_index] + get_x_speed();
        }
    } else {
        temp_x_col += actors.width[pad_index];
        if (get_collision_type()) {
            temp_x_col = actors.x[pad_index];
        } else {
            temp_x_col = actors.x[pad_index] + get_x_speed();
        }
    }

    // Hit the skull
    if (is_paddle_collision_skull()) {
        if ((pad1 & 0b00000011) && is_skull_h_beside()) {
            actors.xVelocity[SKULL] = 60;
        }
    } else {
        // Everything's fine, update x
        actors.x[pad_index] = temp_x_col;
    }

    // FRICTION
    if (actors.xSpeedFloat[pad_index]) {
        subtract_x_speed(16);
    }
}

void move_vertical_paddle() {
    temp_x_col = actors.x[pad_index];
    temp_y_col = actors.y[pad_index];

    // Check wall collision:
    if (actors.yDir[pad_index] == UP) {
        --temp_y_col;
        if (get_collision_type()) {
            ++temp_y_col;
        } else {
            temp_y_col = actors.y[pad_index] + get_y_speed();
        }
    } else {
        temp_y_col += actors.height[pad_index];
        if (get_collision_type()) {
            temp_y_col = actors.y[pad_index];
        } else {
            temp_y_col = actors.y[pad_index] + get_y_speed();
        }
    }

    // Hit the skull
    if (is_paddle_collision_skull()) {
        if ((pad1 & 0b00001100) && is_skull_v_beside()) {
            actors.yVelocity[SKULL] = 60;
        }
    } else {
        // Everything's fine, update x
        actors.y[pad_index] = temp_y_col;
    }

    // FRICTION
    if (actors.ySpeedFloat[pad_index]) {
        subtract_y_speed(16);
    }
}

// Skull collision with paddle
void check_paddle_collision() {
    // paddle 0 is always on bottom, no need to check if skull is not in the zone
    temp = FALSE;
    if (actors.y[SKULL] > 127) {
        temp = TRUE;
        pad_index = 0;
    } else if (paddle_count > 1) {
        temp = TRUE;
        pad_index = 1;
    }
    if (temp && is_skull_collision_paddle()) {
        // horizontal paddle
        if (temp_x < actors.x[pad_index] + (actors.width[pad_index] >> 1)) {
            // We hit left side of Paddle
            if (temp_x <= actors.x[pad_index] + 4) {
                actors.xDir[SKULL] = LEFT;
                actors.xSpeedFloat[SKULL] = 140;
                actors.ySpeedFloat[SKULL] = 60;
            } else if (temp_x <= actors.x[pad_index] + 8) {
                actors.xDir[SKULL] = LEFT;
                actors.xSpeedFloat[SKULL] = 100;
                actors.ySpeedFloat[SKULL] = 100;
            } else {
                actors.xSpeedFloat[SKULL] = 60;
                actors.ySpeedFloat[SKULL] = 140;
            }
        } else {
            // Right side of Paddle
            if (temp_x >= actors.x[pad_index] + actors.width[pad_index] - 4) {
                actors.xDir[SKULL] = RIGHT;
                actors.xSpeedFloat[SKULL] = 140;
                actors.ySpeedFloat[SKULL] = 60;
            } else if (temp_x >= actors.x[pad_index] + actors.width[pad_index] - 8) {
                actors.xDir[SKULL] = RIGHT;
                actors.xSpeedFloat[SKULL] = 100;
                actors.ySpeedFloat[SKULL] = 100;
            } else {
                actors.xSpeedFloat[SKULL] = 60;
                actors.ySpeedFloat[SKULL] = 140;
            }
        }

        // Skull going up or down?
        if (actors.y[SKULL] < actors.y[pad_index]) {
            actors.yDir[SKULL] = UP;
            // if (!is_skull_beside()) {
            //     temp_y = actors.y[pad_index] - actors.height[SKULL];
            // }
        } else {
            actors.yDir[SKULL] = DOWN;
            // if (!is_skull_beside()) {
            //     temp_y = actors.y[pad_index] + 4;
            // }
        }
    }

    temp = FALSE;
    if (paddle_count > 2) {
        if (actors.x[SKULL] < 127) {
            temp = TRUE;
            pad_index = 2;
        } else if (paddle_count > 3) {
            temp = TRUE;
            pad_index = 3;
        }
    }
    if (temp) {
        if (is_skull_collision_paddle()) {
            // vertical paddle
            if (temp_y < actors.y[pad_index] + (actors.height[pad_index] >> 1)) {
                // We hit upper side of Paddle
                if (temp_y <= actors.y[pad_index] + 4) {
                    actors.yDir[SKULL] = UP;
                    actors.xSpeedFloat[SKULL] = 60;
                    actors.ySpeedFloat[SKULL] = 140;
                } else if (temp_x <= actors.x[pad_index] + 8) {
                    actors.yDir[SKULL] = UP;
                    actors.xSpeedFloat[SKULL] = 100;
                    actors.ySpeedFloat[SKULL] = 100;
                } else {
                    actors.xSpeedFloat[SKULL] = 140;
                    actors.ySpeedFloat[SKULL] = 60;
                }
            } else {
                // Lower side of Paddle
                if (temp_y >= actors.y[pad_index] + actors.height[pad_index] - 4) {
                    actors.yDir[SKULL] = DOWN;
                    actors.xSpeedFloat[SKULL] = 60;
                    actors.ySpeedFloat[SKULL] = 140;
                } else if (temp_x >= actors.x[pad_index] + actors.width[pad_index] - 8) {
                    actors.yDir[SKULL] = RIGHT;
                    actors.xSpeedFloat[SKULL] = 100;
                    actors.ySpeedFloat[SKULL] = 100;
                } else {
                    actors.xSpeedFloat[SKULL] = 140;
                    actors.ySpeedFloat[SKULL] = 60;
                }
            }

            // Skull going left or right?
            if (actors.x[SKULL] < actors.x[pad_index]) {
                actors.xDir[SKULL] = LEFT;
                // if (!is_skull_beside()) {
                //     temp_y = actors.y[pad_index] - actors.height[SKULL];
                // }
            } else {
                actors.xDir[SKULL] = RIGHT;
                // if (!is_skull_beside()) {
                //     temp_y = actors.y[pad_index] + 4;
                // }
            }
        }
    }
}

void check_main_input() {
    for (pad_index = 0; pad_index < paddle_count; ++pad_index) {
        z = pad_index;
        if (pad_index < 2) {
            // Horizontal paddle
            if (pad1 & PAD_LEFT) {
                add_x_speed(80);
                actors.xDir[pad_index] = LEFT;
            }
            if (pad1 & PAD_RIGHT) {
                add_x_speed(80);
                actors.xDir[pad_index] = RIGHT;
            }
            move_horizontal_paddle();
        } else {
            // Vertical paddle
            if (pad1 & PAD_UP) {
                add_y_speed(80);
                actors.yDir[pad_index] = UP;
            }
            if (pad1 & PAD_DOWN) {
                add_y_speed(80);
                actors.yDir[pad_index] = DOWN;
            }
            move_vertical_paddle();
        }
    }

    if (pad1_new & PAD_A) {
        if (skull_launched) {
            temp = 80;
            actors.xVelocity[0] = temp;
            actors.xVelocity[1] = temp;
            actors.yVelocity[2] = temp;
            actors.yVelocity[3] = temp;
        } else {
            skull_launched = TRUE;
        }
    }

    if (pad1 & PAD_B) {
        // TODO ITEM USE
    }

    if (pad1 & PAD_START) {
        // TODO ITEM SELECTION...
        // stop music (or lower volume?)
        // palette fade
        // write PAUSE
        // while (on pause)
        // if already on pause resume gameplay
    }

    if (pad1 & PAD_SELECT) {
        // TODO Item selection
    }
}

void update_skull() {
    // TODO: We should probably check to make sure skull is within boundaries
    backup_col_type = NULL;

    if (skull_launched) {
        // Required for get_x_speed()
        z = SKULL;

        temp_x = actors.x[SKULL] + get_x_speed();
        temp_y = actors.y[SKULL] + get_y_speed();

        // bbox of skull is 1, so we increment because it's faster than addition
        ++temp_x;
        ++temp_y;

        if (actors.xDir[SKULL] == RIGHT) {
            if (actors.yDir[SKULL] == DOWN) {
                // Going DOWN RIGHT
                // Check down
                temp_x_col = temp_x;
                temp_y_col = temp_y + actors.height[SKULL];
                if (set_collision_data()) {
                    if (backup_col_type != COL_TYPE_SOFT) {
                        actors.yDir[SKULL] = UP;
                        temp_y -= temp_y % 8;
                    }
                    do_skull_tile_collision();
                }
                temp_x_col += actors.width[SKULL];
                temp_y_col = temp_y;
                if (set_collision_data()) {
                    // Check right
                    if (backup_col_type != COL_TYPE_SOFT) {
                        actors.xDir[SKULL] = LEFT;
                        temp_x -= temp_x % 8;
                    }
                    do_skull_tile_collision();
                } else {
                    // Check down-right
                    temp_y_col += actors.height[SKULL];
                    if (set_collision_data()) {
                        if (backup_col_type != COL_TYPE_SOFT) {
                            if (temp_y % 8 < COL_OFFSET && temp_x % 8 > 3) {
                                actors.yDir[SKULL] = UP;
                                temp_y -= temp_y % 8;
                            } else {
                                actors.xDir[SKULL] = LEFT;
                                temp_x -= temp_x % 8;
                            }
                        }
                        do_skull_tile_collision();
                    }
                }
            } else {
                // Going UP RIGHT
                // Check up
                temp_x_col = temp_x;
                temp_y_col = temp_y;
                if (set_collision_data()) {
                    if (backup_col_type != COL_TYPE_SOFT) {
                        actors.yDir[SKULL] = DOWN;
                        temp_y += 8;
                        temp_y -= temp_y % 8;
                    }
                    do_skull_tile_collision();
                }
                // Check right
                temp_x_col = temp_x + actors.width[SKULL];
                temp_y_col += actors.height[SKULL];
                if (set_collision_data()) {
                    if (backup_col_type != COL_TYPE_SOFT) {
                        actors.xDir[SKULL] = LEFT;
                        temp_x -= temp_x % 8;
                    }
                    do_skull_tile_collision();
                } else {
                    // Check up right
                    temp_y_col = temp_y;
                    if (set_collision_data()) {
                        if (backup_col_type != COL_TYPE_SOFT) {
                            if (temp_y % 8 < COL_OFFSET && temp_x % 8 < 3) {
                                actors.xDir[SKULL] = LEFT;
                                temp_x -= temp_x % 8;
                            } else {
                                actors.yDir[SKULL] = DOWN;
                                temp_y += 8;
                                temp_y -= temp_y % 8;
                            }
                        }
                        do_skull_tile_collision();
                    }
                }
            }
        } else {
            if (actors.yDir[SKULL] == DOWN) {
                // Going DOWN LEFT
                // Check down
                temp_x_col = temp_x + actors.width[SKULL];
                temp_y_col = temp_y + actors.height[SKULL];
                if (set_collision_data()) {
                    if (backup_col_type != COL_TYPE_SOFT) {
                        actors.yDir[SKULL] = UP;
                        temp_y -= temp_y % 8;
                    }
                    do_skull_tile_collision();
                }
                // Check left
                temp_x_col = temp_x;
                temp_y_col = temp_y;
                if (set_collision_data()) {
                    if (backup_col_type != COL_TYPE_SOFT) {
                        actors.xDir[SKULL] = RIGHT;
                        temp_x += 8;
                        temp_x -= temp_x % 8;
                    }
                    do_skull_tile_collision();
                } else {
                    // Check down-left
                    temp_x_col = temp_x;
                    temp_y_col += actors.height[SKULL];
                    if (set_collision_data()) {
                        if (backup_col_type != COL_TYPE_SOFT) {
                            if (temp_y % 8 < COL_OFFSET && temp_x % 8 > 3) {
                                actors.yDir[SKULL] = UP;
                                temp_y -= temp_y % 8;
                            } else {
                                actors.xDir[SKULL] = RIGHT;
                                temp_x += 8;
                                temp_x -= temp_x % 8;
                            }
                        }
                        do_skull_tile_collision();
                    }
                }
            } else {
                // Going UP LEFT
                // Check up
                temp_x_col = temp_x + actors.width[SKULL];
                temp_y_col = temp_y;
                if (set_collision_data()) {
                    if (backup_col_type != COL_TYPE_SOFT) {
                        actors.yDir[SKULL] = DOWN;
                        temp_y += 8;
                        temp_y -= temp_y % 8;
                    }
                    do_skull_tile_collision();
                }
                // Check left
                temp_x_col = temp_x;
                temp_y_col = temp_y + actors.height[SKULL];
                if (set_collision_data()) {
                    if (backup_col_type != COL_TYPE_SOFT) {
                        actors.xDir[SKULL] = RIGHT;
                        temp_x += 8;
                        temp_x -= temp_x % 8;
                    }
                    do_skull_tile_collision();
                } else {
                    // Check up-left
                    temp_y_col = temp_y;
                    if (set_collision_data()) {
                        if (backup_col_type != COL_TYPE_SOFT) {
                            if (temp_y % 8 < COL_OFFSET && temp_x % 8 > 3) {
                                actors.xDir[SKULL] = RIGHT;
                                temp_x += 8;
                                temp_x -= temp_x % 8;

                            } else {
                                actors.yDir[SKULL] = DOWN;
                                temp_y += 8;
                                temp_y -= temp_y % 8;
                            }
                        }
                        do_skull_tile_collision();
                    }
                }
            }
        }

        check_paddle_collision();

        // We decrement because Skull bbox is 1
        --temp_x;
        --temp_y;
    } else {
        // Skull not launched, follow the paddle
        temp_x = actors.x[PADDLE] + (actors.width[PADDLE] >> 1) - (actors.width[SKULL] >> 1);
        temp_y = actors.y[PADDLE] + actors.bbox_y[PADDLE] - actors.height[SKULL] - actors.bbox_x[SKULL];
    }

    // All is fine, we can update x and y
    actors.x[SKULL] = temp_x;
    actors.y[SKULL] = temp_y;
}

void draw_sprites(void) {
    // clear all sprites from sprite buffer
    oam_clear();

    // draw paddles
    for (i = 0; i < paddle_count; ++i) {
        if (i < 2) {
            oam_meta_spr(actors.x[i], actors.y[i], HorizontalPaddleSpr);
        } else {
            oam_meta_spr(actors.x[i], actors.y[i], VerticalPaddleSpr);
        }
    }
    oam_spr(actors.x[SKULL], actors.y[SKULL], 0x00, 0b00000000);
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
                current_level = 1;
                p1_health = 3;
                p1_max_health = 3;

                load_level();
            }
        }

        while (game_state == MAIN) {
            ppu_wait_nmi();

            pad1 = pad_poll(0);
            pad1_new = get_pad_new(0);

            check_main_input();
            update_skull();

            draw_sprites();
            // gray_line();

            // game_loop();

            // if(game_over) ++current_level; else show_game_over();
        }

        // show_screen(!game_lives?SCREEN_GAMEOVER:SCREEN_WELLDONE);//show
        // game results
    }
}
