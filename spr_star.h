const unsigned char star_0_data[] = {

    -3, -23, 0x12, 0,
    17, -4, 0x12, 0,
    -24, -4, 0x12, 0,
    -16, 19, 0x12, 0,
    9, 19, 0x12, 0,
    128

};

const unsigned char star_1_data[] = {

    6, -19, 0x12, 0,
    15, 4, 0x12, 0,
    -18, -10, 0x12, 0,
    -19, 11, 0x12, 0,
    1, 18, 0x12, 0,
    128

};

const unsigned char star_2_data[] = {

    10, -13, 0x12, 0,
    10, 7, 0x12, 0,
    -10, -14, 0x12, 0,
    -18, 0, 0x12, 0,
    -6, 14, 0x12, 0,
    128

};

const unsigned char star_3_data[] = {

    7, -6, 0x12, 0,
    3, 7, 0x12, 0,
    -4, -15, 0x12, 0,
    -15, -8, 0x12, 0,
    -11, 7, 0x12, 0,
    128

};

const unsigned char star_4_data[] = {

    6, 0, 0x12, 0,
    -4, 7, 0x12, 0,
    2, -10, 0x12, 0,
    -8, -11, 0x12, 0,
    -13, -1, 0x12, 0,
    128

};

const unsigned char star_5_data[] = {

    2, 1, 0x12, 0,
    -7, 1, 0x12, 0,
    3, -6, 0x12, 0,
    -3, -11, 0x12, 0,
    -9, -6, 0x12, 0,
    128

};

const unsigned char star_6_data[] = {

    -3, 3, 0x12, 0,
    -9, -2, 0x12, 0,
    3, -2, 0x12, 0,
    1, -8, 0x12, 0,
    -5, -8, 0x12, 0,
    128

};

const unsigned char star_7_data[] = {

    -3, -8, 0x12, 0,
    1, -4, 0x12, 0,
    -7, -5, 0x12, 0,
    -6, 1, 0x12, 0,
    -1, 1, 0x12, 0,
    128

};

const unsigned char star_8_data[] = {

    -3, 0, 0x12, 0,
    0, -5, 0x12, 0,
    -6, -5, 0x12, 0,
    128

};

const unsigned char star_9_data[] = {

    -3, -3, 0x12, 0,
    128

};

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char stars_animation_index[] = {
    0, 1,   // NOTHING
    1, 10,  // ANIMATION
    0, 1    // NOTHING

};

const unsigned char* const stars_animation[] = {

    no_sprite,
    star_0_data,
    star_1_data,
    star_2_data,
    star_3_data,
    star_4_data,
    star_5_data,
    star_6_data,
    star_7_data,
    star_8_data,
    star_9_data

};
