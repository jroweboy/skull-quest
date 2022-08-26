#include "skullquest.h"

#include "LIB/nesdoug.h"
#include "LIB/neslib.h"
#include "MMC3/mmc3_code.h"

// Dialog
#include "dialog.h"
#include "Nametable/black_level.h"

// PALETTES
#pragma rodata-name(push, "CODE")
#include "palettes.h"
#pragma rodata-name(pop)



#pragma bss-name(push, "ZEROPAGE")
static unsigned char pad1;
static unsigned char pad1_new;

static unsigned char pad_index, temp_y_col, temp_x_col, chr_4_index, chr_5_index;
static unsigned char i, j, draw_index, param1, param2, param3, param4, temp, temp2, temp3, is_first, temp_speed, temp_x, temp_y, backup_col_type, skull_launched;
static unsigned char p1_health, p1_max_health, brick_counter, wait_timer, level_condition1, level_condition2;
static unsigned char level_bank;
static unsigned char game_state, current_level, paddle_count, story_step, story_counter;
static unsigned char animation_index, frame_count, show_face, show_item, current_item, current_selection, exit_inventory = FALSE;
static unsigned char map_x, map_y, map_lvl_name_x, map_lvl_name_y;
static unsigned char brightness = 4;
static unsigned char NECROMANCER, GHOST, LIGHTNING, DEVIL, SKELETON1, SKELETON2, DOOR1, DOOR2, DOOR3, STARS;
static unsigned char CROW, GATE, CRATE1, CRATE2, CRATE3, HERO, STILL_DECORATION, SORCERER, TRIGGER, TRIGGER2;
static unsigned char SPECIAL_ITEM;

unsigned char const** animation_array;

static const unsigned char paddle_hit_zones[] = {
        3, 6, 11, 14, // Short paddle
        4, 10, 22, 28 // Long paddle
};

static int collision_index, backup_col_index, backup_nt_index, scroll_index_y;

#pragma bss-name(pop)

#pragma bss-name(push, "XRAM")
// extra RAM at $6000-$7fff
unsigned char wram_array[0x1500];
#pragma bss-name(pop)

static char exp[] = "00000000";
static unsigned char c_map[368];

const unsigned char* current_nametable;
const unsigned char* current_collision_map;
const unsigned char* current_background_palette;
const char* current_sprite_palette;

#define ITEM_NUMBER 6
typedef struct {
    unsigned char type[ITEM_NUMBER];
    unsigned char is_active[ITEM_NUMBER];
    unsigned char sprite[ITEM_NUMBER];
} Item;

Item items;

#define ACTOR_NUMBER 14

typedef struct
{
    unsigned char x[ACTOR_NUMBER];
    unsigned char y[ACTOR_NUMBER];
    unsigned char width[ACTOR_NUMBER];
    unsigned char height[ACTOR_NUMBER];
    unsigned char bbox_x[ACTOR_NUMBER];
    unsigned char bbox_y[ACTOR_NUMBER];
    signed char xDir[ACTOR_NUMBER];
    signed char yDir[ACTOR_NUMBER];
    unsigned char xSpeed[ACTOR_NUMBER];
    unsigned char ySpeed[ACTOR_NUMBER];
    unsigned char xRemain[ACTOR_NUMBER];
    unsigned char yRemain[ACTOR_NUMBER];
    unsigned char xVelocity[ACTOR_NUMBER];
    unsigned char yVelocity[ACTOR_NUMBER];
    unsigned char minSpeed[ACTOR_NUMBER];
    unsigned char maxSpeed[ACTOR_NUMBER];
    unsigned char counter[ACTOR_NUMBER];          // Animation timer
    unsigned char animation_delay[ACTOR_NUMBER];  // Time to wait between frames
    unsigned char current_frame[ACTOR_NUMBER];
    unsigned char state[ACTOR_NUMBER];
    unsigned char type[ACTOR_NUMBER];
    unsigned char has_collision[ACTOR_NUMBER]; // Collision with skull
} Actors;

// index 0-3 paddles
// index 4 skull
// index 5 inventory cursor

Actors actors;

#pragma rodata-name("BANK0")
#pragma code-name("BANK0")
#include "bank0.h"

#pragma rodata-name("BANK1")
#pragma code-name("BANK1")
#include "bank1.h"

#pragma rodata-name("BANK2")
#pragma code-name("BANK2")
#include "bank2.h"

#pragma rodata-name("BANK3")
#pragma code-name("BANK3")
#include "bank3.h"

#pragma rodata-name("BANK4")
#pragma code-name("BANK4")
#include "bank4.h"

#pragma rodata-name("BANK5")
#pragma code-name("BANK5")
#include "bank5.h"

#pragma rodata-name("BANK6")
#pragma code-name("BANK6")
#include "bank6.h"

#pragma rodata-name("BANK7")
#pragma code-name("BANK7")
#include "bank7.h"

#pragma rodata-name("BANK8")
#pragma code-name("BANK8")
#include "bank8.h"

#pragma rodata-name("BANK9")
#pragma code-name("BANK9")
#include "bank9.h"

#pragma rodata-name("BANK10")
#pragma code-name("BANK10")
#include "bank10.h"

