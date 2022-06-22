
#define ONES 7
#define TENS 6
#define HUNDREDS 5
#define THOUSANDS 4
#define TEN_THOUSANDS 3
#define HUNDRED_THOUSANDS 2
#define MILLIONS 1
#define TEN_MILLIONS 0

#define MAX_HEALTH 8
#define SPEED_STEP 1
#define TILE_BACK 0x01
#define TILE_BACK_GRASS 0x07
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
#define COL_TYPE_2HIT 0x09
#define COL_TYPE_0A 0x0a
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

#define TITLE 0
#define MAIN 1
#define STORY 2
#define MAP 3
#define GAME_OVER 4

// ACTOR INDEX
#define SKULL 4
#define SKULL2 5
#define PADDLE 0


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
#define DISAPPEARING 3
#define RISING 5
#define PARALYZED 5
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
#define TYPE_MAGNET 22
#define TYPE_BOMB 23


// PADDLE
#define TYPE_PAD_HORZ 0
#define TYPE_PAD_VERT 1
#define PAD_SHORT 16
#define PAD_LONG 32
#define PAD_VELOCITY 80
#define FACE_X 16
#define FACE_Y 8
#define ITEM_X 188
#define ITEM_Y 21

// ITEMS TILE INDEX
#define ITEM_MAGNET 0x09
#define ITEM_BOMB 0x20

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