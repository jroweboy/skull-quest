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
#define FALSE 0
#define TRUE 1
#define TILE_BACK 0x10
#define TILE_BACK_GRASS 0x23
#define TILE_HEART_FULL 0x5B
#define TILE_HEART_EMPTY 0x5C
#define COL_TYPE_SOFT 0x06

#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1
#define NONE 0

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

signed char xCollisionDir, yCollisionDir;

char exp[] = "00000000";

unsigned char debug1, debug2;
unsigned char pad1;
unsigned char pad1_new;
unsigned char c_map[368];

int collision_index, backup_col_index, backup_nt_index;
unsigned char collision_mask;
unsigned char i, j, temp, temp2, temp_x, temp_y, backup_col_type, skull_launched;
unsigned char p1_health;
unsigned char p1_max_health;
unsigned char game_state, current_level, is_soft_hit, paddle_count;

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

enum GameStates { TITLE,
                  MAIN,
                  STORY,
                  PAUSE,
                  GAME_OVER };

enum ActorStates { IDLE,
                   WALKING,
                   ATTACKING };

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

// not really decimal.... I know it's a mess...
signed char get_x_speed(Actor* actor) {
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

signed char get_y_speed(Actor* actor) {
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

void add_x_speed(unsigned char val, Actor* actor) {
    temp = actor->xSpeedFloat;
    actor->xSpeedFloat += val;
    if (actor->xSpeedFloat > actor->maxSpeed || actor->xSpeedFloat < temp) {
        actor->xSpeedFloat = actor->maxSpeed;
    }
}

void subtract_x_speed(unsigned char val, Actor* actor) {
    temp = actor->xSpeedFloat;
    actor->xSpeedFloat -= val;
    if (actor->xSpeedFloat < actor->minSpeed || actor->xSpeedFloat > temp) {
        actor->xSpeedFloat = actor->minSpeed;
    }
}

void add_y_speed(unsigned char val, Actor* actor) {
    temp = actor->ySpeedFloat;
    actor->ySpeedFloat += val;
    if (actor->ySpeedFloat > actor->maxSpeed || actor->ySpeedFloat < temp) {
        actor->ySpeedFloat = actor->maxSpeed;
    }
}

void subtract_y_speed(unsigned char val, Actor* actor) {
    temp = actor->ySpeedFloat;
    actor->ySpeedFloat -= val;
    if (actor->ySpeedFloat < actor->minSpeed || actor->ySpeedFloat > temp) {
        actor->ySpeedFloat = actor->minSpeed;
    }
}

char get_collision_type(char param_x, char param_y) {
    collision_index = (param_x >> 4) + (((param_y >> 3) - 5) * 16);
    return (param_x >> 3) % 2 ? c_map[collision_index] & 0x0F : c_map[collision_index] >> 4;
}

char set_collision_data(char param_x, char param_y) {
    backup_nt_index = NTADR_A((param_x >> 3), (param_y >> 3));
    backup_col_type = get_collision_type(param_x, param_y);
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

char is_skull_collision_paddle(Actor* paddle) {
    // Skull temp_x and temp_y already include the skull bbox
    return (temp_x < paddle->x + paddle->width + paddle->bbox_x &&
            temp_x + Skull.width > paddle->x + paddle->bbox_x &&
            temp_y < paddle->y + paddle->height + paddle->bbox_y &&
            temp_y + Skull.height > paddle->y + paddle->bbox_y);
}

// TODO Will probably have to merge with above function...
char is_paddle_collision_skull(Actor* paddle) {
    // Skull bbox is hard-coded here
    return (Skull.x + 7 > paddle->x + paddle->bbox_x &&
            Skull.y + 7 > paddle->y + paddle->bbox_y &&
            Skull.x + 1 < paddle->x + paddle->width + paddle->bbox_x &&
            Skull.y + 1 < paddle->y + paddle->height + paddle->bbox_y);
}

// Paddle collision with wall and skull
void move_paddle(Actor* paddle) {
    temp_x = paddle->x + get_x_speed(paddle);
    temp_y = paddle->y + get_y_speed(paddle);

    // Check wall collision:
    if (paddle->xDir == LEFT) {
        while (get_collision_type(temp_x, temp_y)) {
            ++temp_x;
        }
    } else {
        while (get_collision_type(temp_x + paddle->width, temp_y)) {
            --temp_x;
        }
    }
    if (paddle->yDir == UP) {
        while (get_collision_type(temp_x, temp_y)) {
            ++temp_y;
        }
    } else {
        while (get_collision_type(temp_x, temp_y + paddle->height)) {
            --temp_y;
        }
    }

    // Everything's fine, update x, y
    paddle->x = temp_x;
    paddle->y = temp_y;

    if (pad1 & 0b00001111 && is_paddle_collision_skull(paddle)) {
        // Collision with skull
        Skull.xVelocity = 80;
    }

    // FRICTION
    if (paddle->xSpeedFloat > 0) {
        subtract_x_speed(16, paddle);
    }
    if (paddle->ySpeedFloat > 0) {
        subtract_y_speed(16, paddle);
    }
}

// Skull collision with paddle
void check_paddle_collision() {
    for (i = 0; i < paddle_count; ++i) {
        if (i < 2) {
            // horizontal paddle
            if (is_skull_collision_paddle(&paddles[i])) {
                // If skull was beside the paddle when collision (Skull.x & Skull.y hasn't been updated yet)
                if (Skull.y + Skull.bbox_y < paddles[i].y + paddles[i].height + paddles[i].bbox_y && Skull.y + Skull.bbox_y + Skull.height > paddles[i].y + paddles[i].bbox_y) {
                    Skull.xSpeedFloat = 150;
                    Skull.ySpeedFloat = 50;
                    if (temp_x < paddles[i].x + (paddles[i].width >> 1)) {
                        // LEFT side
                        xCollisionDir = RIGHT;
                        temp_x = paddles[i].x - Skull.width;
                        while (get_collision_type(temp_x, temp_y)) {
                            ++temp_x;
                            ++paddles[i].x;
                        }
                    } else {
                        // RIGHT side
                        xCollisionDir = LEFT;
                        temp_x = paddles[i].x + paddles[i].width;
                        while (get_collision_type(temp_x + Skull.width - 1, temp_y)) {
                            --temp_x;
                            --paddles[i].x;
                        }
                    }
                    xCollisionDir = DOWN;
                } else {
                    // Skull not beside
                    if (temp_x < paddles[i].x + (paddles[i].width >> 1)) {
                        // We hit left side of Paddle
                        if (temp_x <= paddles[i].x + 4) {
                            if (Skull.xDir == RIGHT) {
                                xCollisionDir = RIGHT;
                            }
                            Skull.xSpeedFloat = 140;
                            Skull.ySpeedFloat = 60;
                        } else if (temp_x <= paddles[i].x + 8) {
                            if (Skull.xDir == RIGHT) {
                                xCollisionDir = RIGHT;
                            }
                            Skull.xSpeedFloat = 100;
                            Skull.ySpeedFloat = 100;
                        } else {
                            Skull.xSpeedFloat = 60;
                            Skull.ySpeedFloat = 140;
                        }

                    } else {
                        // Right side of Paddle
                        if (temp_x >= paddles[i].x + paddles[i].width - 4) {
                            if (Skull.xDir == LEFT) {
                                xCollisionDir = LEFT;
                            }
                            Skull.xSpeedFloat = 140;
                            Skull.ySpeedFloat = 60;
                        } else if (temp_x >= paddles[i].x + paddles[i].width - 8) {
                            if (Skull.xDir == LEFT) {
                                xCollisionDir = LEFT;
                            }
                            Skull.xSpeedFloat = 100;
                            Skull.ySpeedFloat = 100;
                        } else {
                            Skull.xSpeedFloat = 60;
                            Skull.ySpeedFloat = 140;
                        }
                    }

                    if (temp_y < paddles[i].y + paddles[i].bbox_y + (paddles[i].height >> 1)) {
                        yCollisionDir = DOWN;
                        while (is_skull_collision_paddle(&paddles[i])) {
                            --temp_y;
                            if (get_collision_type(temp_x, temp_y)) {
                                ++temp_y;
                                temp_x < paddles[i].x + (paddles[i].width >> 1) ? --temp_x : ++temp_x;
                            }
                        }
                    } else {
                        yCollisionDir = UP;
                        while (is_skull_collision_paddle(&paddles[i])) {
                            ++temp_y;
                            if (get_collision_type(temp_x, temp_y)) {
                                --temp_y;
                                temp_x < paddles[i].x + (paddles[i].width >> 1) ? --temp_x : ++temp_x;
                            }
                        }
                    }
                }
            }

        } else {
            // vertical paddle
        }
    }
}

void check_main_input() {
    for (i = 0; i < paddle_count; ++i) {
        if (!is_paddle_collision_skull(&paddles[i])) {
            if (i < 2) {
                // Horizontal paddle
                if (pad1 & PAD_LEFT) {
                    add_x_speed(80, &paddles[i]);
                    paddles[i].xDir = LEFT;
                }
                if (pad1 & PAD_RIGHT) {
                    add_x_speed(80, &paddles[i]);
                    paddles[i].xDir = RIGHT;
                }
            } else {
                // Vertical paddle
                if (pad1 & PAD_UP) {
                    add_y_speed(80, &paddles[i]);
                    paddles[i].yDir = UP;
                }
                if (pad1 & PAD_DOWN) {
                    add_y_speed(80, &paddles[i]);
                    paddles[i].yDir = DOWN;
                }
            }
            
            move_paddle(&paddles[i]);
        }
    }

    if (pad1 & PAD_A) {
        if (skull_launched) {
            // WHAT DOES BUTTON A DO?? WEAPON? SPEED_UP THE PADDLE?
            add_x_speed(SPEED_STEP, &Skull);
        } else {
            skull_launched = TRUE;
            // stuck_times = 0;
        }
    }

    if (pad1 & PAD_B) {
        // WHAT DOES BUTTON B DO?? WEAPON? SPEED_UP THE PADDLE?
        subtract_x_speed(SPEED_STEP, &Skull);
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

    xCollisionDir = NONE;
    yCollisionDir = NONE;
    backup_col_type = NONE;
    is_soft_hit = FALSE;

    if (skull_launched) {
        temp_x = Skull.x + get_x_speed(&Skull);
        temp_y = Skull.y + get_y_speed(&Skull);

        // bbox of skull is 1, so we increment because it's faster than addition
        ++temp_x;
        ++temp_y;

        if (Skull.xDir == RIGHT) {
            if (Skull.yDir == DOWN) {
                // Check down
                if (set_collision_data(temp_x, temp_y + Skull.height)) {
                    yCollisionDir = DOWN;
                    // backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (get_collision_type(temp_x, temp_y + Skull.height)) {
                            --temp_y;
                        }
                    }
                    do_skull_tile_collision();
                }
                // Check right OR down-right
                if (set_collision_data(temp_x + Skull.width, temp_y) || (backup_col_type == 0 && set_collision_data(temp_x + Skull.width, temp_y + Skull.height))) {
                    xCollisionDir = RIGHT;
                    // backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (get_collision_type(temp_x + Skull.width, temp_y)) {
                            --temp_x;
                        }
                    }
                    do_skull_tile_collision();
                }
            } else {
                // Check up
                if (set_collision_data(temp_x, temp_y)) {
                    yCollisionDir = UP;
                    // backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (get_collision_type(temp_x, temp_y)) {
                            ++temp_y;
                        }
                    }
                    do_skull_tile_collision();
                }
                // Check right OR up-right
                if (set_collision_data(temp_x + Skull.width, temp_y + Skull.height) || (backup_col_type == 0 && set_collision_data(temp_x + Skull.width, temp_y))) {
                    xCollisionDir = RIGHT;
                    // backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (get_collision_type(temp_x + Skull.width, temp_y + Skull.height)) {
                            --temp_x;
                        }
                    }
                    do_skull_tile_collision();
                }
            }
        } else {
            if (Skull.yDir == DOWN) {
                // Check down
                if (set_collision_data(temp_x + Skull.width, temp_y + Skull.height)) {
                    yCollisionDir = DOWN;
                    // backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (get_collision_type(temp_x + Skull.width, temp_y + Skull.height)) {
                            --temp_y;
                        }
                    }
                    do_skull_tile_collision();
                }
                // Check left OR down-left
                if (set_collision_data(temp_x, temp_y) || (backup_col_type == 0 && set_collision_data(temp_x, temp_y + Skull.height))) {
                    xCollisionDir = LEFT;
                    // backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (get_collision_type(temp_x, temp_y)) {
                            ++temp_x;
                        }
                    }
                    do_skull_tile_collision();
                }
            } else {
                // Check up
                if (set_collision_data(temp_x + Skull.width, temp_y)) {
                    yCollisionDir = UP;
                    // backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (get_collision_type(temp_x + Skull.width, temp_y)) {
                            ++temp_y;
                        }
                    }
                    do_skull_tile_collision();
                }
                // Check left OR up-left
                if (set_collision_data(temp_x, temp_y + Skull.height) || (backup_col_type == 0 && set_collision_data(temp_x, temp_y))) {
                    xCollisionDir = LEFT;
                    // backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (get_collision_type(temp_x, temp_y + Skull.height)) {
                            ++temp_x;
                        }
                    }
                    do_skull_tile_collision();
                }
            }
        }

        check_paddle_collision();

        // Don't change direction if soft collision
        if (backup_col_type != COL_TYPE_SOFT) {
            if (xCollisionDir) {
                Skull.xSpeed = -Skull.xSpeed;
                Skull.xDir = -Skull.xDir;
            }
            if (yCollisionDir) {
                Skull.ySpeed = -Skull.ySpeed;
                Skull.yDir = -Skull.yDir;
            }
        }

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

            // game_loop();

            // if(game_over) ++current_level; else show_game_over();
        }

        // show_screen(!game_lives?SCREEN_GAMEOVER:SCREEN_WELLDONE);//show
        // game results
    }
}