#pragma rodata-name("BANK11")
#pragma code-name("BANK11")
#include "bank11.h"

#pragma rodata-name("BANK12")
#pragma code-name("BANK12")
#include "bank12.h"

#pragma rodata-name("CODE")
#pragma code-name("CODE")

void debug(unsigned char value) {
    one_vram_buffer(value, NTADR_A(1, 1));
}

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

void show_HUD() {
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

    // COINS
    // TODO update_coins();

    // OTHER??
}

void hide_HUD() {
    // Health
    for (i = 0; i < p1_max_health; ++i) {
        one_vram_buffer(0x00, NTADR_A(i + 2, 2));
    }
    // ppu_wait_nmi();
    // Exp
    multi_vram_buffer_horz(empty_line, sizeof(empty_line), NTADR_A(7, 2));
    
    // ppu_wait_nmi();
    
    // ITEM BOX:
    one_vram_buffer(0x00, NTADR_A(22, 2));
    one_vram_buffer(0x00, NTADR_A(23, 2));
    one_vram_buffer(0x00, NTADR_A(24, 2));
    one_vram_buffer(0x00, NTADR_A(23, 3));
    one_vram_buffer(0x00, NTADR_A(24, 3));
    ppu_wait_nmi();
}

void show_game_over() {
    // TODO
}

void load_black_level() {
    ppu_off();
    vram_adr(NAMETABLE_A);
    vram_unrle(black_level);
    pal_bg(pal_cemetery_bg);
    ppu_on_all();
}

void load_level() {
    ppu_off();
    // set_scroll_x(0x0000);

    // banked_call(1, reset_actors);
    reset_actors();
    // banked_call(6, init_level_specifics);
    init_level_specifics();
    // banked_call(0, init_skull);
    init_skull();

    // How many paddles?
    paddle_count = NULL;
    for (i = 0; i < 4; ++i){
        if (actors.state[i] < INACTIVE) {
            ++paddle_count;
        }
    }
    
    // banked_call(0, init_paddles);
    init_paddles();
    
    // bank_push(level_bank);
    // vram_adr(NAMETABLE_A);
    // vram_unrle(current_nametable);
    // bank_pop();
    RUN_WITH_BANK(GET_BANK(cemetery), {
        vram_adr(NAMETABLE_A);
        vram_unrle(current_nametable);
    });
    
    pal_bg(current_background_palette);
    pal_spr(current_sprite_palette);

    load_collision();

    // Check how many tombstones or destructible bricks there are
    brick_counter = 0;
    for (collision_index = 0; collision_index < 368; ++collision_index) {
        // collision_index contient 2 info de 4 bits
        // First 4 bits:
        temp = c_map[collision_index] >> 4;
        if (temp > 2 && temp < 11) {
            ++brick_counter;
        }
        // Last 4 bits:
        temp = c_map[collision_index] & 0x0F;
        if (temp > 2 && temp < 11) {
            ++brick_counter;
        }
    }

    set_chr_mode_4(chr_4_index);
    set_chr_mode_5(chr_5_index);

    skull_launched = FALSE;
    if (game_state == MAIN) {
        show_HUD();
    }

    pal_bright(0);

    ppu_on_all();
}

void load_title_screen() {
    // Load graphics
    set_chr_mode_1(0x0C);  // Sprite staff & lightning
    set_chr_mode_2(0x0E);
    set_chr_mode_3(0x0F);
    set_chr_mode_4(0x10);
    set_chr_mode_5(0x11);

    pal_bg((const char*)pal_title);
    // Staff palette
    pal_col(0x11, 0x21);
    pal_col(0x12, 0x18);
    pal_col(0x13, 0x0c);

    // bank_push(2);
    // vram_adr(NAMETABLE_A);
    // vram_unrle(title_screen);

    // vram_adr(NAMETABLE_C);
    // vram_unrle(story);
    // bank_pop();

    RUN_WITH_BANK(GET_BANK(title_screen), {
        vram_adr(NAMETABLE_A);
        vram_unrle(title_screen);

        vram_adr(NAMETABLE_C);
        vram_unrle(story);
    });

    music_play(0);
    game_state = TITLE;
}

void first_hit_long() {
    if (backup_nt_index % 2) {
        --backup_nt_index;
    }
    one_vram_buffer(0x0a, backup_nt_index);
    ++backup_nt_index;
    one_vram_buffer(0x0b, backup_nt_index);
    c_map[backup_col_index] = 0b00110011;
}

void first_hit_small() {
    one_vram_buffer(0x17, backup_nt_index);
    if (backup_nt_index % 2) {
        temp = c_map[backup_col_index] & 0b11110000;
        c_map[backup_col_index] = temp + 0b00000101;

    } else {
        temp = c_map[backup_col_index] & 0b00001111;
        c_map[backup_col_index] = temp + 0b01010000;
    }
}
// param1: actor index
// A speed of 128 equals 1 pixel per frame
signed char get_x_speed() {
    temp_speed = actors.xSpeed[param1] >> 7;                       // Divided by 128
    actors.xRemain[param1] += actors.xSpeed[param1] & 0b01111111;  // MODULO 128
    temp = 0;
    temp2 = 0;

    // Remain of Speed Float
    if (actors.xRemain[param1] > 127) {
        actors.xRemain[param1] &= 0b01111111;
        temp = 1;
    }

    // Velocity. (For exemple: when paddle hits the skull, it sets xVelocity to 80)
    temp2 = ((actors.xVelocity[param1] > 40) || (actors.xVelocity[param1] % 2)) ? 1 : 0;

    if (actors.xVelocity[param1]) {
        --actors.xVelocity[param1];
    }

    return (temp_speed + temp + temp2) * actors.xDir[param1];
}

