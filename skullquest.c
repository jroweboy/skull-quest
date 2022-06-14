#include "skullquest.h"

#include "LIB/nesdoug.h"
#include "LIB/neslib.h"
#include "MMC3/mmc3_code.h"
#include "MMC3/mmc3_code.c"
#include "dialog.h"

// NAMETABLES
#include "I-CHR/altar.pngE/altar.h"
#include "I-CHR/cemetery.pngE/cemetery.h"
#include "I-CHR/church-interior.pngE/temple.h"
#include "I-CHR/map.pngE/map.h"
#include "I-CHR/title_screen.pngE/title_screen.h"

// PALETTES
#include "palettes.h"

// SPRITES METATILES IN BANK 1
#include "spr_general.h"
#include "spr_angelic.h"
#include "spr_crow.h"
#include "spr_hero.h"
#include "spr_lightning.h"
#include "spr_necromancer.h"
#include "spr_skeleton.h"
#include "spr_skull.h"

#pragma bss-name(push, "ZEROPAGE")
static unsigned char pad1;
static unsigned char pad1_new;

static unsigned char pad_index, temp_y_col, temp_x_col, chr_4_index, chr_5_index;
static unsigned char i, j, draw_index, param1, param2, param3, param4, temp, temp2, temp3, is_first, temp_speed, temp_x, temp_y, backup_col_type, skull_launched;
static unsigned char p1_health, p1_max_health, brick_counter, wait_timer;
static unsigned char game_state, current_level, paddle_count, story_step, story_counter;
static unsigned char animation_index, frame_count, show_face, show_item, current_item;
static unsigned char map_x, map_y, map_lvl_name_x, map_lvl_name_y;
static unsigned char brightness = 4;
static unsigned char NECROMANCER, GHOST, LIGHTNING, DEVIL, SKELETON1, SKELETON2, DOOR1, DOOR2, DOOR3, STARS;
static unsigned char CROW, GATE, CRATE1, CRATE2, CRATE3, HERO, STILL_DECORATION, SORCERER;

unsigned char const** animation_array;

static int collision_index, backup_col_index, backup_nt_index, scroll_index_y;

#pragma bss-name(pop)

#pragma bss-name(push, "XRAM")
// extra RAM at $6000-$7fff
unsigned char wram_array[0x2000];
#pragma bss-name(pop)

static char exp[] = "00000000";
static unsigned char c_map[368];

const unsigned char* current_nametable;
const unsigned char* current_collision_map;
const unsigned char* current_background_palette;
const char* current_sprite_palette;

#define ITEM_NUMBER 8
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
} Actors;

// index 0-3 paddles
// index 4 skull
// index 5 necromancer / angelica / extra skull?
// index 6-7-8-9 ennemies
// index 10 STARS animation for item reveal
// index 11-12-13 Breakables like crates...
Actors actors;

// ---------------------------- BANK 0  -------------------------------
#pragma rodata-name("BANK0")
#pragma code-name("BANK0")

// COLLISION
#include "Collision/master_collision.h"
// LEVELS
#include "I-CHR/temple2/temple2.h"
#include "I-CHR/temple3/temple3.h"
#include "I-CHR/title_screen.pngE/story.h"
#include "I-CHR/town-ruins.pngE/town_ruins.h"
#include "Nametable/black_level.h"
// SPRITES
#include "spr_devil.h"
#include "spr_door.h"
#include "spr_fireball.h"
#include "spr_skull_door.h"
#include "spr_sorcerer.h"
#include "spr_staff.h"
#include "spr_stainedglass.h"
#include "spr_star.h"
#include "spr_villagers.h"
// MAP
#include "map.h"

void reset_actors() {
    for (i = 5; i < ACTOR_NUMBER; ++i) {
        actors.state[i] = INACTIVE;
        actors.x[i] = 255;
        actors.y[i] = 240;
        actors.xDir[i] = LEFT;
        actors.yDir[i] = UP;
        actors.width[i] = NULL;
        actors.height[i] = NULL;
        actors.bbox_x[i] = NULL;
        actors.bbox_y[i] = NULL;
        actors.xSpeed[i] = NULL;
        actors.ySpeed[i] = NULL;
        actors.xRemain[i] = NULL;
        actors.yRemain[i] = NULL;
        actors.xVelocity[i] = NULL;
        actors.yVelocity[i] = NULL;
        actors.minSpeed[i] = NULL;
        actors.maxSpeed[i] = NULL;
        actors.counter[i] = NULL;
        actors.animation_delay[i] = NULL;
        actors.current_frame[i] = NULL;
        actors.type[i] = NULL;
    }
}

