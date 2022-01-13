

// Levels - generated with NES Screen Tool, with RLE option on
#include "Nametable/Dungeon/level01.h"

// collision data, hand typed
#include "Collision/dungeon.h"

#define PADDLE_MIN 0x10
#define PADDLE_MAX 0xd0
#define PADDLE_Y 0xd0
#define MAX_UP 0x30
#define MAX_DOWN 0xe0
#define BALL_MIN 0x10
#define BALL_MAX 0xea

#pragma bss-name(push, "ZEROPAGE")

// GLOBAL VARIABLES
unsigned char pad1;
unsigned char pad1_new;
unsigned char collision;
unsigned char temp1;
unsigned char temp_x;
unsigned char temp_y;

int address;
unsigned char lives01 = 3;
unsigned char score10;
unsigned char score01;
unsigned char ball_state;
enum{
	BALL_OFF, BALL_STUCK, BALL_ACTIVE
};

unsigned char ball_x_rel; // if stuck
unsigned char ball_count; // if off
unsigned char ball_direction;
enum{
	GOING_UP, GOING_DOWN
};




#pragma bss-name(push, "BSS")

unsigned char c_map[256];
// collision map 
// 16 wide has easier math than 14 wide, so sides are padded with 0



struct SpObject {
	unsigned char X;
	unsigned char Y;
	unsigned char width;
	unsigned char height;
};

struct SpObject Paddle = {0x75,PADDLE_Y,0x1f,7};
struct SpObject Ball = {0xff,0xff,5,5}; // balls x and x will be init later

const unsigned char palette_bg[16]={ 0x0f,0x15,0x3d,0x03,0x0f,0x37,0x17,0x03,0x0f,0x22,0x12,0x03,0x0f,0x00,0x10,0x03 };

const unsigned char palette_sp[16]={ 0x0f, 0x00, 0x10, 0x30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; 

// PROTOTYPES
void draw_bg(void);
void draw_sprites(void);
void movement(void);	
void score_lives_draw(void);
void adjust_score(void);
void hit_block(void);