// param1: actor index
// A spead of 128 equals 1 pixel per frame
signed char get_y_speed() {
    temp_speed = actors.ySpeed[param1] >> 7;                       // Divided by 128
    actors.yRemain[param1] += actors.ySpeed[param1] & 0b01111111;  // MODULO 128
    temp = 0;
    temp2 = 0;

    if (actors.yRemain[param1] > 127) {
        actors.yRemain[param1] &= 0b01111111;
        temp = 1;
    }

    temp2 = ((actors.yVelocity[param1] > 40) || (actors.yVelocity[param1] % 2)) ? 1 : 0;

    if (actors.yVelocity[param1]) {
        --actors.yVelocity[param1];
    }

    return (temp_speed + temp + temp2) * actors.yDir[param1];
}

// Don't forget to reset the current frame when changing state.
void animate() {
    // banked_call(0, set_animation_info);
    set_animation_info();

    if (actors.counter[draw_index] == actors.animation_delay[draw_index]) {
        // STATE odd && animation finished
        if ((actors.state[draw_index] % 2 != 0) && actors.current_frame[draw_index] == frame_count - 1) {
            // NEXT STATE
            ++actors.state[draw_index];
            // banked_call(0, set_animation_info);
            set_animation_info();
            actors.current_frame[draw_index] = 0;
        } else {
            actors.current_frame[draw_index] = ++actors.current_frame[draw_index] % frame_count;
        }
        actors.counter[draw_index] = 0;
    }
    ++actors.counter[draw_index];

    animation_index += actors.current_frame[draw_index];
    if (actors.xDir[draw_index] == RIGHT) {
        animation_index += frame_count;
    }
    
    oam_meta_spr(actors.x[draw_index], actors.y[draw_index], animation_array[animation_index]);
}

unsigned char get_inactive_actor_index() {
    for (i = 6; i < ACTOR_NUMBER; ++i) {
        if (actors.state[i] == INACTIVE) {
            return i;
        }
    }
    
    return FALSE;
}

void animate_actors() {
    for (draw_index = 4; draw_index < ACTOR_NUMBER; ++draw_index) {
        if (actors.state[draw_index] != INACTIVE) {
            animate();
            move();
        }
    }
}

// param1: projectile index
// param2: actor target index
// Get the right direction and speed for a projectile to follow a target
void set_projectile_dir_speed() {
    if (actors.x[param1] > actors.x[param2]) {
        actors.xDir[param1] = LEFT;
        actors.xSpeed[param1] = actors.x[param1] - actors.x[param2];
    } else {
        actors.xDir[param1] = RIGHT;
        actors.xSpeed[param1] = actors.x[param2] - actors.x[param1];
    }
    if (actors.y[param1] > actors.y[param2]) {
        actors.yDir[param1] = UP;
        actors.ySpeed[param1] = actors.y[param1] - actors.y[param2];
    } else {
        actors.yDir[param1] = DOWN;
        actors.ySpeed[param1] = actors.y[param2] - actors.y[param1];
    }
}


