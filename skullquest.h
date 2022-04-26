
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
#define TILE_BACK 0x10
#define TILE_BACK_GRASS 0x23
#define TILE_HEART_FULL 0x5B
#define TILE_HEART_EMPTY 0x5C
#define COL_TYPE_SOFT 0x06
#define COL_OFFSET 3

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
#define PADDLE 0
#define CROW 8
#define NECROMANCER 5
#define ANGELICA 5
#define LIGHTNING 6

// Actor STATES!
// Always putting temporary before the next (ex DYING -> DEAD)
#define IDLE 0
#define FLYING 0
#define TURNING 1
#define SKWAK 1
#define WALKING 2
#define RISING 5

#define ROTATE_H 2

#define DYING 3
#define DEAD 4

#define ROTATE_V 4

// Ennemy types
#define TYPE_CROW 0
#define TYPE_SKELETON 1


void animate_skeleton();

// index = actor index
// array = animation index
void set_animation_info(const unsigned char p1, const unsigned char array[][2]);

// param1: actor index
signed char get_x_speed();

// param1: actor index
signed char get_y_speed();