void init_skeletons() {
    temp = SKELETON1 + 2;  // TODO, replace hardcoded 2 with skel_number???
    for (i = SKELETON1; i < temp; ++i) {
        if (i == SKELETON1) {
            actors.x[i] = 40;
            actors.y[i] = 72;
        } else {
            actors.x[i] = 160;
            actors.y[i] = 104;
            actors.xDir[i] = RIGHT;
        }
        actors.width[i] = 0x04;
        actors.height[i] = 0x14;
        actors.bbox_x[i] = 0x01;
        actors.bbox_y[i] = 0x02;
        actors.xSpeed[i] = 10;
        actors.maxSpeed[i] = 20;
        actors.animation_delay[i] = 16;
        actors.state[i] = DEAD;
        actors.type[i] = TYPE_SKELETON;
    }
}

void set_torch(unsigned char p_index, unsigned char p_x, unsigned char p_y) {
    actors.x[p_index] = p_x;
    actors.y[p_index] = p_y;
    actors.animation_delay[p_index] = 8;
    actors.state[p_index] = IDLE;
    actors.type[p_index] = TYPE_TORCHES;
}

void set_animation_info() {
    j = actors.state[draw_index] << 1;
    switch (actors.type[draw_index]) {
        case TYPE_SKELETON:
            animation_index = skeleton_animation_index[j];
            ++j;
            frame_count = skeleton_animation_index[j];
            animation_array = skeleton_animation;
            break;
        case TYPE_GLASS:
            animation_index = glass_animation_index[j];
            ++j;
            frame_count = glass_animation_index[j];
            animation_array = glass_animation;
            break;
        case TYPE_CRATE:
            animation_index = 0;
            frame_count = 1;
            animation_array = crate_animation;
            break;
        case TYPE_HOUSE_DOOR:
            animation_index = house_door_animation_index[j];
            ++j;
            frame_count = house_door_animation_index[j];
            animation_array = house_door_animation;
            break;
        case TYPE_CROW:
            animation_index = crow_animation_index[j];
            ++j;
            frame_count = crow_animation_index[j];
            animation_array = crow_animation;
            break;
        case TYPE_DEVIL:
            animation_index = devil_animation_index[j];
            ++j;
            frame_count = devil_animation_index[j];
            animation_array = devil_animation;
            break;
        case TYPE_SKULL:
            animation_index = skull_animation_index[j];
            ++j;
            frame_count = skull_animation_index[j];
            animation_array = skull_animation;
            break;
        case TYPE_GATE:
            animation_index = gate_animation_index[j];
            ++j;
            frame_count = gate_animation_index[j];
            animation_array = gate_animation;
            break;
        case TYPE_TREE:
            animation_index = 0;
            frame_count = 1;
            animation_array = tree_animation;
            break;
        case TYPE_ANGELIC:
            animation_index = angelic_animation_index[j];
            ++j;
            frame_count = angelic_animation_index[j];
            animation_array = angelic_animation;
            break;
        case TYPE_SKULL_PILE:
            animation_index = 0;
            frame_count = 1;
            animation_array = skull_pile_animation;
            break;
        case TYPE_LIGHTNING:
            animation_index = lightning_animation_index[j];
            ++j;
            frame_count = lightning_animation_index[j];
            animation_array = lightning_animation;
            break;
        case TYPE_NECROMANCER:
            animation_index = necromancer_animation_index[j];
            ++j;
            frame_count = necromancer_animation_index[j];
            animation_array = necromancer_animation;
            break;
        case TYPE_STARS:
            animation_index = stars_animation_index[j];
            ++j;
            frame_count = stars_animation_index[j];
            animation_array = stars_animation;
            break;
        case TYPE_VILLAGER1:
            animation_index = villager_animation_index[j];
            ++j;
            frame_count = villager_animation_index[j];
            animation_array = villager_animation;
            break;
        case TYPE_TORCHES:
            animation_index = torch_animation_index[j];
            ++j;
            frame_count = torch_animation_index[j];
            animation_array = torch_animation;
            break;
        case TYPE_HERO:
            animation_index = hero_animation_index[j];
            ++j;
            frame_count = hero_animation_index[j];
            animation_array = hero_animation;
            break;
        case TYPE_SORCERER:
            animation_index = sorcerer_animation_index[j];
            ++j;
            frame_count = sorcerer_animation_index[j];
            animation_array = sorcerer_animation;
            break;
        case TYPE_PARALYZER:
            animation_index = fireball_animation_index[j];
            ++j;
            frame_count = fireball_animation_index[j];
            animation_array = fireball_animation;
            break;
        case TYPE_SKELETON_KING:
            animation_index = 0;
            frame_count = 1;
            animation_array = skeleton_king_animation;
            break;
        case TYPE_SKULL_DOOR:
            animation_index = skull_door_animation_index[j];
            ++j;
            frame_count = skull_door_animation_index[j];
            animation_array = skull_door_animation;
            break;
    }
}

