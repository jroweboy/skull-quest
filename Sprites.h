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

const unsigned char crow_left[] = {
    -8, -7, 0x22, 1 | OAM_FLIP_H,
    -8, -2, 0x25, 1,
    0, -2, 0x26, 1,
    128

};

const unsigned char crow_left_skwak[] = {
    -10, -6, 0x21, 1,
    -8, -2, 0x23, 1,
    0, -2, 0x24, 1,
    128

};

const unsigned char crow_right[] = {
    0, -7, 0x22, 1,
    0, -2, 0x25, 1 | OAM_FLIP_H,
    -8, -2, 0x26, 1 | OAM_FLIP_H,
    128

};

const unsigned char crow_right_skwak[] = {
    2, -6, 0x21, 1 | OAM_FLIP_H,
    0, -2, 0x23, 1 | OAM_FLIP_H,
    -8, -2, 0x24, 1 | OAM_FLIP_H,
    128

};

const unsigned char door1[] = {
    -8, -16, 0x30, 1 | OAM_FLIP_H,
    -8, -8, 0x40, 1 | OAM_FLIP_H,
    -8, 0, 0x50, 1 | OAM_FLIP_H,
    -8, 8, 0x60, 1 | OAM_FLIP_H,
    0, -16, 0x30, 1,
    0, -8, 0x40, 1,
    0, 0, 0x50, 1,
    0, 8, 0x60, 1,
    128

};

const unsigned char door2[] = {
    -8, -16, 0x31, 1 | OAM_FLIP_H,
    -8, -8, 0x41, 1 | OAM_FLIP_H,
    -8, 0, 0x51, 1 | OAM_FLIP_H,
    -8, 8, 0x61, 1 | OAM_FLIP_H,
    0, -16, 0x31, 1,
    0, -8, 0x41, 1,
    0, 0, 0x51, 1,
    0, 8, 0x61, 1,
    128

};

const unsigned char door3[] = {
    -8, -16, 0x32, 1 | OAM_FLIP_H,
    -8, -8, 0x42, 1 | OAM_FLIP_H,
    -8, 0, 0x52, 1 | OAM_FLIP_H,
    -8, 8, 0x62, 1 | OAM_FLIP_H,
    0, -16, 0x32, 1,
    0, -8, 0x42, 1,
    0, 0, 0x52, 1,
    0, 8, 0x62, 1,
    128

};

const unsigned char door4[] = {
    -8, -16, 0x33, 1 | OAM_FLIP_H,
    -8, -8, 0x43, 1 | OAM_FLIP_H,
    -8, 0, 0x43, 1 | OAM_FLIP_H,
    -8, 8, 0x53, 1 | OAM_FLIP_H,
    0, -16, 0x33, 1,
    0, -8, 0x43, 1,
    0, 0, 0x43, 1,
    0, 8, 0x53, 1,
    128};

const unsigned char skeleton_left_still[] = {
    -5, -4, 0xdd, 3 | OAM_FLIP_H,
    -4, 8, 0xd8, 3 | OAM_FLIP_H,
    -4, 1, 0xd9, 3 | OAM_FLIP_H,
    -12, 0, 0xdc, 3 | OAM_FLIP_H,
    128

};

const unsigned char skeleton_left_walk1[] = {
    -5, -5, 0xdd, 3 | OAM_FLIP_H,
    -4, 8, 0xda, 3 | OAM_FLIP_H,
    -4, 0, 0xd9, 3 | OAM_FLIP_H,
    -12, -1, 0xdc, 3 | OAM_FLIP_H,
    128

};

const unsigned char skeleton_left_walk2[] = {
    -6, -4, 0xdd, 3 | OAM_FLIP_H,
    -4, 8, 0xdb, 3 | OAM_FLIP_H,
    -5, 1, 0xd9, 3 | OAM_FLIP_H,
    -13, 0, 0xdc, 3 | OAM_FLIP_H,
    128

};

const unsigned char skeleton_left_walk3[] = {
    -5, 8, 0xde, 3 | OAM_FLIP_H,
    -5, 0, 0xd9, 3 | OAM_FLIP_H,
    -13, -2, 0xdc, 3 | OAM_FLIP_H,
    -6, -5, 0xdd, 3 | OAM_FLIP_H,
    128

};

const unsigned char skeleton_left_walk4[] = {
    -5, 8, 0xdf, 3 | OAM_FLIP_H,
    -5, 1, 0xd9, 3 | OAM_FLIP_H,
    -13, -1, 0xdc, 3 | OAM_FLIP_H,
    -6, -4, 0xdd, 3 | OAM_FLIP_H,
    128

};

const unsigned char skeleton_left_lookout[] = {
    -5, -4, 0xdd, 3,
    -4, 8, 0xd8, 3 | OAM_FLIP_H,
    -4, 1, 0xd9, 3 | OAM_FLIP_H,
    -12, 0, 0xdc, 3 | OAM_FLIP_H,
    128

};

const unsigned char* const skeleton_left_walk[] = {
    skeleton_left_walk1,
    skeleton_left_walk2,
    skeleton_left_walk3,
    skeleton_left_walk4,
};

const unsigned char skeleton_right_still[] = {
    -3, -4, 0xdd, 3,
    -4, 8, 0xd8, 3,
    -4, 1, 0xd9, 3,
    4, 0, 0xdc, 3,
    128

};

const unsigned char skeleton_right_walk1[] = {
    -3, -5, 0xdd, 3,
    -4, 8, 0xda, 3,
    -4, 0, 0xd9, 3,
    4, -1, 0xdc, 3,
    128

};

const unsigned char skeleton_right_walk2[] = {
    -2, -4, 0xdd, 3,
    -4, 8, 0xdb, 3,
    -3, 1, 0xd9, 3,
    5, 0, 0xdc, 3,
    128

};

const unsigned char skeleton_right_walk3[] = {
    -3, 8, 0xde, 3,
    -3, 0, 0xd9, 3,
    5, -2, 0xdc, 3,
    -2, -5, 0xdd, 3,
    128

};

const unsigned char skeleton_right_walk4[] = {
    -3, 8, 0xdf, 3,
    -3, 1, 0xd9, 3,
    5, -1, 0xdc, 3,
    -2, -4, 0xdd, 3,
    128

};

const unsigned char skeleton_right_lookout[] = {
    -3, -4, 0xdd, 3 | OAM_FLIP_H,
    -4, 8, 0xd8, 3,
    -4, 1, 0xd9, 3,
    4, 0, 0xdc, 3,
    128

};

const unsigned char* const skeleton_right_list[] = {
    skeleton_right_walk1,
    skeleton_right_walk2,
    skeleton_right_walk3,
    skeleton_right_walk4,

};
