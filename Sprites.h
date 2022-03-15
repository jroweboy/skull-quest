//  0 - 3 are decorative sprite elements
//  4 the Skull
// 	5 -29 ennemies...
#define ANIMATION_COUNT 30

// typedef struct {
//     unsigned char speed[ANIMATION_COUNT];
//     unsigned char frames[ANIMATION_COUNT];

// } Animations;

// a 16x16 pixel metasprite
const unsigned char HorizontalPaddleSpr[] = {
    0, 0, 0x05, 0,
    8, 0, 0x05, 0,
    16, 0, 0x05, 0,
    24, 0, 0x05, 0,
    128};

const unsigned char VerticalPaddleSpr[] = {
    0, 0, 0x06, 0,
    0, 8, 0x06, 0,
    0, 16, 0x06, 0,
    0, 24, 0x06, 0,
    128};

const unsigned char SkullAnim[] = {
    0x00, 0b00000000,
    0x01, 0b00000000,
    0x00, 0b01000000,
    0x02, 0b00000000,
    0x03, 0b10000000,
    0x02, 0b01000000};

// const unsigned char animations