void init_level_specifics() {
    switch (current_level) {
        case LVL_ALTAR:
            current_nametable = altar;
            current_collision_map = cemetery_col;
            current_background_palette = pal_altar_bg;
            current_sprite_palette = pal_altar_spr;
            chr_4_index = 2;

            // Skull doesn't appear immediately
            actors.x[SKULL] = 125;
            actors.y[SKULL] = 130;
            actors.state[SKULL] = INACTIVE;

            // Lightning
            LIGHTNING = 12;
            actors.x[LIGHTNING] = 128;
            actors.y[LIGHTNING] = 108;
            actors.animation_delay[LIGHTNING] = 8;
            actors.type[LIGHTNING] = TYPE_LIGHTNING;
            actors.state[LIGHTNING] = IDLE;

            // Torches:
            set_torch(7, 64, 72);
            set_torch(8, 88, 72);
            set_torch(9, 160, 72);
            set_torch(10, 184, 72);

            // Necromancer:
            NECROMANCER = 6;
            actors.x[NECROMANCER] = 120;
            actors.y[NECROMANCER] = 142;
            actors.animation_delay[NECROMANCER] = 20;
            actors.state[NECROMANCER] = IDLE;
            actors.type[NECROMANCER] = TYPE_NECROMANCER;

            // Hero
            HERO = 11;
            actors.animation_delay[HERO] = 1;
            actors.x[HERO] = 120;
            actors.y[HERO] = 72;
            actors.state[HERO] = IDLE;
            actors.type[HERO] = TYPE_HERO;
            break;
        case LVL_CEMETERY:
            // Achievement 1 : Scarecrow
            // Achievement 2 : Skeleton buster

            current_nametable = cemetery;
            current_collision_map = cemetery_col;
            current_background_palette = pal_cemetery_bg;
            current_sprite_palette = pal_cemetery_spr;

            chr_4_index = 2;
            chr_5_index = 3;

            // Paddle
            paddle_count = 1;
            actors.x[0] = 0x70;
            actors.y[0] = 0xD0;
            actors.state[0] = PAD_LONG;
            actors.type[0] = TYPE_PAD_HORZ;

            // CROW
            CROW = 5;
            actors.x[CROW] = 207;
            actors.y[CROW] = 117;
            actors.state[CROW] = 2;  // IDLE state of crow is 2... don't ask!
            actors.animation_delay[CROW] = 64;
            actors.xSpeed[CROW] = 128;
            actors.ySpeed[CROW] = 32;
            actors.type[CROW] = TYPE_CROW;
            actors.bbox_y[CROW] = 4;
            actors.width[CROW] = 16;
            actors.height[CROW] = 16;

            // Skeleton index 6 & 7
            SKELETON1 = 6;
            SKELETON2 = 7;
            banked_call(0, init_skeletons);

            // GATE
            GATE = 8;
            actors.x[GATE] = 120;
            actors.y[GATE] = 48;
            actors.animation_delay[GATE] = 16;
            actors.type[GATE] = TYPE_GATE;
            actors.state[GATE] = IDLE;

            // TREE
            STILL_DECORATION = 9;
            actors.x[STILL_DECORATION] = 219;
            actors.y[STILL_DECORATION] = 61;
            actors.state[STILL_DECORATION] = IDLE;
            actors.type[STILL_DECORATION] = TYPE_TREE;

            // ANGELIC
            GHOST = 10;
            actors.x[GHOST] = 120;
            actors.y[GHOST] = 92;
            actors.animation_delay[GHOST] = 16;
            actors.state[GHOST] = IDLE;
            actors.type[GHOST] = TYPE_ANGELIC;

            // STARS
            STARS = 11;
            actors.x[STARS] = 127;
            actors.y[STARS] = 136;
            actors.state[STARS] = IDLE;
            actors.type[STARS] = TYPE_STARS;
            actors.animation_delay[STARS] = 8;
            break;
        case LVL_TEMPLE1:
            // Achievement 1: Glass Hash (break 3 stained glasses)
            // Achievement 2: Paralyzed by Death (waking the sorcerer by hiting pile of skulls)

            current_nametable = temple;
            current_collision_map = temple_col;
            current_background_palette = pal_temple_bg;
            current_sprite_palette = pal_temple_spr;

            chr_4_index = 0x0A;
            chr_5_index = 0x0B;

            // Paddles
            paddle_count = 1;
            actors.x[0] = 0x70;  // 14
            actors.y[0] = 0xD0;  // 26
            actors.state[0] = PAD_LONG;
            actors.type[0] = TYPE_PAD_HORZ;

            // Stained glasses
            actors.x[6] = 96;
            actors.x[7] = 120;
            actors.x[8] = 144;
            for (i = 6; i < 9; ++i) {
                actors.animation_delay[i] = 12;
                actors.y[i] = 32;
                actors.type[i] = TYPE_GLASS;
                actors.width[i] = 16;
                actors.height[i] = 50;
                actors.state[i] = IDLE;
            }

            // Pile of skulls
            STILL_DECORATION = 10;
            actors.x[STILL_DECORATION] = 184;
            actors.y[STILL_DECORATION] = 82;
            actors.width[STILL_DECORATION] = 16;
            actors.height[STILL_DECORATION] = 8;
            actors.state[STILL_DECORATION] = IDLE;
            actors.type[STILL_DECORATION] = TYPE_SKULL_PILE;

            // Sorcerer
            SORCERER = 9;
            actors.x[SORCERER] = actors.x[STILL_DECORATION] + 8;
            actors.y[SORCERER] = actors.y[STILL_DECORATION] - 20;
            actors.animation_delay[SORCERER] = 12;
            actors.state[SORCERER] = DEAD;
            actors.type[SORCERER] = TYPE_SORCERER;
            break;
        case LVL_TEMPLE2:
            // Achievement 1:
            // Achievement 2:
            current_nametable = temple2;
            current_collision_map = temple2_col;
            current_background_palette = pal_temple_bg;
            current_sprite_palette = pal_temple_spr;

            chr_4_index = 0x0A;
            chr_5_index = 0x0B;

            paddle_count = 2;
            // Paddle 1
            actors.x[0] = 0x70;
            actors.y[0] = 0xC0;
            actors.state[0] = PAD_LONG;
            actors.type[0] = TYPE_PAD_HORZ;

            // Paddle 2
            actors.x[1] = 0x70;
            actors.y[1] = 0x48;
            actors.state[0] = PAD_LONG;
            actors.type[0] = TYPE_PAD_HORZ;
            break;
        case LVL_TEMPLE3:
            // Achievement 1:
            // Achievement 2:
            current_nametable = temple3;
            current_collision_map = temple3_col;
            current_background_palette = pal_temple_bg;
            current_sprite_palette = pal_temple_spr;

            chr_4_index = 0x0A;
            chr_5_index = 0x0B;

            paddle_count = 4;
            // Paddle 1
            actors.x[0] = 15 * 8;
            actors.y[0] = 12 * 8;
            actors.state[0] = PAD_SHORT;
            actors.type[0] = TYPE_PAD_HORZ;

            // Paddle up
            actors.x[1] = 15 * 8;
            actors.y[1] = 21 * 8;
            actors.state[1] = PAD_SHORT;
            actors.type[1] = TYPE_PAD_HORZ;

            // Paddle left
            actors.x[2] = 8 * 8;
            actors.y[2] = 16 * 8;
            actors.state[2] = PAD_SHORT;
            actors.type[2] = TYPE_PAD_VERT;

            // Paddle right
            actors.x[3] = 23 * 8;
            actors.y[3] = 16 * 8;
            actors.state[3] = PAD_SHORT;
            actors.type[3] = TYPE_PAD_VERT;

            // Skull Door
            DOOR1 = 6;
            actors.x[DOOR1] = 120;
            actors.y[DOOR1] = 21;
            actors.type[DOOR1] = TYPE_SKULL_DOOR;
            actors.state[DOOR1] = IDLE;
            actors.animation_delay[DOOR1] = 18;
            break;

            // Torches
            set_torch(7, 104, 32);
            set_torch(8, 144, 32);

        case LVL_TOWN:
            // Achievement 1: Door knocker
            // Achievement 2: Devil slayer

            current_nametable = town_ruins;
            current_collision_map = town_col;
            current_background_palette = pal_town_bg;
            current_sprite_palette = pal_town_spr;

            chr_4_index = 0x0a;
            chr_5_index = 0x0b;

            // Paddle
            paddle_count = 1;
            actors.x[0] = 0x70;
            actors.y[0] = 0xD0;

            // Doors
            DOOR1 = 6;
            DOOR2 = 7;
            DOOR3 = 8;
            actors.x[DOOR1] = 30;
            actors.y[DOOR1] = 87;
            actors.width[DOOR1] = 16;
            actors.height[DOOR1] = 50;
            actors.state[DOOR1] = IDLE;
            actors.type[DOOR1] = TYPE_HOUSE_DOOR;

            actors.x[DOOR2] = 110;
            actors.y[DOOR2] = 71;
            actors.width[DOOR2] = 16;
            actors.height[DOOR2] = 50;
            actors.state[DOOR2] = IDLE;
            actors.type[DOOR2] = TYPE_HOUSE_DOOR;

            actors.x[DOOR3] = 182;
            actors.y[DOOR3] = 87;
            actors.width[DOOR3] = 16;
            actors.height[DOOR3] = 50;
            actors.state[DOOR3] = IDLE;
            actors.type[DOOR3] = TYPE_HOUSE_DOOR;

            // Crates
            CRATE1 = 9;
            CRATE2 = 10;
            CRATE3 = 11;

            actors.x[CRATE1] = 16;
            actors.y[CRATE1] = 112;
            actors.x[CRATE2] = 48;
            actors.y[CRATE2] = 112;
            actors.x[CRATE3] = 224;
            actors.y[CRATE3] = 112;
            temp = 15;
            actors.width[CRATE1] = temp;
            actors.width[CRATE2] = temp;
            actors.width[CRATE3] = temp;
            actors.height[CRATE1] = temp;
            actors.height[CRATE2] = temp;
            actors.height[CRATE3] = temp;
            temp = 1;
            actors.bbox_x[CRATE1] = temp;
            actors.bbox_y[CRATE1] = temp;
            actors.bbox_x[CRATE2] = temp;
            actors.bbox_y[CRATE2] = temp;
            actors.bbox_x[CRATE3] = temp;
            actors.bbox_y[CRATE3] = temp;
            actors.state[CRATE1] = IDLE;
            actors.state[CRATE2] = IDLE;
            actors.state[CRATE3] = IDLE;
            actors.type[CRATE1] = TYPE_CRATE;
            actors.type[CRATE2] = TYPE_CRATE;
            actors.type[CRATE3] = TYPE_CRATE;

            // DEVIL
            DEVIL = 12;
            actors.x[DEVIL] = 12;
            actors.y[DEVIL] = 152;
            actors.width[DEVIL] = 15;
            actors.height[DEVIL] = 15;
            actors.animation_delay[DEVIL] = 16;
            actors.state[DEVIL] = IDLE;
            actors.type[DEVIL] = TYPE_DEVIL;
            break;
    }

    for (i = 0; i < paddle_count; ++i) {
        // The first 2 paddles are horizontal, the others vertical
        if (i < 2) {
            actors.width[i] = 0x20;   // 32
            actors.height[i] = 0x04;  // 4
            actors.bbox_y[i] = 0x02;
        } else {
            actors.width[i] = 0x04;
            actors.height[i] = 0x20;
            actors.bbox_x[i] = 0x02;
        }
        actors.maxSpeed[i] = 250;
    }
}

