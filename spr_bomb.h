const unsigned char bomb_0a_data[] = {

    -4, -4, 0x20, 0,
    128

};

const unsigned char bomb_0b_data[] = {

    -4, -4, 0x20, 1,
    128

};
const unsigned char bomb_0c_data[] = {

    -4, -4, 0x20, 2,
    128

};

const unsigned char bomb_1_data[] = {

    -8, -8, 0xd4, 0,
    0, -8, 0xd4, 0 | OAM_FLIP_H,
    0, 0, 0xd4, 0 | OAM_FLIP_H | OAM_FLIP_V,
    -8, 0, 0xd4, 0 | OAM_FLIP_V,
    128

};

const unsigned char bomb_2_data[] = {

    -12, -12, 0xd4, 0,
    -4, 6, 0xf4, 0 | OAM_FLIP_V,
    4, -13, 0xd4, 0 | OAM_FLIP_H,
    -14, -4, 0xe4, 0,
    6, -4, 0xe4, 0 | OAM_FLIP_H,
    -4, -14, 0xf4, 0,
    -12, 4, 0xd4, 0 | OAM_FLIP_V,
    4, 4, 0xd4, 0 | OAM_FLIP_H | OAM_FLIP_V,
    -4, -4, 0xe5, 0,
    128

};

const unsigned char bomb_3_data[] = {

    -16, -12, 0xd4, 0,
    0, 10, 0xf4, 0 | OAM_FLIP_V,
    8, -12, 0xd4, 0 | OAM_FLIP_H,
    -18, -3, 0xe4, 0,
    10, -3, 0xe4, 0 | OAM_FLIP_H,
    -8, -16, 0xf4, 0,
    -16, 6, 0xd4, 0 | OAM_FLIP_V,
    8, 6, 0xd4, 0 | OAM_FLIP_H | OAM_FLIP_V,
    0, -16, 0xd5, 0,
    -8, 10, 0xd5, 0 | OAM_FLIP_H | OAM_FLIP_V,
    128

};

const unsigned char bomb_4_data[] = {

    -16, -13, 0xe5, 0,
    9, -13, 0xe5, 0 | OAM_FLIP_H,
    -21, -3, 0xe5, 0,
    13, -3, 0xe5, 0 | OAM_FLIP_H,
    -4, -19, 0xe5, 0,
    -17, 7, 0xe5, 0 | OAM_FLIP_V,
    7, 7, 0xe5, 0 | OAM_FLIP_H | OAM_FLIP_V,
    -4, 12, 0xe5, 0 | OAM_FLIP_H | OAM_FLIP_V,
    128

};

const unsigned char bomb_5_data[] = {

    -17, -14, 0xf5, 0,
    10, -14, 0xf5, 0 | OAM_FLIP_H,
    -22, -3, 0xf5, 0,
    14, -3, 0xf5, 0 | OAM_FLIP_H,
    -4, -20, 0xf5, 0,
    -18, 8, 0xf5, 0 | OAM_FLIP_V,
    8, 8, 0xf5, 0 | OAM_FLIP_H | OAM_FLIP_V,
    -4, 13, 0xf5, 0 | OAM_FLIP_H | OAM_FLIP_V,
    128

};

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char bomb_animation_index[] = {
    0, 3,  // IDLE
    3, 5,  // EXPLODING
    8, 1   // NOTHING

};

const unsigned char* const bomb_animation[] = {

    bomb_0a_data,
    bomb_0b_data,
    bomb_0c_data,

    bomb_1_data,
    bomb_2_data,
    bomb_3_data,
    bomb_4_data,
    bomb_5_data,

    no_sprite

};
