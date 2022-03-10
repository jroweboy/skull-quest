#include "LIB/nesdoug.h"
#include "LIB/neslib.h"

// LEVEL NAMETABLES AND COLLISION
#include "Collision/master_collision.h"
#include "Nametable/Forest/level01.h"
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

#define LEVEL_TOTAL 1
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
    forest_level_01, forest_col_01, pal_forest_bg  // LEVEL 01
};

static char exp[] = "00000000";

static unsigned char debug1, debug2;
static unsigned char pad1;
static unsigned char pad1_new;
static unsigned char c_map[368];

static int collision_index, backup_col_index, backup_nt_index;
static unsigned char pad_index, temp_y_col, temp_x_col;
static unsigned char i, temp, temp2, temp_x, temp_y, backup_col_type, skull_launched;
static unsigned char p1_health;
static unsigned char p1_max_health;
static unsigned char game_state, current_level, is_soft_hit, paddle_count;

typedef struct {
    unsigned char x;
    unsigned char y;
    unsigned char width;
    unsigned char height;
    unsigned char bbox_x;
    unsigned char bbox_y;
    signed char xSpeed;
    signed char ySpeed;
    signed char xDir;
    signed char yDir;
    unsigned char xSpeedFloat;
    unsigned char ySpeedFloat;
    unsigned char xRemain;
    unsigned char yRemain;
    unsigned char xVelocity;
    unsigned char yVelocity;
    unsigned char minSpeed;
    unsigned char maxSpeed;
} Actor;

Actor* actor;

Actor paddles[4];

Actor Skull = {
    0xFF,   // x
    0xFF,   // y
    6,      // width
    6,      // height
    1,      // bbox_x
    1,      // bbox_y
    0,      // xSpeed
    0,      // ySpeed
    RIGHT,  // xDir
    UP,     // yDir
    100,    // xSpeedFloat
    100,    // ySpeedFloat
    0,      // xRemain
    0,      // yRemain
    0,      // xVelocity
    0,      // yVelocity
    64,     // minSpeed
    250     // maxSpeed
};

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
            paddles[0].x = 0x75;
            paddles[0].y = 0xCF;
            paddles[1].x = 0x75;
            paddles[1].y = 0x50;
            break;
        case 1:
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
            paddles[i].width = 0x20;
            paddles[i].height = 0x04;
            paddles[i].bbox_x = 0x02;
            paddles[i].bbox_y = 0x00;
        } else {
            paddles[i].width = 0x04;
            paddles[i].height = 0x20;
            paddles[i].bbox_x = 0x00;
            paddles[i].bbox_y = 0x02;
        }
        paddles[i].xSpeed = 0;
        paddles[i].ySpeed = 0;
        paddles[i].xDir = 0;
        paddles[i].yDir = 0;
        paddles[i].xSpeedFloat = 0;
        paddles[i].ySpeedFloat = 0;
        paddles[i].xVelocity = 0;
        paddles[i].yVelocity = 0;
        paddles[i].minSpeed = 0;
        paddles[i].maxSpeed = 250;
    }
}

void load_level() {
    ppu_off();

    vram_adr(NAMETABLE_A);
    vram_unrle(level_list[current_level]);

    // TODO: compress col data! and decompress here:
    memcpy(c_map, level_list[current_level + 1], 368);

    load_paddles();

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
    is_soft_hit = FALSE;
    // Play sound
    // Update score
    // Spawn random coin ??
}

// actor must be defined first
// not really decimal.... I know it's a mess...
signed char get_x_speed() {
    actor->xSpeed = actor->xSpeedFloat >> 7;
    actor->xRemain += actor->xSpeedFloat & 0b01111111;  // MODULO 128
    temp = 0;
    temp2 = 0;

    // Remain of Speed Float
    if (actor->xRemain > 127) {
        actor->xRemain &= 0b01111111;
        temp = 1;
    }

    // Velocity. (For exemple: when paddle hits the skull, it sets xVelocity to 80)
    temp2 = ((actor->xVelocity > 40) || (actor->xVelocity % 2)) ? 1 : 0;

    if (actor->xVelocity) {
        --actor->xVelocity;
    }

    return (actor->xSpeed + temp + temp2) * actor->xDir;
}