void init_skull() {
    actors.width[SKULL] = 6;
    actors.height[SKULL] = 6;
    actors.bbox_x[SKULL] = 1;
    actors.bbox_y[SKULL] = 1;
    actors.xDir[SKULL] = RIGHT;
    actors.yDir[SKULL] = UP;
    actors.xSpeed[SKULL] = 100;
    actors.ySpeed[SKULL] = 100;
    actors.minSpeed[SKULL] = 64;
    actors.maxSpeed[SKULL] = 250;
    actors.animation_delay[SKULL] = 8;
    actors.type[SKULL] = TYPE_SKULL;
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

    // COINS
    // TODO update_coins();

    // OTHER??
}

void show_game_over() {
    // TODO
}

void load_black_level() {
    ppu_off();
    // set_scroll_x(0x0000);

    vram_adr(NAMETABLE_A);
    vram_unrle(black_level);
    ppu_on_all();
}

// We assume there will never be a col type value greater than 15;
void load_collision() {
    collision_index = 0;
    is_first = TRUE;
    i = 0;
    temp = current_collision_map[i];
    // Array terminated by 255;
    while (temp < 255) {
        ++i;
        // of which type
        temp2 = current_collision_map[i];
        for (j = 0; j < temp; ++j) {
            if (is_first) {
                temp3 = temp2 << 4;
                is_first = FALSE;
            } else {
                c_map[collision_index] = temp3 | temp2;
                ++collision_index;
                is_first = TRUE;
            }
        }
        ++i;
        // how many
        temp = current_collision_map[i];
    }
}

