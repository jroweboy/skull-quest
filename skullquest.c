#include "dialog.h"
#include "skullquest.h"

#include "LIB/nesdoug.h"
#include "LIB/neslib.h"
#include "MMC3/mmc3_code.h"
#include "MMC3/mmc3_code.c"

// NAMETABLES COLLISION PALETTE
#include "Collision/master_collision.h"
#include "I-CHR/altar.pngE/altar.h"
#include "I-CHR/cemetery.pngE/cemetery.h"
#include "I-CHR/map.pngE/map.h"
#include "Nametable/title_screen.h"
#include "Nametable/black_level.h"
#include "palettes.h"

// SPRITES METATILES IN BANK 1
#include "spr_angelic.h"
#include "spr_crow.h"
#include "spr_general.h"
#include "spr_hero.h"
#include "spr_lightning.h"
#include "spr_necromancer.h"
#include "spr_skeleton.h"
#include "spr_skull.h"
#include "spr_star.h"

#pragma bss-name(push, "ZEROPAGE")
static unsigned char pad1;
static unsigned char pad1_new;

static unsigned char pad_index, temp_y_col, temp_x_col, chr_4_index, chr_5_index;
static unsigned char i, param1, param2, param3, param4, temp, temp2, temp_speed, temp_x, temp_y, backup_col_type, skull_launched;
static unsigned char p1_health, p1_max_health, brick_counter, tombstone_count, wait_timer;
static unsigned char game_state, current_level, paddle_count, enemy_count, story_step, story_counter;
static unsigned char brightness = 4;

static int collision_index, backup_col_index, backup_nt_index;
#pragma bss-name(pop)

#pragma bss-name(push, "XRAM")
// extra RAM at $6000-$7fff
unsigned char wram_array[0x2000];
#pragma bss-name(pop)

static char exp[] = "00000000";
static unsigned char c_map[368];

#define LEVEL_NUMBER 2

typedef struct {
    unsigned char* nametable[LEVEL_NUMBER];
    unsigned char* collision_map[LEVEL_NUMBER];
    unsigned char* background_palette[LEVEL_NUMBER];
    unsigned char* sprite_palette[LEVEL_NUMBER];
} Levels;

Levels levels;

#define ACTOR_NUMBER 12

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
} Actors;

// index 0-3 paddles
// index 4 skull
// index 5 necromancer / angelica
// index 6-7-8-9 ennemies
// index 10 STARS animation for item reveal
Actors actors;

#pragma rodata-name("BANK0")
#pragma code-name("BANK0")

void init_skeletons() {
    for (i = 6; i < 8; ++i) {
        if (i == 6) {
            actors.x[i] = 40;
            actors.y[i] = 72;
            actors.xDir[i] = LEFT;
        } else {
            actors.x[i] = 160;
            actors.y[i] = 104;
            actors.xDir[i] = RIGHT;
        }
        actors.width[i] = 0x04;
        actors.height[i] = 0x14;
        actors.bbox_x[i] = 0x01;
        actors.bbox_y[i] = 0x02;
        actors.yDir[i] = NULL;
        actors.xSpeed[i] = 10;
        actors.ySpeed[i] = 0;
        actors.xVelocity[i] = 0;
        actors.yVelocity[i] = 0;
        actors.minSpeed[i] = 0;
        actors.maxSpeed[i] = 20;
        actors.counter[i] = 0;
        actors.animation_delay[i] = 16;
        actors.current_frame[i] = 0;
        actors.state[i] = DEAD;
        actors.type[i] = TYPE_SKELETON;
    }
}

void init_levels(){
    // ALTAR 
    levels.nametable[0] = altar;
    levels.collision_map[0] = cemetery_col;
    levels.background_palette[0] = pal_altar_bg;
    levels.sprite_palette[0] = pal_altar_spr;

    // CEMETERY
    levels.nametable[1] = cemetery;
    levels.collision_map[1] = cemetery_col;
    levels.background_palette[1] = pal_cemetery_bg;
    levels.sprite_palette[1] = pal_cemetery_spr;

    // TEMPLATE
    // levels.nametable[] = ;
    // levels.collision_map[] = ;
    // levels.background_palette[] = ;
    // levels.sprite_palette[] = ;

    // SINGLE STATIC ANIMATIONS
    // Lightning
    actors.state[LIGHTNING] = IDLE;
    actors.animation_delay[LIGHTNING] = 8;

    // STARS
    actors.x[STARS] = 127;
    actors.y[STARS] = 136;
    actors.state[STARS] = IDLE;
    actors.animation_delay[STARS] = 8;
};