void move() {
    switch (actors.type[draw_index]) {
        case TYPE_SKELETON:
            param1 = draw_index;  // actor index for get_x_speed()
            if (actors.state[draw_index] == DEAD) {
                if (game_state != STORY && actors.counter[draw_index] == 254) {
                    // LIFE AFTER DEATH!
                    actors.state[draw_index] = RISING;
                    actors.counter[draw_index] == 0;
                    actors.animation_delay[draw_index] = 16;
                }
            } else if (actors.state[draw_index] != DYING && actors.state[draw_index] != RISING) {
                temp_x = actors.x[draw_index] + get_x_speed();
                // Collision detection at the feet of the skeleton:
                temp_y_col = actors.y[draw_index] + actors.height[draw_index];

                temp_x_col = temp_x;
                if (actors.xDir[draw_index] == RIGHT) {
                    temp_x_col += actors.width[draw_index];
                }

                if (get_collision_type()) {
                    actors.current_frame[draw_index] = 0;
                    actors.counter[draw_index] = 0;
                    actors.state[draw_index] = TURNING;
                    actors.xDir[draw_index] = -actors.xDir[draw_index];
                } else {
                    actors.x[draw_index] = temp_x;
                }
            }
            break;
        case TYPE_CROW:
            param1 = CROW;
            switch (actors.state[CROW]) {
                case FLYING:
                    if (actors.x[CROW] < 6) {
                        actors.state[CROW] = INACTIVE;
                    }
                    break;
                case CHASING:
                    param2 = SKULL;
                    set_projectile_dir_speed();
                    actors.xVelocity[CROW] = 64;
                    actors.yVelocity[CROW] = 64;
                    break;
                case KIDNAPPING:
                    actors.x[SKULL] = actors.x[CROW] + 8;
                    actors.y[SKULL] = actors.y[CROW] + 8;
                    if (actors.y[CROW] < 72) {
                        actors.ySpeed[CROW] = 0;
                        actors.yRemain[CROW] = 0;
                        actors.yVelocity[CROW] = 0;
                    }
                    if (actors.x[CROW] < 80) {
                        actors.state[CROW] = RETURNING;
                        actors.animation_delay[CROW] = 64;
                        // Crow let the skull go:
                        actors.xVelocity[SKULL] = 32;
                        actors.xDir[SKULL] = LEFT;
                        actors.state[SKULL] = ROTATE_H;

                        // Move crow towards scarecrow
                        param2 = STILL_DECORATION;
                        set_projectile_dir_speed();
                    }
                    oam_spr(actors.x[SKULL], actors.y[SKULL], 0x00, 0x00);
                    break;
                case RETURNING:
                    if (actors.x[CROW] == actors.x[STILL_DECORATION]) {
                        actors.state[CROW] = IDLE2;
                        actors.xDir[CROW] = LEFT;
                        actors.x[CROW] = 192;
                        actors.y[CROW] = 60;
                        actors.state[TRIGGER] = IDLE;
                    }
                    break;
            }
            if (actors.state[CROW] == IDLE2 || actors.state[CROW] == SKWAK) {
                if (actors.y[SKULL] > 120 && actors.y[SKULL] < 132) {
                    actors.state[CROW] = SKWAK;
                }
            } else {
                actors.x[CROW] += get_x_speed();
                actors.y[CROW] += get_y_speed();
            }
            break;
        case TYPE_SORCERER:
            if (actors.state[SORCERER] == ATTACKING) {
                ++actors.state[SORCERER];
                temp = get_inactive_actor_index();
                if (temp) {
                    actors.x[temp] = actors.x[SORCERER] + 8;
                    actors.y[temp] = actors.y[SORCERER] + 16;
                    actors.width[temp] = 8;
                    actors.height[temp] = 8;
                    actors.animation_delay[temp] = 8;
                    actors.state[temp] = APPEARING;
                    actors.type[temp] = TYPE_PARALYZER;

                    param1 = temp;
                    param2 = 0;
                    set_projectile_dir_speed();

                }
            }
            break;
        case TYPE_PARALYZER:
            param1 = draw_index;
            actors.x[draw_index] += get_x_speed();
            actors.y[draw_index] += get_y_speed();
            if (actors.x[draw_index] < 8 || actors.x[draw_index] > 248 || actors.y[draw_index] > 240 || actors.y[draw_index] < 8) {
                actors.state[draw_index] = INACTIVE;
            }
            // Check paddle collision
            pad_index = 0;
            if (has_collision()) {
                actors.counter[pad_index] = 100;
                actors.state[draw_index] = INACTIVE;
                actors.state[pad_index] = PARALYZED;
            }
            break;
        case TYPE_ITEM_BOMB:
            ++actors.width[draw_index];
            if (actors.width[draw_index] == BOMB_DELAY) {
                actors.counter[draw_index] = 0;
                actors.state[draw_index] = APPEARING;
            }
            if (actors.state[draw_index] == 2) {
                temp_x_col = actors.x[draw_index];
                temp_y_col = actors.y[draw_index];
                
                for (i = 0; i < 3; i += 8) {
                    for (j = 0; j < 3; j += 8) {
                        temp_x_col += i;
                        temp_y_col += j;
                        if (set_collision_data()) {
                            if (backup_col_type == COL_TYPE_BOMBABLE) {
                                do_skull_tile_collision();
                            }
                        }
                    }
                }
                actors.counter[draw_index] = NULL;
                actors.width[draw_index] = NULL;
                actors.state[draw_index] = INACTIVE;
            }
            break;
        case TYPE_ITEM_SEED:
            ++actors.width[draw_index]; // TIMER
            if (actors.state[draw_index] == IDLE) {
                if (actors.width[draw_index] == 250) {
                    ++actors.state[draw_index];
                }
            } else if (actors.state[draw_index] == IDLE2) {
                if (actors.width[draw_index] == 250) {
                    actors.state[TRIGGER] = INACTIVE;
                    actors.state[CROW] = FLYING;
                    param1 = CROW;
                    param2 = SPECIAL_ITEM;
                    set_projectile_dir_speed();
                    ++actors.state[draw_index];
                }
            } else if (actors.state[draw_index] == 4) {
                if (actors.x[CROW] == actors.x[draw_index]) {
                    actors.state[CROW] = IDLE;
                    actors.xSpeed[CROW] = 0;
                    actors.ySpeed[CROW] = 0;
                    actors.counter[CROW] = 0;
                    actors.y[CROW] = actors.y[draw_index] - 16;
                }
                if (actors.width[draw_index == 188]) {
                    actors.state[CROW] = CROW_EAT;
                }
                if (actors.width[draw_index] == 250) {
                    ++actors.state[draw_index];
                }
            } else if (actors.state[draw_index] == CHEWING) {
                actors.state[CROW] = INACTIVE;
                if (actors.x[CROW] == actors.x[draw_index] - 8) {
                    ++actors.state[draw_index];
                }
            }
            break;
        
    }
}