signed char get_y_speed() {
    actor->ySpeed = actor->ySpeedFloat >> 7;
    actor->yRemain += actor->ySpeedFloat & 0b01111111;  // MODULO 128
    temp = 0;
    temp2 = 0;

    if (actor->yRemain > 127) {
        actor->yRemain &= 0b01111111;
        temp = 1;
    }

    temp2 = ((actor->yVelocity > 40) || (actor->yVelocity % 2)) ? 1 : 0;

    if (actor->yVelocity) {
        --actor->yVelocity;
    }

    return (actor->ySpeed + temp + temp2) * actor->yDir;
}

void add_x_speed(unsigned char val) {
    temp = actor->xSpeedFloat;
    actor->xSpeedFloat += val;
    if (actor->xSpeedFloat > actor->maxSpeed || actor->xSpeedFloat < temp) {
        actor->xSpeedFloat = actor->maxSpeed;
    }
}

void subtract_x_speed(unsigned char val) {
    temp = actor->xSpeedFloat;
    actor->xSpeedFloat -= val;
    if (actor->xSpeedFloat < actor->minSpeed || actor->xSpeedFloat > temp) {
        actor->xSpeedFloat = actor->minSpeed;
    }
}

void add_y_speed(unsigned char val) {
    temp = actor->ySpeedFloat;
    actor->ySpeedFloat += val;
    if (actor->ySpeedFloat > actor->maxSpeed || actor->ySpeedFloat < temp) {
        actor->ySpeedFloat = actor->maxSpeed;
    }
}