void init_level_specifics() {
    switch (current_level) {
        case 0:
            // Altar
            // paddle_count = 0;
            // Prepare the lightning graphics
            set_chr_mode_1(0x0A);
            // Torches:
            actors.x[0] = 64;
            actors.x[1] = 88;
            actors.x[2] = 160;
            actors.x[3] = 184;
            for (i = 0; i < 4; ++i) {
                actors.y[i] = 72;
                actors.animation_delay[i] = 8;
                actors.current_frame[i] = i;
                actors.state[i] = IDLE;
                actors.xDir[i] = LEFT;
            }
            // Necromancer:
            actors.animation_delay[NECROMANCER] = 20;
            actors.current_frame[NECROMANCER] = 0;
            actors.state[NECROMANCER] = IDLE;
            actors.xDir[NECROMANCER] = LEFT;
            break;
        case 1:
            // CEMETERY
            // Achievement 1 : Scarecrow
            // Achievement 2 : Skeleton buster

            // STAR

            // Prepare Angelic sprites
            set_chr_mode_1(0x07);
            enemy_count = 3;
            chr_4_index = 2;
            chr_5_index = 3;
            
            // Paddle
            paddle_count = 1;
            actors.x[0] = 0x70;
            actors.y[0] = 0xD0;

            banked_call(0, init_skeletons);

            actors.x[CROW] = 207;
            actors.y[CROW] = 117;
            actors.state[CROW] = 2;  // IDLE state of crow is 2... don't ask!
            actors.animation_delay[CROW] = 64;
            actors.current_frame[CROW] = 0;
            actors.xDir[CROW] = LEFT;
            actors.yDir[CROW] = UP;
            actors.xSpeed[CROW] = 128;
            actors.ySpeed[CROW] = 32;
            actors.type[CROW] = TYPE_CROW;
            actors.bbox_x[CROW] = 0;
            actors.bbox_y[CROW] = 4;
            actors.width[CROW] = 16;
            actors.height[CROW] = 16;
            break;
        case 2:
            // TEMP
            // chr_4_index = ?
            // chr_5_index = ?
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
        actors.xDir[i] = 0;
        actors.yDir[i] = 0;
        actors.xSpeed[i] = 0;
        actors.ySpeed[i] = 0;
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
    actors.xDir[SKULL] = RIGHT;
    actors.yDir[SKULL] = UP;
    actors.xSpeed[SKULL] = 100;
    actors.ySpeed[SKULL] = 100;
    actors.xRemain[SKULL] = 0;
    actors.yRemain[SKULL] = 0;
    actors.xVelocity[SKULL] = 0;
    actors.yVelocity[SKULL] = 0;
    actors.minSpeed[SKULL] = 64;
    actors.maxSpeed[SKULL] = 250;
    actors.counter[SKULL] = 0;
    actors.animation_delay[SKULL] = 8;
    actors.current_frame[SKULL] = 0;
    actors.state[SKULL] = ROTATE_H;

    p1_health = 3;
    p1_max_health = 3;
}

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
    // vram_adr(NAMETABLE_A);
    // vram_fill(0xFF, 32);
    // ppu_wait_nmi();
    // vram_fill(0xFF, 32);
    // ppu_wait_nmi();
    // vram_fill(0xFF, 32);
    // ppu_wait_nmi();
    // vram_fill(0xFF, 32);
    // ppu_wait_nmi();

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

void show_map() {
    ppu_off();

    // TODO DRAW NAME OF LEVEL
    // TODO DRAW SKULL AT CORRECT POSITION
    oam_clear();
    // oam_spr(1, 1, 0x00, 0x00);  // Show skull TODO Change x,y to actual map position
    set_chr_mode_4(8);
    set_chr_mode_5(9);
    pal_col(0x00, 0x0f);
    pal_col(0x01, 0x28); // MAP BACK COLOR
    pal_col(0x02, 0x18);
    set_scroll_x(0x0100);
    ppu_wait_nmi();
    ppu_on_all();
}

void hide_map() {
    pal_bg(levels.background_palette[current_level]);
    set_chr_mode_4(chr_4_index);
    set_chr_mode_5(chr_5_index);
    set_scroll_x(0x0000);
}

void show_game_over() {
    // TODO
}

void draw_level_specifics() {
    switch (current_level) {
        case 0:
            // ALTAR
            if (story_step < 5) {
                for (i = 0; i < 4; ++i) {
                    set_animation_info(i, torch_animation_index);
                    oam_meta_spr(actors.x[i], actors.y[i], torch_animation[actors.current_frame[i] + param2]);
                }
                // Necromancer
                set_animation_info(NECROMANCER, necromancer_animation_index);
                oam_meta_spr(120, 142, necromancer_animation[actors.current_frame[NECROMANCER] + param2]);
                // Lightning
                set_animation_info(LIGHTNING, lightning_animation_index);
                oam_meta_spr(128, 108, lightning_animation[actors.current_frame[LIGHTNING] + param2]);
            }

            if (story_step > 2) {
                if (story_step < 5) {
                    oam_meta_spr(120, 72, hero_head_down);
                }
            } else {
                oam_meta_spr(120, 72, hero_head_up);
            }

            // SKULL

            set_animation_info(SKULL, skull_animation_index);
            oam_meta_spr(actors.x[SKULL], actors.y[SKULL], skull_animation[actors.current_frame[SKULL] + param2]);
            break;
        case 1:
            // CEMETERY
            // CROW
            if (actors.state[CROW] == FLYING) {
                param1 = CROW;
                actors.x[CROW] += get_x_speed();
                actors.y[CROW] += get_y_speed();
                if (actors.x[CROW] < 6) {
                    actors.state[CROW] = SKWAK;
                    actors.y[CROW] = 240;
                }
            } else if (actors.y[SKULL] > 120 && actors.y[SKULL] < 132) {
                actors.state[CROW] = SKWAK;  // SKWAK!
            }
            set_animation_info(CROW, crow_animation_index);
            oam_meta_spr(actors.x[CROW], actors.y[CROW], crow_animation[actors.current_frame[CROW] + param2]);

            // SKELETONS
            animate_skeleton();

            // OTHER
            oam_meta_spr(128, 64, door1);
            oam_meta_spr(219, 61, tree);
            break;
        case 2:
            break;
    }
}

void load_black_level() {
    ppu_off();
    set_scroll_x(0x0000);

    vram_adr(NAMETABLE_A);
    vram_unrle(black_level);
    ppu_on_all();
}

void load_level() {
    ppu_off();
    set_scroll_x(0x0000);

    vram_adr(NAMETABLE_A);
    vram_unrle(levels.nametable[current_level]);

    // TODO: compress col data! and decompress here:
    memcpy(c_map, levels.collision_map[current_level], 368);

    pal_bg(levels.background_palette[current_level]);
    pal_spr(levels.sprite_palette[current_level]);

    // Check how many tombstones or destructible brick there are
    brick_counter = 0;
    tombstone_count = 0;
    for (collision_index = 0; collision_index < 368; ++collision_index) {
        temp = c_map[collision_index] >> 4;
        if (temp == 7) {
            // Tombstone
            ++tombstone_count;
        } else {
            if (temp == 3) {
                ++brick_counter;
            } else {
                if (temp == 5) {
                    ++brick_counter;
                }
                temp = c_map[collision_index] & 0x0F;
                if (temp == 4 || temp == 5) {
                    ++brick_counter;
                }
            }
        }
    }
    tombstone_count = tombstone_count >> 1;  // Divided by 2 because we only count the first 7 in the 0x77

    banked_call(0, init_level_specifics);

    banked_call(0, init_skull);

    if (game_state == MAIN) {
        show_HUD();
        skull_launched = FALSE;
    }

    ppu_on_all();
}

void load_title_screen() {
    pal_bg((const char *)pal_forest_bg);
    pal_spr(pal_cemetery_spr);
    vram_adr(NAMETABLE_A);
    vram_unrle(title_screen);
    game_state = TITLE;
}

void load_map() {
    vram_adr(NAMETABLE_B);
    vram_unrle(map);
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

// param1: actor index
signed char get_x_speed() {
    temp_speed = actors.xSpeed[param1] >> 7;
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
signed char get_y_speed() {
    temp_speed = actors.ySpeed[param1] >> 7;
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

void add_x_speed(unsigned char val) {
    temp = actors.xSpeed[param1];
    actors.xSpeed[param1] += val;
    if (actors.xSpeed[param1] > actors.maxSpeed[param1] || actors.xSpeed[param1] < temp) {
        actors.xSpeed[param1] = actors.maxSpeed[param1];
    }
}

void subtract_x_speed(unsigned char val) {
    temp = actors.xSpeed[param1];
    actors.xSpeed[param1] -= val;
    if (actors.xSpeed[param1] < actors.minSpeed[param1] || actors.xSpeed[param1] > temp) {
        actors.xSpeed[param1] = actors.minSpeed[param1];
    }
}

void add_y_speed(unsigned char val) {
    temp = actors.ySpeed[param1];
    actors.ySpeed[param1] += val;
    if (actors.ySpeed[param1] > actors.maxSpeed[param1] || actors.ySpeed[param1] < temp) {
        actors.ySpeed[param1] = actors.maxSpeed[param1];
    }
}

void subtract_y_speed(unsigned char val) {
    temp = actors.ySpeed[param1];
    actors.ySpeed[param1] -= val;
    if (actors.ySpeed[param1] < actors.minSpeed[param1] || actors.ySpeed[param1] > temp) {
        actors.ySpeed[param1] = actors.minSpeed[param1];
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

// index = actor index
// array = animation info array
// Don't forget to reset the current frame when changing state.
void set_animation_info(const unsigned char index, const unsigned char array[][2]) {
    param2 = array[actors.state[index]][0];  // animation index
    param3 = array[actors.state[index]][1];  // number of frames
    if (actors.counter[index] == actors.animation_delay[index]) {
        if ((actors.state[index] % 2 != 0) && actors.current_frame[index] == param3 - 1) {
            ++actors.state[index];                   // NEXT STATE
            param2 = array[actors.state[index]][0];  // animation index
            param3 = array[actors.state[index]][1];  // number of frames
            actors.current_frame[index] = 0;
        } else {
            actors.current_frame[index] = ++actors.current_frame[index] % param3;
        }
        actors.counter[index] = 0;
    }
    ++actors.counter[index];

    if (actors.xDir[index] == RIGHT) {
        param2 += param3;
    }
}

void animate_skeleton() {
    for (i = 6; i < 8; ++i) {
        param1 = i;  // actor index
        if (actors.state[i] == DEAD){
            actors.animation_delay[i] = 255;
            if (game_state != STORY && actors.counter[i] == 254) {
                actors.state[i] = RISING;
                actors.counter[i] == 0;
                actors.animation_delay[i] = 16;
            }
        } else if (actors.state[i] != DYING && actors.state[i] != RISING) {
            temp_x = actors.x[i] + get_x_speed();
            // Collision detection at the feet of the skeleton:
            temp_y_col = actors.y[i] + actors.height[i];

            temp_x_col = temp_x;
            if (actors.xDir[i] == RIGHT) {
                temp_x_col += actors.width[i];
            }

            if (get_collision_type()) {
                actors.current_frame[i] = 0;
                actors.counter[i] = 0;
                actors.state[i] = TURNING;
                actors.xDir[param1] = -actors.xDir[param1];
            } else {
                actors.x[i] = temp_x;
            }
        }

        set_animation_info(i, skeleton_animation_index);
        oam_meta_spr(actors.x[i], actors.y[i], skeleton_animation[actors.current_frame[i] + param2]);
    }
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
            // Long brick
            remove_brick(TILE_BACK);
            backup_nt_index % 2 ? --backup_nt_index : ++backup_nt_index;
            --brick_counter;
            hit_brick(TILE_BACK);
            add_xp(1, HUNDREDS);
            break;
        case 0x04:
            // Tall brick ??
            break;
        case 0x05:
            // Dot brick
            hit_brick(0x11);
            add_xp(5, TENS);
            --brick_counter;
            break;
        case 0x06:
            // Foliage
            hit_brick(TILE_BACK_GRASS);
            break;
        case 0x07:
            // Tombstone
            c_map[backup_col_index] = 0x11;
            temp = 0b01010101;
            if (((temp_y_col >> 3) - 5) % 2) {
                if (backup_col_index == 182 || backup_col_index == 185) {
                    temp = 0b10100101;
                }
                backup_col_index += 16;
            } else {
                if (backup_col_index == 198 || backup_col_index == 201) {
                    temp = 0b10100101;
                }
                backup_col_index -= 16;
            }
            c_map[backup_col_index] = 0x11;

            one_vram_buffer(temp, (backup_nt_index & 0x2C00) | 0x3C0 | ((backup_nt_index >> 4) & 0x38) | ((backup_nt_index >> 2) & 0x07));
            --tombstone_count;
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

char skull_was_beside() {
    return actors.x[SKULL] + 1 > actors.x[pad_index] + actors.width[pad_index] + actors.bbox_x[pad_index] ||
           actors.x[SKULL] + 7 < actors.x[pad_index] + actors.bbox_x[pad_index];
}

char skull_was_above() {  // or below !!!
    return actors.y[SKULL] + 1 > actors.y[pad_index] + actors.height[pad_index] + actors.bbox_y[pad_index] ||
           actors.y[SKULL] + 7 < actors.y[pad_index] + actors.bbox_y[pad_index];
}

char is_skull_collision_paddle() {
    // Skull temp_x and temp_y already include the bbox
    return (temp_x < actors.x[pad_index] + actors.width[pad_index] + actors.bbox_x[pad_index] &&
            temp_x + actors.width[SKULL] > actors.x[pad_index] + actors.bbox_x[pad_index] &&
            temp_y < actors.y[pad_index] + actors.height[pad_index] + actors.bbox_y[pad_index] &&
            temp_y + actors.height[SKULL] > actors.y[pad_index] + actors.bbox_y[pad_index]);
}

char is_paddle_collision_skull() {
    // Skull bbox is hard-coded here
    return (actors.x[SKULL] + 7 > temp_x_col + actors.bbox_x[pad_index] &&
            actors.y[SKULL] + 7 > temp_y_col + actors.bbox_y[pad_index] &&
            actors.x[SKULL] + 1 < temp_x_col + actors.width[pad_index] + actors.bbox_x[pad_index] &&
            actors.y[SKULL] + 1 < temp_y_col + actors.height[pad_index] + actors.bbox_y[pad_index]);
}

void check_enemy_collision() {
    for (i = 6; i < 6 + enemy_count; ++i) {
        if (actors.state[i] != DEAD) {
            pad_index = i;
            if (actors.state[i] != DYING && is_skull_collision_paddle()) {
                switch (actors.type[i]) {
                    case TYPE_CROW:
                        actors.counter[i] = 0;
                        actors.state[i] = FLYING;
                        actors.animation_delay[i] = 8;
                        break;
                    case TYPE_SKELETON:
                        actors.counter[i] = 0;
                        actors.current_frame[i] = 0;
                        actors.state[i] = DYING;
                        actors.xDir[SKULL] = -actors.xDir[SKULL];
                        actors.yDir[SKULL] = -actors.yDir[SKULL];
                        break;
                }
            }
        }
    }
}

// Paddle collision with wall and skull
// param1: actor index
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
        if (pad1 & 0b00000011 && skull_was_beside()) {
            actors.xVelocity[SKULL] = 70;
        }
    } else {
        // Everything's fine, update x
        actors.x[pad_index] = temp_x_col;
    }

    // FRICTION
    if (actors.xSpeed[pad_index]) {
        subtract_x_speed(16);
    }
}

// param1: actor index
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
        if ((pad1 & 0b00001100) && skull_was_above()) {
            actors.yVelocity[SKULL] = 70;
        }
    } else {
        // Everything's fine, update x
        actors.y[pad_index] = temp_y_col;
    }

    // FRICTION
    if (actors.ySpeed[pad_index]) {
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
                actors.xSpeed[SKULL] = 140;
                actors.ySpeed[SKULL] = 60;
            } else if (temp_x <= actors.x[pad_index] + 10) {
                actors.xDir[SKULL] = LEFT;
                actors.xSpeed[SKULL] = 100;
                actors.ySpeed[SKULL] = 100;
            } else {
                actors.xSpeed[SKULL] = 60;
                actors.ySpeed[SKULL] = 140;
            }
        } else {
            // Right side of Paddle
            if (temp_x >= actors.x[pad_index] + actors.width[pad_index] - 4) {
                actors.xDir[SKULL] = RIGHT;
                actors.xSpeed[SKULL] = 140;
                actors.ySpeed[SKULL] = 60;
            } else if (temp_x >= actors.x[pad_index] + actors.width[pad_index] - 10) {
                actors.xDir[SKULL] = RIGHT;
                actors.xSpeed[SKULL] = 100;
                actors.ySpeed[SKULL] = 100;
            } else {
                actors.xSpeed[SKULL] = 60;
                actors.ySpeed[SKULL] = 140;
            }
        }

        // Skull going up or down?
        actors.yDir[SKULL] = actors.y[SKULL] < actors.y[pad_index] ? UP : DOWN;
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
    if (temp && is_skull_collision_paddle()) {
        // vertical paddle
        if (temp_y < actors.y[pad_index] + (actors.height[pad_index] >> 1)) {
            // We hit upper side of Paddle
            if (temp_y <= actors.y[pad_index] + 4) {
                actors.yDir[SKULL] = UP;
                actors.xSpeed[SKULL] = 60;
                actors.ySpeed[SKULL] = 140;
            } else if (temp_x <= actors.x[pad_index] + 8) {
                actors.yDir[SKULL] = UP;
                actors.xSpeed[SKULL] = 100;
                actors.ySpeed[SKULL] = 100;
            } else {
                actors.xSpeed[SKULL] = 140;
                actors.ySpeed[SKULL] = 60;
            }
        } else {
            // Lower side of Paddle
            if (temp_y >= actors.y[pad_index] + actors.height[pad_index] - 4) {
                actors.yDir[SKULL] = DOWN;
                actors.xSpeed[SKULL] = 60;
                actors.ySpeed[SKULL] = 140;
            } else if (temp_x >= actors.x[pad_index] + actors.width[pad_index] - 8) {
                actors.yDir[SKULL] = RIGHT;
                actors.xSpeed[SKULL] = 100;
                actors.ySpeed[SKULL] = 100;
            } else {
                actors.xSpeed[SKULL] = 140;
                actors.ySpeed[SKULL] = 60;
            }
        }

        // Skull going left or right?
        actors.xDir[SKULL] = actors.x[SKULL] < actors.x[pad_index] ? LEFT : RIGHT;
    }
}

void check_main_input() {
    for (pad_index = 0; pad_index < paddle_count; ++pad_index) {
        param1 = pad_index;
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
            actors.state[SKULL] = ROTATE_H;
        }
    }

    if (pad1 & PAD_B) {
        // TODO ITEM USE
    }

    if (pad1 & PAD_SELECT) {
        // TODO ITEM SELECTION...
        // stop music (or lower volume?)
        // palette fade
        // write PAUSE
        // while (on pause)
        // if already on pause resume gameplay
    }

    if (pad1_new & PAD_START) {
        game_state = MAP;
    }
}

void update_skull() {
    // TODO: We should probably check to make sure skull is within boundaries
    backup_col_type = NULL;

    if (skull_launched) {
        // Required for get_x_speed()
        param1 = SKULL;

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
                            temp = temp_x_col & 0b11111000;  // Find the x of collision tile
                            if (actors.x[SKULL] + 7 < temp) {
                                actors.xDir[SKULL] = LEFT;
                                temp_x &= 0b11111000;
                            } else {
                                actors.yDir[SKULL] = UP;
                                temp_y &= 0b11111000;
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
                            temp = temp_x_col & 0b11111000;
                            if (actors.x[SKULL] + 7 < temp) {
                                actors.xDir[SKULL] = LEFT;
                                temp_x &= 0b11111000;
                            } else {
                                actors.yDir[SKULL] = DOWN;
                                temp_y = (actors.y[SKULL] & 0b11111000) + 8;
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
                            temp = temp_x_col & 0b11111000;
                            if (actors.x[SKULL] > temp + 7) {
                                actors.xDir[SKULL] = RIGHT;
                                temp_x = actors.x[SKULL] & 0b11111000;
                            } else {
                                actors.yDir[SKULL] = UP;
                                temp_y &= 0b11111000;
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
                            temp = (temp_x_col & 0b11111000) + 7;
                            if (actors.x[SKULL] > temp) {
                                actors.xDir[SKULL] = RIGHT;
                                temp_x = temp;
                            } else {
                                actors.yDir[SKULL] = DOWN;
                                temp_y = (actors.y[SKULL] & 0b11111000) + 8;
                            }
                        }
                        do_skull_tile_collision();
                    }
                }
            }
        }

        check_paddle_collision();

        check_enemy_collision();

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

void draw_skull() {
    set_animation_info(SKULL, skull_animation_index);
    oam_meta_spr(actors.x[SKULL], actors.y[SKULL], skull_animation[actors.current_frame[SKULL] + param2]);
}

void draw_paddles() {
    for (i = 0; i < paddle_count; ++i) {
        if (i < 2) {
            oam_meta_spr(actors.x[i], actors.y[i], HorizontalPaddleSpr);
        } else {
            oam_meta_spr(actors.x[i], actors.y[i], VerticalPaddleSpr);
        }
    }
}

void draw_sprites(void) {
    oam_clear();

    draw_paddles();

    draw_skull();

    draw_level_specifics();

    // TODO draw_ennemies();
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
void fadeout(){
    if (story_counter % FADE_SPEED == 0) {
        pal_bright(--brightness);
    }
    if (brightness == 0) {
        ++story_step;
        story_counter = 0;
    }
}

// Will fade in and increment story_step
void fadein(){
    if (story_counter % FADE_SPEED == 0) {
        pal_bright(++brightness);
    }
    if (brightness == 4) {
        ++story_step;
        story_counter = 0;
    }
}

// Will brighten to white and increment story_step
void brightout(){
    if (story_counter % FADE_SPEED == 0) {
        pal_bright(++brightness);
    }
    if (brightness == 8) {
        ++story_step;
        story_counter = 0;
    }
}

void wait_input(){
    // Show waiting cursor
    if (wait_timer > 64 ) {
        one_vram_buffer (0x00, NTADR_A(30, 3));
        
    } else {
        one_vram_buffer (0x29, NTADR_A(30, 3));
    }
    // Erasing previous dialogs
    if (pad1_new) {
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

void wait(unsigned char delay){
    if (story_counter > delay){
        ++story_step;
        story_counter = 0;
    }
}

void play_story() {
    switch (current_level) {
        case 0:
            // Altar
            oam_clear();
            draw_level_specifics();
            switch (story_step) {
                case 0:
                    if (story_counter == 128) {
                        // Necromancer lift his arms
                        ++actors.state[NECROMANCER];
                        actors.current_frame[NECROMANCER] = 0;
                        story_counter = 0;
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
                        actors.x[SKULL] = 125;
                        actors.y[SKULL] = 130;
                    }
                    break;
                case 3:
                    // Waiting...
                    if (story_counter > 180) {
                        ++story_step;
                        story_counter = 0;
                    }
                    break;
                case 4:
                    // Black out
                    fadeout();
                    break;
                case 5:
                    show_map();
                    actors.x[SKULL] = 128;
                    actors.y[SKULL] = 56;
                    ++story_step;
                case 6:
                    fadein();
                    break;
                case 7:
                    story_counter = 56;
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
                    }
                    // Write cemetery
                    break;
                case 10:
                    fadeout();
                    break;
                case 11:
                    oam_clear();
                    current_level = 1;
                    story_step = 0;
                    actors.y[SKULL] = 240;
                    break;
            }
            break;
        case 1:
            // Cemetery
            // Show story sprites
            if (story_step > 6) {
                draw_sprites();
                oam_meta_spr(16, 8, angelic_face);
                set_animation_info(ANGELICA, angelic_animation_index);
                oam_meta_spr(actors.x[ANGELICA], actors.y[ANGELICA], angelic_animation[actors.current_frame[ANGELICA] + param2]);
                if (story_step > 16){
                    oam_spr(124, 132, 0x09, 0x00); // Show magnet item
                }
            }
            switch (story_step) {
                case 0:
                    load_black_level();
                    pal_bg(levels.background_palette[1]);
                    pal_bright(4);
                    ++story_step;
                    break;
                case 1:
                    wait(128);
                    break;
                case 2:
                    // Where?
                    multi_vram_buffer_horz(dialogs[0], sizeof(dialogs[0]), NTADR_A(6, 15));
                    ++story_step;
                    break;
                case 3:
                    wait(72);
                    break;
                case 4:
                    // Where am I?
                    multi_vram_buffer_horz(dialogs[1], sizeof(dialogs[1]), NTADR_A(15, 15));
                    ++story_step;
                    break;
                case 5:
                    wait(128);
                    break;
                case 6:
                    pal_bright(0);
                    brightness = 0;
                    load_level();
                    hide_map();
                    actors.x[ANGELICA] = 120;
                    actors.y[ANGELICA] = 92;
                    actors.current_frame[ANGELICA] = 0;
                    actors.animation_delay[ANGELICA] = 16;
                    actors.state[ANGELICA] = IDLE;

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
                    multi_vram_buffer_horz(dialogs[2], DIALOG_LENGTH, NTADR_A(7, 1));
                    multi_vram_buffer_horz(dialogs[3], DIALOG_LENGTH, NTADR_A(7, 2));
                    ++story_step;
                    break;
                case 10:
                    wait_input();
                    break;
                case 11:
                    // This is the start
                    multi_vram_buffer_horz(dialogs[4], DIALOG_LENGTH, NTADR_A(7, 1));
                    multi_vram_buffer_horz(dialogs[5], DIALOG_LENGTH, NTADR_A(7, 2));
                    ++story_step;
                    break;
                case 12:
                    wait_input();
                    break;
                case 13:
                    // You must go back
                    multi_vram_buffer_horz(dialogs[6], DIALOG_LENGTH, NTADR_A(7, 1));
                    multi_vram_buffer_horz(dialogs[7], DIALOG_LENGTH, NTADR_A(7, 2));
                    multi_vram_buffer_horz(dialogs[8], DIALOG_LENGTH, NTADR_A(7, 3));
                    ++story_step;
                    break;
                case 14:
                    wait_input();
                    break;
                case 15:
                    // Take this
                    multi_vram_buffer_horz(dialogs[9], DIALOG_LENGTH, NTADR_A(7, 1));
                    // Show magnet item
                    ++actors.state[STARS];
                    ++story_step;
                case 16:
                    set_animation_info(STARS, stars_animation_index);
                    oam_meta_spr(actors.x[STARS], actors.y[STARS], stars_animation[actors.current_frame[STARS] + param2]);
                    if (actors.state[STARS] == WALKING){
                        ++story_step;
                    }
                    break;
                case 17:
                    // Use it if you get stuck
                    multi_vram_buffer_horz(dialogs[10], DIALOG_LENGTH, NTADR_A(7, 1));
                    multi_vram_buffer_horz(dialogs[11], DIALOG_LENGTH, NTADR_A(7, 2));
                    ++story_step;
                    break;
                case 18:
                    wait_input();
                    break;
                case 19:
                    // Hit all tombstones...
                    multi_vram_buffer_horz(dialogs[12], DIALOG_LENGTH, NTADR_A(7, 1));
                    multi_vram_buffer_horz(dialogs[13], DIALOG_LENGTH, NTADR_A(7, 2));
                    multi_vram_buffer_horz(dialogs[14], DIALOG_LENGTH, NTADR_A(7, 3));
                    ++story_step;
                    break;
                case 20:
                    wait_input();
                    break;
                case 21:
                    brightout();
                    break;
                case 22:
                    pal_bright(4);
                    show_HUD();
                    actors.counter[6] = 250;
                    actors.counter[7] = 250;
                    game_state = MAIN;
                    break;
            }
            break;
    }
    ++story_counter;
    if (wait_timer == 127){
            wait_timer = 0;
    }
    ++wait_timer;
}

void main() {
    // famitone_init(&music_data);
    // sfx_init(&sound_data);
    // nmi_set_callback(famitone_update);
    ppu_off();

    disable_irq();

    // clear the WRAM, not done by the init code
    // memfill(void *dst,unsigned char value,unsigned int len);
    memfill(wram_array, 0, 0x2000);

    banked_call(0, init_levels);

    set_scroll_y(0xff);  // shift the bg down 1 pixel

    set_vram_buffer();

    load_title_screen();

    load_map();

    bank_spr(1);

    ppu_on_all();

    while (1) {
        ppu_wait_nmi();

        pad1 = pad_poll(0);
        pad1_new = get_pad_new(0);

        if (game_state == TITLE && pad1_new & PAD_START) {
            // PRESS START TO PLAY!

            game_state = STORY;
            current_level = 0;

            // game_state = MAIN;
            current_level = 1;

            story_step = 0;
            load_level();
        } else if (game_state == MAP && pad1_new & PAD_START) {
            // Return to gameplay
            hide_map();
            game_state = MAIN;
        } else if (game_state == STORY) {
            // STORY TIME!!!
            play_story();
        } else if (game_state == MAIN) {
            check_main_input();
            update_skull();

            draw_sprites();

            if (brick_counter == 0) {
                // Next Level!
            } else if (tombstone_count == 0) {
                //
            }

            if (game_state == MAP) {
                show_map();
            }

            // gray_line();
        }
    }
}
