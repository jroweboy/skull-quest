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
    0, -16, 0x30, 1,
    0, -8, 0x40, 1,
    0, 0, 0x50, 1,
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
