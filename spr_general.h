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

const unsigned char tree[] = {
    -10, -33, 0x0c, 2,
    -10, -25, 0x1c, 2,
    -18, -25, 0x1b, 2,
    -26, -25, 0x1a, 2,
    14, -25, 0x1f, 2,
    -10, -17, 0x2c, 2,
    -2, -17, 0x2d, 2,
    6, -17, 0x2e, 2,
    14, -17, 0x2f, 2,
    -10, -9, 0x3c, 2,
    -2, -9, 0x3d, 2,
    6, -9, 0x3e, 2,
    -10, -1, 0x4c, 2,
    -2, -1, 0x4d, 2,
    6, -1, 0x4e, 2,
    14, -1, 0x4f, 2,
    -10, 7, 0x5c, 2,
    -2, 7, 0x5d, 2,
    6, 7, 0x3c, 2 | OAM_FLIP_H,
    -2, -25, 0x1d, 2,
    -18, 15, 0x6b, 2,
    -10, 15, 0x6c, 2,
    -2, 15, 0x6d, 2,
    6, 15, 0x6e, 2,
    128

};

const unsigned char* const tree_animation[] = {
    tree

};

const unsigned char gate0[] = {
    0, 0, 0x30, 1 | OAM_FLIP_H,
    0, 8, 0x40, 1 | OAM_FLIP_H,
    0, 16, 0x50, 1 | OAM_FLIP_H,
    8, 0, 0x30, 1,
    8, 8, 0x40, 1,
    8, 16, 0x50, 1,
    128
    
};

const unsigned char gate1[] = {
    0, 0, 0x30, 1 | OAM_FLIP_H,
    0, 8, 0x40, 1 | OAM_FLIP_H,
    0, 16, 0x50, 1 | OAM_FLIP_H,
    8, 0, 0x30, 1,
    8, 8, 0x40, 1,
    8, 16, 0x50, 1,
    8, 24, 0x46, 1,
    0, 24, 0x46, 1 | OAM_FLIP_H,
    128};

const unsigned char gate2[] = {
    0, 0, 0x31, 1 | OAM_FLIP_H,
    0, 8, 0x41, 1 | OAM_FLIP_H,
    0, 16, 0x51, 1 | OAM_FLIP_H,
    0, 24, 0x61, 1 | OAM_FLIP_H,
    8, 0, 0x31, 1,
    8, 8, 0x41, 1,
    8, 16, 0x51, 1,
    8, 24, 0x61, 1,
    128};

const unsigned char gate3[] = {
    0, 0, 0x32, 1 | OAM_FLIP_H,
    0, 8, 0x42, 1 | OAM_FLIP_H,
    0, 16, 0x52, 1 | OAM_FLIP_H,
    0, 24, 0x62, 1 | OAM_FLIP_H,
    8, 0, 0x32, 1,
    8, 8, 0x42, 1,
    8, 16, 0x52, 1,
    8, 24, 0x62, 1,
    128};

const unsigned char gate4[] = {
    0, 0, 0x33, 1 | OAM_FLIP_H,
    0, 8, 0x43, 1 | OAM_FLIP_H,
    0, 16, 0x43, 1 | OAM_FLIP_H,
    0, 24, 0x53, 1 | OAM_FLIP_H,
    8, 0, 0x33, 1,
    8, 8, 0x43, 1,
    8, 16, 0x43, 1,
    8, 24, 0x53, 1,
    128};

const unsigned char torch_flicker1[] = {
    0, 0, 0x12, 3,
    0, 8, 0x14, 1,
    128};

const unsigned char torch_flicker2[] = {
    0, 0, 0x13, 3,
    0, 8, 0x14, 1,
    128};

const unsigned char torch_flicker3[] = {
    0, 0, 0x12, 3 | OAM_FLIP_H,
    0, 8, 0x14, 1,
    128};

const unsigned char torch_flicker4[] = {
    0, 0, 0x16, 3,
    0, 8, 0x14, 1,
    128};

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char torch_animation_index[] = {
    0, 4  // IDLE
};

const unsigned char* const torch_animation[] = {
    torch_flicker1,
    torch_flicker2,
    torch_flicker3,
    torch_flicker4};

const unsigned char gate_animation_index[] = {
    0, 1,  // IDLE
    1, 3,  // OPENING
    4, 1   // OPEN
};

const unsigned char* const gate_animation[] = {
    gate0,

    gate1,
    gate2,
    gate3,

    gate4

};

const unsigned char skull_pile[] = {
    0, 0, 0x39, 3,
    8, 0, 0x3a, 3,
    16, 0, 0x3b, 3,
    8, 8, 0x4a, 3,
    16, 8, 0x4b, 3,
    128};

const unsigned char* const skull_pile_animation[] = {
    skull_pile};

const unsigned char crate_data[] = {

    0, 0, 0x0d, 1,
    8, 0, 0x0e, 1,
    8, 8, 0x0f, 1,
    0, 8, 0x1e, 1,
    128};

const unsigned char* const crate_animation[] = {
    crate_data};
