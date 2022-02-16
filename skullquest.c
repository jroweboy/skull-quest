#include "LIB/nesdoug.h"
#include "LIB/neslib.h"

// LEVEL NAMETABLES AND COLLISION
#include "Collision/master_collision.h"
#include "Nametable/Forest/level01.h"
#include "Nametable/title_screen.h"
#include "sprites.h"

#define LEVEL_TOTAL 1
#define MAX_HEALTH 8
#define PADDLE_MAX_SPEED 7
#define SPEED_STEP 1
#define MIN_SPEED 64
#define MAX_SPEED 250
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

const unsigned char pal_spr_01[16] = {
    0x0f, 0x20, 0x15, 0x12, 0x0f, 0x11, 0x22, 0x32,
    0x0f, 0x13, 0x23, 0x33, 0x0f, 0x14, 0x24, 0x34  //
};

const unsigned char* const level_list[LEVEL_TOTAL * 3] = {
    forest_level_01, forest_col_01, pal_forest_bg  // LEVEL 01
};

unsigned char speed_float;
signed char dir, xCollisionDir, yCollisionDir;

unsigned char exp_1, exp_10, exp_100, exp_1000, exp_10000, exp_100000;

unsigned char debug1, debug2;
unsigned char pad1;
unsigned char pad1_new;
unsigned char c_map[368];

int collision_index, nametable_index, backup_col_index, backup_nt_index;
unsigned char collision_mask, backup_col_mask, stuck_times;
unsigned char i, j, temp, temp_x, temp_y, collision_type, backup_col_type, skull_launched;
unsigned char p1_health;
unsigned char p1_max_health;
unsigned char game_state, current_level, is_soft_hit;

// Global Pointers
unsigned char* ptr_speed_float;
signed char *ptr_skull_dir, ptr_skull_speed;

struct Actor {
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
};

struct Actor Paddle = {
    0x75,  // x
    0xCF,  // y
    0x20,  // width
    4,     // height
    0,     // bbox_x
    2,     // bbox_y
    0,     // xSpeed
    0,     // ySpeed
    0,     // xDir
    0,     // yDir
    0,     // xSpeedFloat
    0      // ySpeedFloat
};

