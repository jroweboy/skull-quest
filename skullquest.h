
#ifndef _SKULLQUEST_H
#define _SKULLQUEST_H

#define ONES 7
#define TENS 6
#define HUNDREDS 5
#define THOUSANDS 4
#define TEN_THOUSANDS 3
#define HUNDRED_THOUSANDS 2
#define MILLIONS 1
#define TEN_MILLIONS 0

#define STORY_LEVEL_START 0
#define STORY_LEVEL_EVENT 20
#define STORY_LEVEL_END 150

#define MAX_HEALTH 8
#define SPEED_STEP 1
#define TILE_BACK 0x01
#define TILE_BACK_GRASS 0x18
#define TILE_HEART_FULL 0x5B
#define TILE_HEART_EMPTY 0x5C

#define COL_TYPE_SOLID 0x01
#define COL_TYPE_HURT 0x02
#define COL_TYPE_LONG 0x03
#define COL_TYPE_TALL 0x04
#define COL_TYPE_SMALL 0x05
#define COL_TYPE_SOFT 0x06
#define COL_TYPE_TOMBSTONE 0x07
#define COL_TYPE_BOMBABLE 0x08
#define COL_TYPE_2HIT_LONG 0x09
#define COL_TYPE_2HIT_SMALL 0x0a
#define COL_TYPE_0B 0x0b
#define COL_TYPE_0C 0x0c
#define COL_TYPE_0D 0x0d
#define COL_TYPE_0E 0x0e
#define COL_TYPE_0F 0x0f

// Fade speed
#define FADE_SPEED 24

#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1

#define MAIN 0
#define INVENTORY 1
#define MAP 2
#define STORY 3
#define GAME_OVER 4
#define TITLE 5

// ACTOR INDEX
#define SKULL 5
#define CURSOR 13
#define PADDLE 0

#define CURSOR_SMALL 0
#define CURSOR_LARGE 2
#define SAVE_BUTTON_X 40
#define SAVE_BUTTON_Y 104
#define INVENTORY_ITEM_Y 32

// Actor STATES!
// Always putting temporary before the next (ex DYING -> DEAD)
#define IDLE 0
#define FLYING 0
#define TURNING 1
#define SKWAK 1
#define OPENING 1
#define APPEARING 1
#define ATTACKING 2
#define WALKING 2
#define IDLE2 2
#define OPENED 2
#define DISAPPEARING 3
#define CHASING 4
#define RISING 5
#define EATING 5
#define CHEWING 6
#define PARALYZED 5
#define KIDNAPPING 6
#define RETURNING 8
#define CROW_EAT 9
#define INACTIVE 128

#define ROTATE_H 2

#define DYING 3
#define DEAD 4

// Actor types
#define TYPE_SKELETON 1
#define TYPE_GLASS 2
#define TYPE_CRATE 3
#define TYPE_HOUSE_DOOR 4
#define TYPE_CROW 5
#define TYPE_DEVIL 6
#define TYPE_SKULL 7
#define TYPE_GATE 8
#define TYPE_TREE 9
#define TYPE_ANGELIC 10
#define TYPE_SKULL_PILE 11
#define TYPE_LIGHTNING 12
#define TYPE_NECROMANCER 13
#define TYPE_STARS 14
#define TYPE_VILLAGER1 15
#define TYPE_TORCHES 16
#define TYPE_HERO 17
#define TYPE_SORCERER 18
#define TYPE_PARALYZER 19
#define TYPE_SKELETON_KING 20
#define TYPE_SKULL_DOOR 21
// ITEMS
#define TYPE_ITEM_MAGNET 22
#define TYPE_ITEM_BOMB 23
#define TYPE_ITEM_HOOK 24
#define TYPE_ITEM_BIG 25
#define TYPE_ITEM_VOLT 26
#define TYPE_ITEM_HEAL 27
#define TYPE_ITEM_SEED 28
#define TYPE_ITEM_X 29
#define TYPE_ITEM_Y 30
// 
#define TYPE_CURSOR 31
#define TYPE_SCARECROW 32
#define TYPE_TRIGGER 33
#define TYPE_TRIGGER2 34
#define TYPE_CANCEL_BTN 35


// ACHIEVEMENTS CHECKBOX COORD
// CHECKBOX1 6, 26
// CHECKBOX2 6, 27

// PADDLE
#define TYPE_PAD_HORZ 0
#define TYPE_PAD_VERT 1
#define PAD_SHORT 16
#define PAD_LONG 32
#define PAD_VELOCITY 80
#define FACE_X 32
#define FACE_Y 8
#define ITEM_X 188
#define ITEM_Y 21

// ITEMS TILE INDEX
#define ITEM_INDEX_MAGNET 0x09
#define ITEM_INDEX_BOMB 0x0a
#define ITEM_INDEX_HOOK 0x0b
#define ITEM_INDEX_BIG 0x17
#define ITEM_INDEX_VOLT 0x18
#define ITEM_INDEX_HEAL 0x19
#define ITEM_INDEX_SEED 0x07

// ITEMS
#define BOMB_DELAY 0x20

// LEVELS
#define LVL_ALTAR 0
#define LVL_CEMETERY 1
#define LVL_TEMPLE1 2
#define LVL_TEMPLE2 3
#define LVL_TEMPLE3 4
#define LVL_TEMPLE4 5
#define LVL_TOWN 6
#define LVL_FARM 7

#define LVL_TEST 240

#define LVL_INTRO 250

// index = actor index
// array = animation index
void set_animation_info();

// param1: actor index
signed char get_x_speed();

// param1: actor index
signed char get_y_speed();

void set_torch(unsigned char p_index, unsigned char p_x, unsigned char p_y);

void do_skull_tile_collision();

void update_health();

void update_xp();

void first_hit_long();

void first_hit_small();

unsigned char get_inactive_actor_index();

#endif // _SKULLQUEST_H