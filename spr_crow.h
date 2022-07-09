const unsigned char crow_left_idle[] = {
    0, 0, 0x22, 1 | OAM_FLIP_H,
    0, 5, 0x25, 1,
    8, 5, 0x26, 1,
    128};

const unsigned char crow_right_idle[] = {
    8, 1, 0x22, 1,
    8, 6, 0x25, 1 | OAM_FLIP_H,
    0, 6, 0x26, 1 | OAM_FLIP_H,
    128};

const unsigned char crow_left_skwak[] = {
    -2, 1, 0x21, 1,
    0, 5, 0x23, 1,
    8, 5, 0x24, 1,
    128};

const unsigned char crow_right_skwak[] = {
    10, 2, 0x21, 1 | OAM_FLIP_H,
    8, 6, 0x23, 1 | OAM_FLIP_H,
    0, 6, 0x24, 1 | OAM_FLIP_H,
    128};

const unsigned char crow_left_fly1[] = {
    0, 6, 0x27, 1,
    7, 6, 0x28, 1,
    6, -1, 0x29, 1,
    -3, 2, 0x22, 1 | OAM_FLIP_H,
    0, 0, 0x29, 1 | OAM_FLIP_H,
    128

};

const unsigned char crow_right_fly1[] = {
    2, -1, 0x29, 1 | OAM_FLIP_H,
    8, 6, 0x27, 1 | OAM_FLIP_H,
    1, 6, 0x28, 1 | OAM_FLIP_H,
    11, 2, 0x22, 1,
    8, 0, 0x29, 1,
    128

};

const unsigned char crow_left_fly2[] = {
    0, 6, 0x27, 1,
    8, 6, 0x28, 1,
    -3, 1, 0x22, 1 | OAM_FLIP_H,
    6, 7, 0x29, 1 | OAM_FLIP_V,
    2, 5, 0x29, 1 | OAM_FLIP_H | OAM_FLIP_V,
    128

};

const unsigned char crow_right_fly2[] = {
    8, 6, 0x27, 1 | OAM_FLIP_H,
    0, 6, 0x28, 1 | OAM_FLIP_H,
    11, 1, 0x22, 1,
    2, 7, 0x29, 1 | OAM_FLIP_H | OAM_FLIP_V,
    6, 5, 0x29, 1 | OAM_FLIP_V,
    128

};

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char crow_animation_index[] = {
    0, 2,  // FLYING
    4, 1,  // SKWAK
    6, 1,  // IDLE2
	0, 2,  // --
    0, 2,  // CHASING
	0, 2,  // --
	0, 2,  // KIDNAPPING
	0, 2,  // --
	0, 2   // RETURNING
};

const unsigned char* const crow_animation[] = {
    crow_left_fly1,
    crow_left_fly2,
	crow_right_fly1,
	crow_right_fly2,

    crow_left_skwak,
	crow_right_skwak,

    crow_left_idle,
	crow_right_idle,
};