void draw_paddles() {
    for (i = 0; i < paddle_count; ++i) {
        if (actors.type[i] == TYPE_PAD_HORZ) {
            if (actors.state[i] == PAD_SHORT) {
                oam_meta_spr(actors.x[i], actors.y[i], HorizontalPaddleSpr_short);
            } else {
                oam_meta_spr(actors.x[i], actors.y[i], HorizontalPaddleSpr);
            }
        } else {
            if (actors.state[i] == PAD_SHORT) {
                oam_meta_spr(actors.x[i], actors.y[i], VerticalPaddleSpr_short);
            } else {
                oam_meta_spr(actors.x[i], actors.y[i], VerticalPaddleSpr);
            }
        }
    }
}

// param1 param2: from x y
// param3 param4: to x y
void move_skull_map() {
    // TODO:
    if (param1 > param3) {
        temp_x = param1 - param3;
    } else {
        temp_x = param3 - param1;
    }
    if (param2 > param4) {
        temp_y = param2 - param4;
    } else {
        temp_y = param4 - param2;
    }
}

// Will fade out brightness and increment story_step
void fadeout() {
    if (story_counter % FADE_SPEED == 0) {
        --brightness;
        pal_bright(brightness);
    }
    if (brightness == 0) {
        ++story_step;
        story_counter = 0;
    }
}

// Will fade in and increment story_step when done
void fadein() {
    if (story_counter % FADE_SPEED == 0) {
        ++brightness;
        pal_bright(brightness);
    }
    if (brightness == 4) {
        ++story_step;
        story_counter = 0;
    }
}

// Will brighten to white and increment story_step when done
void brightout() {
    if (story_counter % FADE_SPEED == 0) {
        ++brightness;
        pal_bright(brightness);
    }
    if (brightness == 8) {
        ++story_step;
        story_counter = 0;
        brightness = 4;
    }
}

void wait_input() {
    // Show waiting cursor
    if (wait_timer > 64) {
        one_vram_buffer(0x00, NTADR_A(30, 3));
    } else {
        one_vram_buffer(0x29, NTADR_A(30, 3));
    }
    // Erasing previous dialogs
    if (pad1_new & PAD_A) {
        ++story_step;
        story_counter = 0;
        wait_timer = 0;
        multi_vram_buffer_horz(empty_line, sizeof(empty_line), NTADR_A(7, 1));
        ppu_wait_nmi();
        multi_vram_buffer_horz(empty_line, sizeof(empty_line), NTADR_A(7, 2));
        ppu_wait_nmi();
        multi_vram_buffer_horz(empty_line, sizeof(empty_line), NTADR_A(7, 3));
        ppu_wait_nmi();
    };
    ++wait_timer;
}

void wait(unsigned char delay) {
    if (story_counter > delay) {
        ++story_step;
        story_counter = 0;
    }
}


#pragma rodata-name("LOWCODE")
#pragma code-name("LOWCODE")

void play_normal_level() {
    switch (story_step) {
        case 0:
            fadein();
            break;
        case 1:
            game_state = MAIN;
            show_HUD();
            break;
        case STORY_LEVEL_END:
            wait(64);
            break;
        case STORY_LEVEL_END + 1:
            fadeout();
            break;
        case STORY_LEVEL_END + 2:
            ++current_level;
            // oam_clear();
            load_level();
            story_step = STORY_LEVEL_START;
            break;
    }
}