struct Actor Skull = {
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
    100     // ySpeedFloat
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

void show_HUD() {
    vram_adr(0x23C0);
    vram_fill(0x00, 8);
    // HEALTH
    update_health();

    // XP
    // TODO update_xp();

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
    is_soft_hit = FALSE;
    // Play sound
    // Update score
    // Spawn random coin ??
}

void add_x_speed(unsigned char val) {
    temp = Skull.xSpeedFloat;
    Skull.xSpeedFloat += val;
    if (Skull.xSpeedFloat > MAX_SPEED || Skull.xSpeedFloat < temp) {
        Skull.xSpeedFloat = MAX_SPEED;
    }
    Skull.xSpeed = (Skull.xSpeedFloat >> 7) * Skull.xDir;
}

void subtract_x_speed(unsigned char val) {
    temp = Skull.xSpeedFloat;
    Skull.xSpeedFloat -= val;
    if (Skull.xSpeedFloat < MIN_SPEED || Skull.xSpeedFloat > temp) {
        Skull.xSpeedFloat = MIN_SPEED;
    }
    Skull.xSpeed = (Skull.xSpeedFloat >> 7) * Skull.xDir;
}

void add_y_speed(unsigned char val) {
    temp = Skull.ySpeedFloat;
    Skull.ySpeedFloat += val;
    if (Skull.ySpeedFloat > MAX_SPEED || Skull.ySpeedFloat < temp) {
        Skull.ySpeedFloat = MAX_SPEED;
    }
    Skull.ySpeed = (Skull.ySpeedFloat >> 7) * Skull.yDir;
}

void subtract_y_speed(unsigned char val) {
    temp = Skull.ySpeedFloat;
    Skull.ySpeedFloat -= val;
    if (Skull.ySpeedFloat < MIN_SPEED || Skull.ySpeedFloat > temp) {
        Skull.ySpeedFloat = MIN_SPEED;
    }
    Skull.ySpeed = (Skull.ySpeedFloat >> 7) * Skull.yDir;
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

void do_tile_collision() {
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

char is_paddle_skull_collision() {
    return (temp_x < Paddle.x + Paddle.width &&
            temp_x + Skull.width > Paddle.x &&
            temp_y < Paddle.y + Paddle.height + Paddle.bbox_y &&
            temp_y + Skull.height > Paddle.y + Paddle.bbox_y);
}

// Paddle collision with wall
void check_paddle_input() {
    if (!is_paddle_skull_collision()) {
        Paddle.xSpeedFloat += 128;
        temp = Paddle.xSpeedFloat ? 1 : 2;
        if ((pad1 & PAD_LEFT) && (Paddle.xSpeed > -PADDLE_MAX_SPEED)) {
            temp_x = Paddle.xSpeed -= temp;
        }
        if ((pad1 & PAD_RIGHT) && (Paddle.xSpeed < PADDLE_MAX_SPEED)) {
            Paddle.xSpeed += temp;
        }

        temp_x = Paddle.x + Paddle.xSpeed;  // Bounding box?
        temp_y = Paddle.y;                  // Bounding box?

        // Check wall collision:
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
}

// Skull collision with paddle
void check_paddle_collision() {
    if (is_paddle_skull_collision()) {
        // If skull is beside the paddle (Skull.x & Skull.y hasn't been updated yet)
        if (Skull.y + Skull.bbox_y < Paddle.y + Paddle.height + Paddle.bbox_y && Skull.y + Skull.bbox_y + Skull.height > Paddle.y + Paddle.bbox_y) {
            add_x_speed(100);
            subtract_y_speed(100);
            if (temp_x < Paddle.x + (Paddle.width >> 1)) {
                // LEFT side
                xCollisionDir = RIGHT;
                temp_x = Paddle.x - Skull.width;
                while (set_collision_data(temp_x, temp_y)) {
                    ++temp_x;
                    ++Paddle.x;
                }
            } else {
                // RIGHT side
                xCollisionDir = LEFT;
                while (set_collision_data(temp_x + Skull.width, temp_y)) {
                    temp_x = Paddle.x + Paddle.width;
                    --temp_x;
                    --Paddle.x;
                }
            }
        } else {
            // Skull not beside
            if (temp_x < Paddle.x + (Paddle.width >> 1)) {
                // We hit left side of Paddle
                if (Skull.xDir == RIGHT) {
                    xCollisionDir = RIGHT;
                }
                if (temp_x < Paddle.x + 2) {
                    add_x_speed(96);
                    subtract_y_speed(96);
                } else if (!(temp_x < Paddle.x + 8)) {
                    subtract_x_speed(64);
                    add_y_speed(64);
                }
            } else {
                // Right side of Paddle
                if (Skull.xDir == LEFT) {
                    xCollisionDir = LEFT;
                }
                if (temp_x > Paddle.x + Paddle.width - 2) {
                    add_x_speed(96);
                    subtract_y_speed(96);
                } else if (!(temp_x > Paddle.x + Paddle.width - 8)) {
                    subtract_x_speed(64);
                    add_y_speed(64);
                }
            }

            if (temp_y < Paddle.y + Paddle.bbox_y + (Paddle.height >> 1)) {
                yCollisionDir = DOWN;
                while (is_paddle_skull_collision()) {
                    --temp_y;
                    if (set_collision_data(temp_x, temp_y)) {
                        ++temp_y;
                        temp_x < Paddle.x + (Paddle.width >> 1) ? --temp_x : ++temp_x;
                    }
                }
            } else {
                yCollisionDir = UP;
                while (is_paddle_skull_collision()) {
                    ++temp_y;
                    if (set_collision_data(temp_x, temp_y)) {
                        --temp_y;
                        temp_x < Paddle.x + (Paddle.width >> 1) ? --temp_x : ++temp_x;
                    }
                }
            }
        }
    }
}

void check_main_input() {
    check_paddle_input();

    if (pad1 & PAD_A) {
        if (skull_launched) {
            // WHAT DOES BUTTON A DO?? WEAPON? SPEED_UP THE PADDLE?
            add_x_speed(SPEED_STEP);
        } else {
            skull_launched = TRUE;
            stuck_times = 0;
        }
    }
    if (pad1 & PAD_B) {
        // WHAT DOES BUTTON B DO?? WEAPON? SPEED_UP THE PADDLE?
        subtract_x_speed(SPEED_STEP);
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

    /// TEMPORARY TO TEST SPEED ///////////
    if (pad1 & PAD_UP) {
        add_y_speed(SPEED_STEP);
    }
    if (pad1 & PAD_DOWN) {
        subtract_y_speed(SPEED_STEP);
    }
    ///////////////////////////////////////
}

// not really decimal.... I know it's a mess...
signed char get_x_speed() {
    Skull.xRemain += Skull.xSpeedFloat & 0b01111111;  // MODULO 128
    if (Skull.xRemain > 127) {
        Skull.xRemain &= 0b01111111;
        return Skull.xDir;
    }
}

signed char get_y_speed() {
    Skull.yRemain += Skull.ySpeedFloat & 0b01111111;  // MODULO 128
    if (Skull.yRemain > 127) {
        Skull.yRemain &= 0b01111111;
        return Skull.yDir;
    }
}

void update_skull() {
    // TODO: We should probably check to make sure skull is within boundaries

    xCollisionDir = NONE;
    yCollisionDir = NONE;
    backup_col_type = NONE;
    is_soft_hit = FALSE;

    if (skull_launched) {
        temp_x = Skull.x + Skull.xSpeed + get_x_speed();
        temp_y = Skull.y + Skull.ySpeed + get_y_speed();

        // bbox of skull is 1, so we increment because it's faster
        ++temp_x;
        ++temp_y;

        if (Skull.xDir == RIGHT) {
            if (Skull.yDir == DOWN) {
                // Check down
                if (set_collision_data(temp_x, temp_y + Skull.height)) {
                    yCollisionDir = DOWN;
                    backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (set_collision_data(temp_x, temp_y + Skull.height)) {
                            --temp_y;
                        }
                    }
                    do_tile_collision();
                }
                // Check right OR down-right
                if (set_collision_data(temp_x + Skull.width, temp_y) || (backup_col_type == 0 && set_collision_data(temp_x + Skull.width, temp_y + Skull.height))) {
                    xCollisionDir = RIGHT;
                    backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (set_collision_data(temp_x + Skull.width, temp_y)) {
                            --temp_x;
                        }
                    }
                    do_tile_collision();
                }
            } else {
                // Check up
                if (set_collision_data(temp_x, temp_y)) {
                    yCollisionDir = UP;
                    backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (set_collision_data(temp_x, temp_y)) {
                            ++temp_y;
                        }
                    }
                    do_tile_collision();
                }
                // Check right OR up-right
                if (set_collision_data(temp_x + Skull.width, temp_y + Skull.height) || (backup_col_type == 0 && set_collision_data(temp_x + Skull.width, temp_y))) {
                    xCollisionDir = RIGHT;
                    backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (set_collision_data(temp_x + Skull.width, temp_y + Skull.height)) {
                            --temp_x;
                        }
                    }
                    do_tile_collision();
                }
            }
        } else {
            if (Skull.yDir == DOWN) {
                // Check down
                if (set_collision_data(temp_x + Skull.width, temp_y + Skull.height)) {
                    yCollisionDir = DOWN;
                    backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (set_collision_data(temp_x + Skull.width, temp_y + Skull.height)) {
                            --temp_y;
                        }
                    }
                    do_tile_collision();
                }
                // Check left OR down-left
                if (set_collision_data(temp_x, temp_y) || (backup_col_type == 0 && set_collision_data(temp_x, temp_y + Skull.height))) {
                    xCollisionDir = LEFT;
                    backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (set_collision_data(temp_x, temp_y)) {
                            ++temp_x;
                        }
                    }
                    do_tile_collision();
                }
            } else {
                // Check up
                if (set_collision_data(temp_x + Skull.width, temp_y)) {
                    yCollisionDir = UP;
                    backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (set_collision_data(temp_x + Skull.width, temp_y)) {
                            ++temp_y;
                        }
                    }
                    do_tile_collision();
                }
                // Check left OR up-left
                if (set_collision_data(temp_x, temp_y + Skull.height) || (backup_col_type == 0 && set_collision_data(temp_x, temp_y))) {
                    xCollisionDir = LEFT;
                    backup_collision_info();
                    if (backup_col_type != COL_TYPE_SOFT) {
                        while (set_collision_data(temp_x, temp_y + Skull.height)) {
                            ++temp_x;
                        }
                    }
                    do_tile_collision();
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
        temp_x = Paddle.x + (Paddle.width >> 1) - (Skull.width >> 1);
        temp_y = Paddle.y + Paddle.bbox_y - Skull.height - Skull.bbox_x;
    }

    // All is fine, we can update speed
    Skull.x = temp_x;
    Skull.y = temp_y;
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
