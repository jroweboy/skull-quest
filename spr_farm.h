const unsigned char scarecrow_data[] = {

    9, -3, 0x8e, 2,
    17, -3, 0x8f, 2,
    4, 5, 0x9e, 2,
    12, 5, 0x9f, 2,
    4, 13, 0xae, 2,
    12, 13, 0xaf, 2,
    20, 10, 0xbe, 2,
    11, 21, 0xbf, 2,
    128

};

const unsigned char pumpkin_1_data[] = {

    0, -3, 0x39, 3,
    -2, -1, 0x8a, 3 | OAM_FLIP_H,
    4, -1, 0x8a, 3,
    128

};

const unsigned char pumpkin_2_data[] = {

    5, -1, 0x8a, 3,
    -3, -1, 0x8a, 3 | OAM_FLIP_H,
    1, -1, 0xf4, 3,
    128

};

const unsigned char pumpkin_3_data[] = {

    -6, -2, 0xc7, 3,
    6, -2, 0xc7, 3 | OAM_FLIP_H,
    2, -1, 0x8a, 3,
    2, -2, 0xd5, 3,
    -1, -1, 0x48, 3,
    128

};

const unsigned char pumpkin_4_data[] = {

    -8, -2, 0xc7, 3,
    10, -2, 0xc6, 3,
    -1, -2, 0xc3, 3,
    3, -2, 0xc4, 3,
    4, -8, 0xc2, 3,
    -2, -8, 0xc1, 3,
    128

};

const unsigned char pumpkin_5_data[] = {

    -12, -2, 0xc5, 3,
    12, -2, 0xc6, 3,
    -4, -2, 0xc3, 3,
    4, -2, 0xc4, 3,
    -4, -10, 0xc1, 3,
    4, -10, 0xc2, 3,
    128

};

const unsigned char pumpkin_6_data[] = {

    -11, -2, 0xc5, 3,
    11, -2, 0xc6, 3,
    -4, -2, 0xc3, 3,
    4, -2, 0xc4, 3,
    -4, -9, 0xc1, 3,
    4, -9, 0xc2, 3,
    128

};

const unsigned char pumpkin_7_data[] = {

    -12, -2, 0xc5, 3,
    12, -2, 0xc6, 3,
    -4, -2, 0xc3, 3,
    4, -2, 0xc4, 3,
    -3, -13, 0xc1, 3,
    4, -13, 0xc2, 3,
    -4, -7, 0xe5, 3,
    3, -7, 0xe5, 3 | OAM_FLIP_H,
    128

};

const unsigned char pumpkin_8_data[] = {

    -12, -2, 0xc5, 3,
    12, -2, 0xc6, 3,
    -4, -1, 0xc3, 3,
    4, -1, 0xc4, 3,
    -1, -19, 0xc1, 3,
    5, -19, 0xc2, 3,
    -4, -6, 0xe5, 3,
    3, -6, 0xe5, 3 | OAM_FLIP_H,
    1, -15, 0xe5, 3,
    128

};

const unsigned char pumpkin_9_data[] = {

    -12, -2, 0xc5, 3,
    12, -2, 0xc6, 3,
    -4, -1, 0xc3, 3,
    4, -1, 0xc4, 3,
    -1, -21, 0xc1, 3,
    5, -21, 0xc2, 3,
    -4, -5, 0xe5, 3,
    3, -5, 0xe5, 3 | OAM_FLIP_H,
    1, -17, 0xe5, 3,
    1, -12, 0x7c, 3,
    128

};

const unsigned char* scarecrow_animation[] = {
    scarecrow_data};

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char pumpkin_animation_index[] = {
    0, 1,   // SEED
    1, 5,   // GROWING
    6, 1,   // IDLE
    7, 4,   // EATING
    11, 10  // CHEWING
            // SPIT_OUT
            // IDLE
};

const unsigned char* pumpkin_animation[] = {
    // 0 -- 0-SEED
    pumpkin_1_data,
    // 1 -- 1-GROWING
    pumpkin_2_data,
    pumpkin_3_data,
    pumpkin_4_data,
    pumpkin_5_data,
    pumpkin_6_data,
    // 6 -- 2-IDLE
    pumpkin_5_data,
    // 7 -- 3-EATING
    pumpkin_6_data,
    pumpkin_7_data,
    pumpkin_8_data,
    pumpkin_9_data,
    // 11 -- 4-CHEWING
    pumpkin_5_data,
    pumpkin_6_data,
    pumpkin_5_data,
    pumpkin_6_data,
    pumpkin_5_data,
    pumpkin_6_data,
    pumpkin_5_data,
    pumpkin_6_data,
    pumpkin_5_data,
    pumpkin_6_data,
    // 21 -- 5-SPIT OUT
    pumpkin_9_data
    // 22 -- 6-IDLE2

};