void play_story() {
    switch (current_level) {
        case LVL_ALTAR:
            // oam_clear();
            animate_actors();
            switch (story_step) {
                case 0:
                    if (story_counter == 128) {
                        // Necromancer lift his arms
                        ++actors.state[NECROMANCER];
                        actors.current_frame[NECROMANCER] = 0;
                        story_counter = 0;
                        brightness = 4;
                        ++story_step;
                    }
                    break;
                case 1:
                    if (story_counter == 64) {
                        // then hits the ground
                        ++actors.state[NECROMANCER];
                    }
                    if (story_counter > 68) {
                        pal_bright(6);
                        pal_spr(pal_altar_lightning);
                        ++actors.state[LIGHTNING];
                        story_counter = 0;
                        ++story_step;
                    }
                    break;
                case 2:
                    // Lightning strike!
                    if (story_counter > 2) {
                        pal_bright(4);
                    }
                    if (actors.state[LIGHTNING] == WALKING) {
                        pal_spr(pal_altar_spr);
                        ++story_step;
                        story_counter = 0;
                    }
                    if (actors.current_frame[LIGHTNING] == 8) {
                        pal_bright(6);
                        actors.state[SKULL] = ROTATE_H;
                    }
                    break;
                case 3:
                    // Waiting...
                    actors.state[HERO] = 1;
                    if (story_counter > 180) {
                        ++story_step;
                        story_counter = 0;
                    }
                    break;
                case 4:
                    fadeout();
                    break;
                case 5:
                    // banked_call(1, reset_actors);
                    reset_actors();
                    // banked_call(0, init_skull);
                    init_skull();
                    // banked_call(1, load_map);
                    load_map();
                    actors.x[SKULL] = 128;
                    actors.y[SKULL] = 56;
                    actors.state[SKULL] = ROTATE_H;
                    ++story_step;
                case 6:
                    fadein();
                    break;
                case 7:
                    story_counter = 56;  // Skull y position
                    ++story_step;
                case 8:
                    // Skull moving to cemetery on map 128.56 to 156.156
                    actors.y[SKULL] = story_counter;
                    if (story_counter > 156) {
                        ++story_step;
                        story_counter = 128;
                    }
                    break;
                case 9:
                    actors.x[SKULL] = story_counter;
                    if (story_counter > 156) {
                        ++story_step;
                        story_counter = 0;
                        brightness = 4;
                    }
                    // TODO? Write cemetery
                    break;
                case 10:
                    wait(24);
                    break;
                case 11:
                    fadeout();
                    break;
                case 12:
                    // banked_call(1, hide_map);
                    hide_map();
                    actors.state[SKULL] = INACTIVE;
                    // banked_call(1, reset_actors);
                    reset_actors();
                    set_chr_mode_1(0x06);  // Angelic sprite
                    current_level = LVL_CEMETERY;
                    story_step = NULL;
                    show_face = FALSE;
                    show_item = FALSE;
                    break;
            }
            break;
        case LVL_CEMETERY:
            // Show story sprites
            // oam_clear();
            if (story_step > 6) {
                draw_paddles();
                animate_actors();
            }
            if (show_face) {
                oam_meta_spr(FACE_X, FACE_Y, angelic_face);
            }
            if (show_item) {
                oam_spr(123, 130, ITEM_INDEX_MAGNET, 0);
            }
            switch (story_step) {
                case 0:
                    load_black_level();
                    pal_bright(4);
                    ++story_step;
                    break;
                case 1:
                    wait(128);
                    break;
                case 2:
                    // Where?
                    multi_vram_buffer_horz(dial0001, sizeof(dial0001), NTADR_A(6, 15));
                    ppu_wait_nmi();
                    ++story_step;
                    break;
                case 3:
                    wait(72);
                    break;
                case 4:
                    // Where am I?
                    multi_vram_buffer_horz(dial0002, sizeof(dial0002), NTADR_A(15, 15));
                    ppu_wait_nmi();
                    ++story_step;
                    break;
                case 5:
                    wait(128);
                    break;
                case 6:
                    pal_bright(0);
                    brightness = 0;
                    show_face = TRUE;
                    load_level();
                    actors.state[SKULL] = ROTATE_H;
                    actors.x[SKULL] = 0x7C;
                    actors.y[SKULL] = 0xCA;
                    ++story_step;
                    break;
                case 7:
                    fadein();
                    break;
                case 8:
                    wait(128);
                    break;
                case 9:
                    // do not be affraid
                    multi_vram_buffer_horz(dial0003, sizeof(dial0003), NTADR_A(7, 1));
                    multi_vram_buffer_horz(dial0004, sizeof(dial0004), NTADR_A(7, 2));
                    ++story_step;
                    break;
                case 10:
                    wait_input();
                    break;
                case 11:
                    // This is the start
                    multi_vram_buffer_horz(dial0005, sizeof(dial0005), NTADR_A(7, 1));
                    multi_vram_buffer_horz(dial0006, sizeof(dial0006), NTADR_A(7, 2));
                    ++story_step;
                    break;
                case 12:
                    wait_input();
                    break;
                case 13:
                    // You must go back
                    multi_vram_buffer_horz(dial0007, sizeof(dial0007), NTADR_A(7, 1));
                    multi_vram_buffer_horz(dial0008, sizeof(dial0008), NTADR_A(7, 2));
                    multi_vram_buffer_horz(dial0009, sizeof(dial0009), NTADR_A(7, 3));
                    ++story_step;
                    break;
                case 14:
                    wait_input();
                    break;
                case 15:
                    // Take this!
                    multi_vram_buffer_horz(dial0010, sizeof(dial0010), NTADR_A(7, 1));
                    show_item = TRUE;
                    current_item = 0;
                    items.type[current_item] = TYPE_ITEM_MAGNET;
                    items.is_active[current_item] = TRUE;
                    items.sprite[current_item] = ITEM_INDEX_MAGNET;
                    ++story_step;
                    break;
                case 16:
                    wait_input();
                    break;
                case 17:
                    // Use it
                    multi_vram_buffer_horz(dial0011, sizeof(dial0011), NTADR_A(7, 1));
                    multi_vram_buffer_horz(dial0012, sizeof(dial0012), NTADR_A(7, 2));
                    multi_vram_buffer_horz(dial0013, sizeof(dial0013), NTADR_A(7, 3));
                    ++story_step;
                    break;
                case 18:
                    wait_input();
                    break;
                case 19:
                    // Hit all tombstones...
                    multi_vram_buffer_horz(dial0014, sizeof(dial0014), NTADR_A(7, 1));
                    multi_vram_buffer_horz(dial0015, sizeof(dial0015), NTADR_A(7, 2));
                    multi_vram_buffer_horz(dial0016, sizeof(dial0016), NTADR_A(7, 3));
                    ++story_step;
                    break;
                case 20:
                    wait_input();
                    break;
                case 21:
                    brightout();
                    break;
                case 22:
                    actors.state[GHOST] = INACTIVE;
                    //
                    actors.counter[SKELETON1] = 250;
                    actors.counter[SKELETON2] = 250;
                    actors.state[SKELETON1] = RISING;
                    actors.state[SKELETON2] = RISING;
                    // MAIN GAMEPLAY
                    ++story_step;
                    game_state = MAIN;
                    show_face = FALSE;
                    show_item = FALSE;
                    ++actors.state[STARS];

                    pal_bright(4);
                    show_HUD();
                    break;
                case 23:
                    wait(64);
                    break;
                case 24:
                    // END OF LEVEL
                    if (actors.state[CROW] != FLYING) {
                        one_vram_buffer(0x11, NTADR_A(15, 9));
                        one_vram_buffer(0x11, NTADR_A(16, 9));
                        actors.state[GATE] = TURNING;
                        ++story_step;
                    }
                    break;
                case 25:
                    wait(64);
                    break;
                case 26:
                    fadeout();
                    break;
                case 27:
                    ++current_level;
                    load_level();
                    // oam_clear();
                    story_step = 0;
                    break;
            }
            break;
        case LVL_TEMPLE1:
            play_normal_level();
            break;
        case LVL_TEMPLE2:
            play_normal_level();
            break;
        case LVL_TEMPLE3:
            play_normal_level();
            break;
        case LVL_TEMPLE4:
            play_normal_level();
            break;
        case LVL_TOWN:
            play_normal_level();
            break;
        case LVL_FARM:
            if (show_face) {
                oam_meta_spr(FACE_X, FACE_Y, grakk_face);
                draw_paddles();
                if (actors.state[DOOR1] == OPENED) {
                    oam_meta_spr(58, 144, grakk);
                }
            }
            animate_actors();
            switch (story_step) {
                case STORY_LEVEL_EVENT:
                    hide_HUD();
                    show_face = TRUE;
                    ++story_step;
                    break;
                case STORY_LEVEL_EVENT + 1:
                    // If crow moved at least once
                    if (level_condition1) {
                        ++story_step;
                        ++actors.state[DOOR1];
                    } else {
                        //  Pesky crow won't leave...
                        multi_vram_buffer_horz(dial0017, sizeof(dial0017), NTADR_A(7, 1));
                        multi_vram_buffer_horz(dial0018, sizeof(dial0018), NTADR_A(7, 2));
                        multi_vram_buffer_horz(dial0019, sizeof(dial0019), NTADR_A(7, 3));
                        story_step = STORY_LEVEL_EVENT + 7;
                    }
                    break;
                case STORY_LEVEL_EVENT + 2:
                    multi_vram_buffer_horz(dial0020, sizeof(dial0020), NTADR_A(7, 1));
                    ++story_step;
                    break;
                case STORY_LEVEL_EVENT + 3:
                    wait_input();
                    break;
                case STORY_LEVEL_EVENT + 4:
                    // Grakk busy. Take this...
                    multi_vram_buffer_horz(dial0021, sizeof(dial0021), NTADR_A(7, 1));
                    multi_vram_buffer_horz(dial0022, sizeof(dial0022), NTADR_A(7, 2));
                    //  Gives seed
                    current_item = 5;
                    items.sprite[current_item] = ITEM_INDEX_SEED;
                    items.type[current_item] = TYPE_ITEM_SEED;
                    items.is_active[current_item] = TRUE;
                    ++story_step;
                    break;
                case STORY_LEVEL_EVENT + 5:
                    oam_spr(136, 16, ITEM_INDEX_SEED, 0);
                    wait_input();
                    break;
                case STORY_LEVEL_EVENT + 6:
                    // This is gift for pesky crow...
                    multi_vram_buffer_horz(dial0023, sizeof(dial0023), NTADR_A(7, 1));
                    multi_vram_buffer_horz(dial0024, sizeof(dial0024), NTADR_A(7, 2));
                    multi_vram_buffer_horz(dial0025, sizeof(dial0025), NTADR_A(7, 3));
                    ++story_step;
                    break;
                case STORY_LEVEL_EVENT + 7:
                    wait_input();
                    break;
                case STORY_LEVEL_EVENT + 8:
                    show_HUD();
                case STORY_LEVEL_EVENT + 9:
                    game_state = MAIN;
                    show_face = FALSE;
                    if (current_item == 5) {
                        actors.state[STARS] = TURNING;
                    }
                    break;
                default:
                    play_normal_level();
            }
            break;
        case LVL_TEST:
            play_normal_level();
            break;
        case LVL_INTRO:
            // INTRO STORY
            switch (story_step) {
                case 0:
                    oam_meta_spr(182, 122, staff);
                    fadeout();
                    break;
                case 1:
                    wait(64);
                    break;
                case 2:
                    // Erase the title screen (sad, but no choice!)
                    load_black_level();
                    ++story_step;
                    break;
                case 3:
                    // Prepare the graphics
                    set_chr_mode_2(0x00);
                    set_chr_mode_3(0x01);
                    pal_bright(4);
                    story_counter = NULL;
                    scroll_index_y = NULL;
                    ++story_step;
                    break;
                case 4:
                    // Scroll !!!
                    set_scroll_y(scroll_index_y);
                    if (story_counter > 10) {
                        scroll_index_y = add_scroll_y(1, scroll_index_y);
                        story_counter = NULL;
                    }
                    ++story_counter;
                    if (scroll_index_y > 488) {
                        ++story_step;
                    }
                    break;
                case 5:
                    fadeout();
                    break;
                case 6:
                    wait(32);
                    break;
                case 7:
                    story_step = 0;
                    current_level = 0;
                    set_scroll_y(0);
                    load_level();
                    pal_bright(4);
                    break;
            }
            break;
    }
    ++story_counter;
    if (wait_timer == 127) {
        wait_timer = 0;
    }
    ++wait_timer;
}

