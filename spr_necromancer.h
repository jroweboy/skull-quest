const unsigned char necromancer_idle[] = {

    0, 0, 0x70, 1,
    8, 0, 0x71, 1,
    0, 8, 0x72, 1,
    8, 8, 0x73, 1,
    0, 16, 0x74, 1,
    8, 16, 0x75, 1,
    15, -6, 0x65, 1,
    3, -4, 0x60, 2,
    9, 7, 0x64, 1,
    13, 4, 0x56, 2 | OAM_FLIP_V,
    8, 16, 0x77, 1,
    10, 8, 0x77, 1,
    17, 2, 0x77, 1 | OAM_FLIP_H | OAM_FLIP_V,
    128

};

const unsigned char necromancer_raising1[] = {

    0, 0, 0x70, 1,
    8, 0, 0x71, 1,
    0, 8, 0x72, 1,
    8, 8, 0x73, 1,
    0, 16, 0x74, 1,
    8, 16, 0x75, 1,
    14, -8, 0x66, 1,
    3, -4, 0x60, 2,
    9, 6, 0x64, 1,
    13, 2, 0x56, 2 | OAM_FLIP_V,
    10, 5, 0x77, 1,
    9, 12, 0x77, 1,
    10, 0, 0x77, 1,
    1, 8, 0x64, 1 | OAM_FLIP_H,
    -3, 5, 0x56, 2,
    128

};

const unsigned char necromancer_raising2[] = {

    0, 0, 0x70, 1,
    8, 0, 0x71, 1,
    0, 8, 0x72, 1,
    8, 8, 0x73, 1,
    0, 16, 0x74, 1,
    8, 16, 0x75, 1,
    14, -11, 0x66, 1,
    3, -4, 0x60, 2,
    9, 1, 0x64, 1 | OAM_FLIP_V,
    14, -2, 0x56, 2 | OAM_FLIP_V,
    10, 5, 0x77, 1,
    9, 10, 0x77, 1,
    10, -3, 0x76, 1,
    -1, 7, 0x64, 1 | OAM_FLIP_H,
    -6, 4, 0x56, 2,
    128

};

const unsigned char necromancer_hit[] = {

    0, 0, 0x70, 1,
    8, 0, 0x71, 1,
    0, 8, 0x72, 1,
    8, 8, 0x73, 1,
    0, 16, 0x74, 1,
    8, 16, 0x75, 1,
    14, -6, 0x66, 1,
    3, -4, 0x60, 2,
    9, 7, 0x64, 1,
    13, 4, 0x56, 2 | OAM_FLIP_V,
    15, 16, 0x77, 1 | OAM_FLIP_H | OAM_FLIP_V,
    10, 8, 0x77, 1,
    17, 2, 0x76, 1 | OAM_FLIP_H | OAM_FLIP_V,
    -1, 7, 0x64, 1 | OAM_FLIP_H,
    -6, 4, 0x56, 2,
    128

};

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char necromancer_animation_index[] = {
    0, 1,  // IDLE
    1, 2,  // RAISING
    2, 1,  // IDLE2
    3, 1,  // IDLE3 - because this is a transitioning state
    3, 1   // IDLE3
};

const unsigned char* const necromancer_animation[] = {

    necromancer_idle,
    necromancer_raising1,
    necromancer_raising2,
    necromancer_hit};