void load_level() {
    ppu_off();
    set_scroll_x(0x0000);

    banked_call(0, reset_actors);
    banked_call(0, init_level_specifics);

    vram_adr(NAMETABLE_A);
    vram_unrle(current_nametable);

    pal_bg(current_background_palette);
    pal_spr(current_sprite_palette);

    load_collision();

    // Check how many tombstones or destructible bricks there are
    brick_counter = 0;
    for (collision_index = 0; collision_index < 368; ++collision_index) {
        // collision_index contient 2 info de 4 bits
        // First 4 bits:
        temp = c_map[collision_index] >> 4;
        if (temp > 2 && temp < 8) {
            ++brick_counter;
        }
        // Last 4 bits:
        temp = c_map[collision_index] & 0x0F;
        if (temp > 2 && temp < 8) {
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

    vram_adr(NAMETABLE_A);
    vram_unrle(title_screen);

    vram_adr(NAMETABLE_C);
    vram_unrle(story);

    game_state = TITLE;
}

void remove_brick(char tile_type) {
    one_vram_buffer(tile_type, backup_nt_index);
    c_map[backup_col_index] &= backup_nt_index % 2 ? 0b11110000 : 0b00001111;
    --brick_counter;
}

void hit_brick(char tile_type) {
    remove_brick(tile_type);
    // Play sound
    // Update score
    // Spawn random coin ??
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

// draw_index: already defined in the loop
// pad_index: object to verify collision
char has_collision() {
    return (actors.x[draw_index] + actors.bbox_x[draw_index] < actors.x[pad_index] + actors.width[pad_index] + actors.bbox_x[pad_index] &&
            actors.x[draw_index] + actors.bbox_x[draw_index] + actors.width[draw_index] > actors.x[pad_index] + actors.bbox_x[pad_index] &&
            actors.y[draw_index] + actors.bbox_y[draw_index] < actors.y[pad_index] + actors.height[pad_index] + actors.bbox_y[pad_index] &&
            actors.y[draw_index] + actors.bbox_y[draw_index] + actors.height[draw_index] > actors.y[pad_index] + actors.bbox_y[pad_index]);
}

unsigned char get_collision_type() {
    collision_index = (temp_x_col >> 4) + (((temp_y_col >> 3) - 5) * 16);
    return (temp_x_col >> 3) % 2 ? c_map[collision_index] & 0x0F : c_map[collision_index] >> 4;
}

unsigned char set_collision_data() {
    backup_nt_index = NTADR_A((temp_x_col >> 3), (temp_y_col >> 3));
    backup_col_type = get_collision_type();
    backup_col_index = collision_index;
    return backup_col_type;
}

// Don't forget to reset the current frame when changing state.
void animate() {
    banked_call(0, set_animation_info);

    if (actors.counter[draw_index] == actors.animation_delay[draw_index]) {
        // STATE odd && animation finished
        if ((actors.state[draw_index] % 2 != 0) && actors.current_frame[draw_index] == frame_count - 1) {
            // NEXT STATE
            ++actors.state[draw_index];
            banked_call(0, set_animation_info);
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
            // CROW
            if (actors.state[CROW] == FLYING) {
                param1 = CROW;
                actors.x[CROW] += get_x_speed();
                actors.y[CROW] += get_y_speed();
                if (actors.x[CROW] < 6) {
                    actors.state[CROW] = INACTIVE;
                }
            } else if (actors.y[SKULL] > 120 && actors.y[SKULL] < 132) {
                actors.state[CROW] = SKWAK;  // SKWAK!
            }
            break;
        case TYPE_SORCERER:
            if (actors.state[SORCERER] == ATTACKING) {
                ++actors.state[SORCERER];
                // Find empty actor slot
                for (i = 6; i < ACTOR_NUMBER; ++i) {
                    if (actors.state[i] == INACTIVE) {
                        actors.x[i] = actors.x[SORCERER] + 8;
                        actors.y[i] = actors.y[SORCERER] + 16;
                        actors.width[i] = 8;
                        actors.height[i] = 8;
                        actors.animation_delay[i] = 8;
                        actors.yDir[i] = DOWN;
                        actors.state[i] = APPEARING;
                        actors.type[i] = TYPE_PARALYZER;
                        actors.ySpeed[i] = 128;

                        // Get the right direction and speed for PARALYZER
                        if (actors.x[i] > actors.x[0]) {
                            actors.xDir[i] = LEFT;
                            actors.xSpeed[i] = actors.x[i] - actors.x[0];
                        } else {
                            actors.xDir[i] = RIGHT;
                            actors.xSpeed[i] = actors.x[0] - actors.x[i];
                        }
                        break;
                    }
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
    }
}

void animate_actors() {
    for (draw_index = SKULL; draw_index < ACTOR_NUMBER; ++draw_index) {
        if (actors.state[draw_index] != INACTIVE) {
            animate();
            move();
        }
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
            // TODO!!!
            // Check death
            // play hurt or death sound ?
            // shake screen ??
            // invincible for a few frames ??
            break;
        case 0x03:
            // Long brick
            remove_brick(TILE_BACK);
            backup_nt_index % 2 ? --backup_nt_index : ++backup_nt_index;
            hit_brick(TILE_BACK);
            add_xp(1, HUNDREDS);
            break;
        case 0x04:
            // Tall brick
            remove_brick(TILE_BACK);
            temp = temp_y_col >> 3;
            if (temp % 2) {
                temp -= 1;
                backup_col_index -= 16;
            } else {
                temp += 1;
                backup_col_index += 16;
            }
            backup_nt_index = NTADR_A(temp_x_col >> 3, temp);
            hit_brick(TILE_BACK);
            add_xp(1, HUNDREDS);
            break;
        case 0x05:
            // Dot brick
            hit_brick(TILE_BACK);
            add_xp(5, TENS);
            break;
        case 0x06:
            // 2-Hit Brick
            // TODO
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
            brick_counter -= 4;  // TODO Dynamic tombstone size to allow more design ideas...
            break;
        case 0x08:
            // Foliage
            hit_brick(TILE_BACK_GRASS);
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
    for (i = 5; i < ACTOR_NUMBER; ++i) {
        if (actors.state[i] != INACTIVE && actors.state[i] != DEAD) {
            pad_index = i;
            if (actors.state[i] != DYING && is_skull_collision_paddle()) {
                switch (actors.type[i]) {
                    case TYPE_CROW:
                        actors.counter[i] = 0;
                        actors.state[i] = FLYING;
                        actors.animation_delay[i] = 8;
                        break;
                    case TYPE_SKELETON:
                        // TODO Verify if skeleton already dead...
                        actors.counter[i] = 0;
                        actors.current_frame[i] = 0;
                        actors.state[i] = DYING;
                        actors.xDir[SKULL] = -actors.xDir[SKULL];
                        actors.yDir[SKULL] = -actors.yDir[SKULL];
                        break;
                    case TYPE_GLASS:
                        // Play sound
                        if (actors.state[i] == IDLE) {
                            ++actors.state[i];
                        }
                        break;
                    case TYPE_CRATE:
                        // Play sound
                        // TODO Random power up appear?
                        if (skull_was_beside()) {
                            actors.xDir[SKULL] = -actors.xDir[SKULL];
                        } else {
                            actors.yDir[SKULL] = -actors.yDir[SKULL];
                        }
                        break;
                    case TYPE_HOUSE_DOOR:
                        // Play sound
                        if (actors.state[i] == IDLE) {
                            ++actors.state[i];
                        }
                        break;
                    case TYPE_SKULL_PILE:
                        if (actors.state[SORCERER] == DEAD) {
                            actors.state[SORCERER] = APPEARING;
                            if (actors.x[SORCERER] > actors.x[0]) {
                                actors.xDir[SORCERER] = LEFT;
                            } else {
                                debug(0x30);
                                actors.xDir[SORCERER] = RIGHT;
                            }
                        }
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
    for (pad_index = 0; pad_index < paddle_count; ++pad_index) {
        if (is_skull_collision_paddle()) {
            if (actors.type[pad_index] == TYPE_PAD_HORZ) {
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
            } else {
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
    }
}

void check_main_input() {
    for (pad_index = 0; pad_index < paddle_count; ++pad_index) {
        param1 = pad_index;
        if (actors.type[pad_index] == TYPE_PAD_HORZ) {
            // Horizontal paddle
            if (actors.counter[pad_index]) {
                --actors.counter[pad_index];
            } else {
                if (pad1 & PAD_LEFT) {
                    add_x_speed(80);
                    actors.xDir[pad_index] = LEFT;
                }
                if (pad1 & PAD_RIGHT) {
                    add_x_speed(80);
                    actors.xDir[pad_index] = RIGHT;
                }
            }
            move_horizontal_paddle();
        } else {
            if (actors.counter[pad_index]) {
                --actors.counter[pad_index];
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

    if (pad1_new & PAD_B) {
        switch (items.type[current_item]) {
            case TYPE_MAGNET:
                actors.yDir[SKULL] = DOWN;
                actors.xDir[SKULL] = actors.x[SKULL] < actors.x[0] ? RIGHT : LEFT;
                break;
        }
    }

    if (pad1_new & PAD_SELECT) {
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
                            temp = temp_y_col - (temp_y_col % 8);
                            temp2 = actors.y[SKULL] + 7;
                            temp2 -= temp2 % 8;
                            if (temp == temp2) {
                                actors.xDir[SKULL] = LEFT;
                                temp_x = actors.x[SKULL];
                            } else {
                                actors.yDir[SKULL] = UP;
                                temp_y = actors.y[SKULL];
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
                            temp = temp_y_col - (temp_y_col % 8);
                            temp2 = actors.y[SKULL];
                            ++temp2;  // BBox
                            temp2 -= temp2 % 8;
                            if (temp == temp2) {
                                actors.xDir[SKULL] = LEFT;
                                temp_x = actors.x[SKULL];
                            } else {
                                actors.yDir[SKULL] = DOWN;
                                temp_y = actors.y[SKULL];
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
                            temp = temp_y_col - (temp_y_col % 8);
                            temp2 = actors.y[SKULL] + 7;
                            temp2 -= temp2 % 8;
                            if (temp == temp2) {
                                actors.xDir[SKULL] = RIGHT;
                                temp_x = actors.x[SKULL];
                            } else {
                                actors.yDir[SKULL] = UP;
                                temp_y = actors.y[SKULL];
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
                            temp = temp_y_col - (temp_y_col % 8);
                            temp2 = actors.y[SKULL];
                            ++temp2;  // BBox
                            temp2 -= temp2 % 8;
                            if (temp == temp2) {
                                actors.xDir[SKULL] = RIGHT;
                                temp_x = actors.x[SKULL];
                            } else {
                                actors.yDir[SKULL] = DOWN;
                                temp_y = actors.y[SKULL];
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

    animate_actors();
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

void wait(unsigned char delay) {
    if (story_counter > delay) {
        ++story_step;
        story_counter = 0;
    }
}

void play_normal_level() {
    oam_clear();
    switch (story_step) {
        case 0:
            fadein();
            break;
        case 1:
            game_state = MAIN;
            show_HUD();
            ++story_step;
            break;
        case 2:
            wait(64);
            break;
        case 3:
            fadeout();
            break;
        case 4:
            ++current_level;
            load_level();
            story_step = 0;
            break;
    }
}

void play_story() {
    switch (current_level) {
        case LVL_ALTAR:
            oam_clear();
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
                    reset_actors();
                    banked_call(0, set_map);
                    set_scroll_y(0x0100);
                    actors.x[SKULL] = 128;
                    actors.y[SKULL] = 56;
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
                    banked_call(0, hide_map);
                    actors.state[SKULL] = INACTIVE;
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
            oam_clear();
            if (story_step > 6) {
                draw_sprites();
            }
            if (show_face) {
                oam_meta_spr(FACE_X, FACE_Y, angelic_face);
            }
            if (show_item) {
                oam_spr(123, 130, ITEM_MAGNET, 0);
            }
            switch (story_step) {
                case 0:
                    load_black_level();
                    pal_bg(pal_cemetery_bg);
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
                    // Take this!
                    multi_vram_buffer_horz(dialogs[9], DIALOG_LENGTH, NTADR_A(7, 1));
                    ++actors.state[STARS];
                    show_item = TRUE;
                    items.type[0] = TYPE_MAGNET;
                    items.is_active[0] = TRUE;
                    items.sprite[0] = ITEM_MAGNET;
                    current_item = 0;
                    ++story_step;
                    break;
                case 16:
                    wait_input();
                    break;
                case 17:
                    // Use it
                    multi_vram_buffer_horz(dialogs[10], DIALOG_LENGTH, NTADR_A(7, 1));
                    multi_vram_buffer_horz(dialogs[11], DIALOG_LENGTH, NTADR_A(7, 2));
                    multi_vram_buffer_horz(dialogs[12], DIALOG_LENGTH, NTADR_A(7, 3));
                    ++story_step;
                    break;
                case 18:
                    wait_input();
                    break;
                case 19:
                    // Hit all tombstones...
                    multi_vram_buffer_horz(dialogs[13], DIALOG_LENGTH, NTADR_A(7, 1));
                    multi_vram_buffer_horz(dialogs[14], DIALOG_LENGTH, NTADR_A(7, 2));
                    multi_vram_buffer_horz(dialogs[15], DIALOG_LENGTH, NTADR_A(7, 3));
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
        case LVL_TOWN:
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
                    oam_clear();
                    load_black_level();
                    ++story_step;
                    break;
                case 3:
                    // Prepare the graphics
                    // set_chr_mode_1(0x0c); // for lightning
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
                    scroll_index_y = add_scroll_y(1, scroll_index_y);
                    if (story_counter > 10) {
                        story_counter = NULL;
                    }
                    ++story_counter;
                    if (scroll_index_y > 460) {
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
                    banked_call(0, load_map);
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
    banked_call(0, load_map);
    pal_bright(NULL);
    brightness = NULL;
    items.sprite[current_item] = ITEM_MAGNET;
    items.is_active[current_item] = TRUE;
    items.type[current_item] = TYPE_MAGNET;
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

    // set_scroll_y(0xff);  // shift the bg down 1 pixel

    // color_emphasis(COL_EMP_NORMAL);

    set_mirroring(MIRROR_HORIZONTAL);

    set_vram_buffer();

    load_title_screen();

    banked_call(0, init_skull);

    bank_spr(1);
    oam_meta_spr(182, 122, staff);

    ppu_on_all();

    while (1) {
        ppu_wait_nmi();

        pad1 = pad_poll(0);
        pad1_new = get_pad_new(0);

        if (game_state == TITLE) {
            // SHOW STAFF
            oam_meta_spr(182, 122, staff);

            // TODO Show skull cursor sprite

            // Player's choice
            // TODO  Continue or New Game

            // PRESS START TO PLAY!
            if (pad1_new & PAD_START) {
                game_state = STORY;
                current_level = LVL_INTRO;
                story_step = NULL;
                scroll_index_y = NULL;

                // DEBUG
                debug_start(LVL_TEMPLE3);
            }

        } else if (game_state == MAP) {
            // DRAW SKULL AT CORRECT POSITION
            oam_spr(1, 1, 0x00, 0x00);
            if (pad1_new & PAD_START) {
                // Return to gameplay
                banked_call(0, hide_map);
                game_state = MAIN;
            }
        } else if (game_state == STORY) {
            // STORY TIME!!!
            play_story();
        } else if (game_state == MAIN) {
            check_main_input();

            update_skull();

            draw_sprites();

            // CURRENT ITEM:
            oam_spr(ITEM_X, ITEM_Y, items.sprite[current_item], 0);

            if (brick_counter == 0) {
                game_state = STORY;
            }

            if (game_state == MAP) {
                banked_call(0, show_map);
            }

            // gray_line();
        }
    }
}