void debug_start(char debuglevel) {
    set_chr_mode_1(0x06);
    set_chr_mode_2(0x00);
    set_chr_mode_3(0x01);
    current_level = debuglevel;
    load_level();
    actors.state[SKULL] = IDLE;
    brightness = NULL;
    pal_bright(brightness);

    current_item = 5;
    items.sprite[current_item] = ITEM_INDEX_SEED;
    items.type[current_item] = TYPE_ITEM_SEED;
    items.is_active[current_item] = TRUE;

    current_item = 4;
    items.sprite[current_item] = ITEM_INDEX_HEAL;
    items.type[current_item] = TYPE_ITEM_HEAL;
    items.is_active[current_item] = TRUE;
    
    current_item = 3;
    items.sprite[current_item] = ITEM_INDEX_BIG;
    items.type[current_item] = TYPE_ITEM_BIG;
    items.is_active[current_item] = TRUE;
    
    current_item = 2;
    items.sprite[current_item] = ITEM_INDEX_HOOK;
    items.type[current_item] = TYPE_ITEM_HOOK;
    items.is_active[current_item] = TRUE;
    
    current_item = 1;
    items.sprite[current_item] = ITEM_INDEX_BOMB;
    items.type[current_item] = TYPE_ITEM_BOMB;
    items.is_active[current_item] = TRUE;
    
    current_item = 0;
    items.sprite[current_item] = ITEM_INDEX_MAGNET;
    items.type[current_item] = TYPE_ITEM_MAGNET;
    items.is_active[current_item] = TRUE;

    // current_item = 5;
}