void subtract_y_speed(unsigned char val) {
    temp = actor->ySpeedFloat;
    actor->ySpeedFloat -= val;
    if (actor->ySpeedFloat < actor->minSpeed || actor->ySpeedFloat > temp) {
        actor->ySpeedFloat = actor->minSpeed;
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
            is_soft_hit = TRUE;
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

char is_skull_beside() {
    return (Skull.y + 1 < paddles[pad_index].y + 6) && (Skull.y + 7 > paddles[pad_index].y + 2);
}

char is_skull_collision_paddle() {
    // Skull temp_x and temp_y already include the bbox
    return (temp_x < paddles[pad_index].x + paddles[pad_index].width + paddles[pad_index].bbox_x &&
            temp_x + Skull.width > paddles[pad_index].x + paddles[pad_index].bbox_x &&
            temp_y < paddles[pad_index].y + paddles[pad_index].height + paddles[pad_index].bbox_y &&
            temp_y + Skull.height > paddles[pad_index].y + paddles[pad_index].bbox_y);
}

// TODO Will probably have to merge with above function...
char is_paddle_collision_skull() {
    // Skull bbox is hard-coded here
    return (Skull.x + 7 > paddles[pad_index].x + paddles[pad_index].bbox_x &&
            Skull.y + 7 > paddles[pad_index].y + paddles[pad_index].bbox_y &&
            Skull.x + 1 < paddles[pad_index].x + paddles[pad_index].width + paddles[pad_index].bbox_x &&
            Skull.y + 1 < paddles[pad_index].y + paddles[pad_index].height + paddles[pad_index].bbox_y);
}

// Paddle collision with wall and skull
void move_horizontal_paddle() {
    temp_x_col = paddles[pad_index].x;
    temp_y_col = paddles[pad_index].y;

    // Check wall collision:
    if (paddles[pad_index].xDir == LEFT) {
        --temp_x_col;
        if (get_collision_type()) {
            ++temp_x_col;
        } else {
            temp_x_col = paddles[pad_index].x + get_x_speed();
        }
    } else {
        temp_x_col += paddles[pad_index].width;
        if (get_collision_type()) {
            temp_x_col = paddles[pad_index].x;
        } else {
            temp_x_col = paddles[pad_index].x + get_x_speed();
        }
    }

    // Hit the skull
    if ((pad1 & 0b00000011) && is_paddle_collision_skull()) {
        // If skull was beside the paddle when collision
        if (is_skull_beside()) {
            Skull.xVelocity = 80;
        }
    }

    // Everything's fine, update x
    paddles[pad_index].x = temp_x_col;

    // FRICTION
    if (paddles[pad_index].xSpeedFloat) {
        subtract_x_speed(16);
    }
}

void move_vertical_paddle() {
    // TODO
}

// Skull collision with paddle
void check_paddle_collision() {
    // paddle 0 is always on bottom, no need to check if skull is not in the zone
    temp = FALSE;
    if (Skull.y > 127) {
        temp = TRUE;
        pad_index = 0;
    } else if (paddle_count > 1) {
        temp = TRUE;
        pad_index = 1;
    }
    if (temp) {
        if (is_skull_collision_paddle()) {
            // horizontal paddle
            if (temp_x < paddles[pad_index].x + (paddles[pad_index].width >> 1)) {
                // We hit left side of Paddle
                if (temp_x <= paddles[pad_index].x + 4) {
                    Skull.xDir = LEFT;
                    Skull.xSpeedFloat = 140;
                    Skull.ySpeedFloat = 60;
                } else if (temp_x <= paddles[pad_index].x + 8) {
                    Skull.xDir = LEFT;
                    Skull.xSpeedFloat = 100;
                    Skull.ySpeedFloat = 100;
                } else {
                    Skull.xSpeedFloat = 60;
                    Skull.ySpeedFloat = 140;
                }
            } else {
                // Right side of Paddle
                if (temp_x >= paddles[pad_index].x + paddles[pad_index].width - 4) {
                    Skull.xDir = RIGHT;
                    Skull.xSpeedFloat = 140;
                    Skull.ySpeedFloat = 60;
                } else if (temp_x >= paddles[pad_index].x + paddles[pad_index].width - 8) {
                    Skull.xDir = RIGHT;
                    Skull.xSpeedFloat = 100;
                    Skull.ySpeedFloat = 100;
                } else {
                    Skull.xSpeedFloat = 60;
                    Skull.ySpeedFloat = 140;
                }
            }

            // Going up or down?
            if (Skull.y < paddles[pad_index].y) {
                Skull.yDir = UP;
                // if (!is_skull_beside()) {
                //     temp_y = paddles[pad_index].y - Skull.height;
                // }
            } else {
                Skull.yDir = DOWN;
                // if (!is_skull_beside()) {
                //     temp_y = paddles[pad_index].y + 4;
                // }
            }
        }
    }

    temp = FALSE;
    if (paddle_count > 2) {
        if (Skull.x < 127) {
            temp = TRUE;
            pad_index = 2;
        } else if (paddle_count > 3) {
            temp = TRUE;
            pad_index = 3;
        }
    }
    if (temp) {
        // TODO Check vertical paddles
    }
}

void check_main_input() {
    for (pad_index = 0; pad_index < paddle_count; ++pad_index) {
        actor = &paddles[pad_index];
        if (pad_index < 2) {
            // Horizontal paddle
            if (pad1 & PAD_LEFT) {
                add_x_speed(80);
                paddles[pad_index].xDir = LEFT;
            }
            if (pad1 & PAD_RIGHT) {
                add_x_speed(80);
                paddles[pad_index].xDir = RIGHT;
            }
            move_horizontal_paddle();
        } else {
            // Vertical paddle
            if (pad1 & PAD_UP) {
                add_y_speed(80);
                paddles[pad_index].yDir = UP;
            }
            if (pad1 & PAD_DOWN) {
                add_y_speed(80);
                paddles[pad_index].yDir = DOWN;
            }
            move_vertical_paddle();
        }
    }

    if (pad1 & PAD_A) {
        if (skull_launched) {
            // WHAT DOES BUTTON A DO?? WEAPON? SPEED_UP THE PADDLE?
        } else {
            skull_launched = TRUE;
        }
    }

    if (pad1 & PAD_B) {
        // WHAT DOES BUTTON B DO?? WEAPON? SPEED_UP THE PADDLE?
    }

    if (pad1 & PAD_START) {
        // TODO PAUSE!
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
        actor = &Skull;

        temp_x = Skull.x + get_x_speed();
        temp_y = Skull.y + get_y_speed();

        // bbox of skull is 1, so we increment because it's faster than addition
        ++temp_x;
        ++temp_y;

        if (Skull.xDir == RIGHT) {
            if (Skull.yDir == DOWN) {
                // Going DOWN RIGHT
                // Check down
                temp_x_col = temp_x;
                temp_y_col = temp_y + Skull.height;
                if (set_collision_data()) {
                    if (backup_col_type != COL_TYPE_SOFT) {
                        Skull.yDir = UP;
                        temp_y -= temp_y % 8;
                    }
                    do_skull_tile_collision();
                }
                temp_x_col += Skull.width;
                temp_y_col = temp_y;
                if (set_collision_data()) {
                    // Check right
                    if (backup_col_type != COL_TYPE_SOFT) {
                        Skull.xDir = LEFT;
                        temp_x -= temp_x % 8;
                    }
                    do_skull_tile_collision();
                } else {
                    // Check down-right
                    temp_y_col += Skull.height;
                    if (set_collision_data()) {
                        if (backup_col_type != COL_TYPE_SOFT) {
                            if (temp_y % 8 < COL_OFFSET && temp_x % 8 > 3) {
                                Skull.yDir = UP;
                                temp_y -= temp_y % 8;
                            } else {
                                Skull.xDir = LEFT;
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
                        Skull.yDir = DOWN;
                        temp_y += 8;
                        temp_y -= temp_y % 8;
                    }
                    do_skull_tile_collision();
                }
                // Check right
                temp_x_col = temp_x + Skull.width;
                temp_y_col += Skull.height;
                if (set_collision_data()) {
                    if (backup_col_type != COL_TYPE_SOFT) {
                        Skull.xDir = LEFT;
                        temp_x -= temp_x % 8;
                    }
                    do_skull_tile_collision();
                } else {
                    // Check up right
                    temp_y_col = temp_y;
                    if (set_collision_data()) {
                        if (backup_col_type != COL_TYPE_SOFT) {
                            if (temp_y % 8 < COL_OFFSET && temp_x % 8 < 3) {
                                Skull.xDir = LEFT;
                                temp_x -= temp_x % 8;
                            } else {
                                Skull.yDir = DOWN;
                                temp_y += 8;
                                temp_y -= temp_y % 8;
                            }
                        }
                        do_skull_tile_collision();
                    }
                }
            }
        } else {
            if (Skull.yDir == DOWN) {
                // Going DOWN LEFT
                // Check down
                temp_x_col = temp_x + Skull.width;
                temp_y_col = temp_y + Skull.height;
                if (set_collision_data()) {
                    if (backup_col_type != COL_TYPE_SOFT) {
                        Skull.yDir = UP;
                        temp_y -= temp_y % 8;
                    }
                    do_skull_tile_collision();
                }
                // Check left
                temp_x_col = temp_x;
                temp_y_col = temp_y;
                if (set_collision_data()) {
                    if (backup_col_type != COL_TYPE_SOFT) {
                        Skull.xDir = RIGHT;
                        temp_x += 8;
                        temp_x -= temp_x % 8;
                    }
                    do_skull_tile_collision();
                } else {
                    // Check down-left
                    temp_x_col = temp_x;
                    temp_y_col += Skull.height;
                    if (set_collision_data()) {
                        if (backup_col_type != COL_TYPE_SOFT) {
                            if (temp_y % 8 < COL_OFFSET && temp_x % 8 > 3) {
                                Skull.yDir = UP;
                                temp_y -= temp_y % 8;
                            } else {
                                Skull.xDir = RIGHT;
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
                temp_x_col = temp_x + Skull.width;
                temp_y_col = temp_y;
                if (set_collision_data()) {
                    if (backup_col_type != COL_TYPE_SOFT) {
                        Skull.yDir = DOWN;
                        temp_y += 8;
                        temp_y -= temp_y % 8;
                    }
                    do_skull_tile_collision();
                }
                // Check left
                temp_x_col = temp_x;
                temp_y_col = temp_y + Skull.height;
                if (set_collision_data()) {
                    if (backup_col_type != COL_TYPE_SOFT) {
                        Skull.xDir = RIGHT;
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
                                Skull.xDir = RIGHT;
                                temp_x += 8;
                                temp_x -= temp_x % 8;

                            } else {
                                Skull.yDir = DOWN;
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
        temp_x = paddles[0].x + (paddles[0].width >> 1) - (Skull.width >> 1);
        temp_y = paddles[0].y + paddles[0].bbox_y - Skull.height - Skull.bbox_x;
    }

    // All is fine, we can update x and y
    Skull.x = temp_x;
    Skull.y = temp_y;
}

void draw_sprites(void) {
    // clear all sprites from sprite buffer
    oam_clear();

    // draw paddles
    for (i = 0; i < paddle_count; ++i) {
        if (i < 2) {
            oam_meta_spr(paddles[i].x, paddles[i].y, HorizontalPaddleSpr);
        } else {
            oam_meta_spr(paddles[i].x, paddles[i].y, VerticalPaddleSpr);
        }
    }
    oam_spr(Skull.x, Skull.y, 0x00, 0b00000000);
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
            gray_line();

            // game_loop();

            // if(game_over) ++current_level; else show_game_over();
        }

        // show_screen(!game_lives?SCREEN_GAMEOVER:SCREEN_WELLDONE);//show
        // game results
    }
}