void init() {
    // clear the WRAM, not done by the init code
    // memfill(void *dst,unsigned char value,unsigned int len);
    memfill(wram_array, 0, 0x2000);
}

void main() {

    ppu_off();

    disable_irq();

    set_scroll_y(0xff);  // shift the bg down 1 pixel

    // color_emphasis(COL_EMP_NORMAL);

    set_mirroring(MIRROR_HORIZONTAL);

    set_vram_buffer();

    load_title_screen();

    // banked_call(0, init_skull);
    init_skull();
    p1_health = 3;
    p1_max_health = 4;

    bank_spr(1);
    oam_meta_spr(182, 122, staff);

    ppu_on_all();

    while (1) {
        ppu_wait_nmi();
        
        oam_clear();

        pad1 = pad_poll(0);
        pad1_new = get_pad_new(0);

        switch (game_state) {
            case MAIN:
                check_main_input();
                update_skull();
                draw_paddles();
                animate_actors();

                // CURRENT ITEM:
                oam_spr(ITEM_X, ITEM_Y, items.sprite[current_item], 0);

                if (brick_counter == 0) {
                    game_state = STORY;
                }

                // Check if status changed, if so prepare data
                if (game_state == MAP) {
                    // banked_call(1, load_map);
                    load_map();
                }
                
                if (game_state == INVENTORY) {
                    // banked_call(1, load_inventory);
                    load_inventory();
                    draw_index = CURSOR;
                    
                    actors.y[CURSOR] = INVENTORY_ITEM_Y;
                    actors.animation_delay[CURSOR] = 32;
                    actors.type[CURSOR] = TYPE_CURSOR;
                    actors.state[CURSOR] = CURSOR_SMALL;
                    
                    current_selection = current_item;
                    // TODO: Play sound ?
                }
                ///////////////////////////////////////////////////

                // gray_line();
                break;
            case INVENTORY:
                // banked_call(1, manage_inventory);
                manage_inventory();
                animate();
                break;
            case MAP:
                // TODO DRAW SKULL AT CORRECT POSITION
                oam_spr(1, 1, 0x00, 0x00);
                if (pad1_new & PAD_START) {
                    // Return to gameplay
                    // banked_call(1, hide_map);
                    hide_map();
                    game_state = MAIN;
                }
                break;
            case STORY:
                play_story();
                break;
            case TITLE:
                // SHOW STAFF
                oam_meta_spr(182, 122, staff);
                // TODO Show skull cursor sprite

                // Player's choice
                // TODO  Continue or New Game

                // PRESS START TO PLAY!
                if (pad1_new & PAD_START) {
                    game_state = STORY;
                    current_level = LVL_INTRO;
                    story_step = STORY_LEVEL_START;
                    scroll_index_y = NULL;

                    // DEBUG
                    // debug_start(LVL_CEMETERY);
                }
                break;
            case GAME_OVER:
                break;
        }
    }
}
