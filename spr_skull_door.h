const unsigned char skull_door[] = {
    5, 0, 0xc0, 3,
    0, 8, 0xd0, 3,
    8, 8, 0xd1, 3,
    0, 16, 0xe0, 3,
    8, 16, 0xe1, 3,
    0, 24, 0xf0, 3,
    8, 24, 0xf1, 3,
    128};

const unsigned char skull_door2[] = {
    0, 8, 0xd2, 3,
    0, 16, 0xe2, 3,
    0, 24, 0xf2, 3,
    8, 8, 0xd2, 3 | OAM_FLIP_H,
    8, 16, 0xe2, 3 | OAM_FLIP_H,
    8, 24, 0xf2, 3 | OAM_FLIP_H,
    128};

const unsigned char skull_door3[] = {
    0, 8, 0xd3, 3,
    0, 16, 0xe3, 3,
    0, 24, 0xf3, 3,
    8, 8, 0xd3, 3 | OAM_FLIP_H,
    8, 16, 0xe3, 3 | OAM_FLIP_H,
    8, 24, 0xf3, 3 | OAM_FLIP_H,
    128};

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char skull_door_animation_index[] = {
    0, 1,   // IDLE
    1, 1,   // TURNING
    2, 1,   // OPENED
};

const unsigned char* const skull_door_animation[] = {
    skull_door,
    skull_door2,
    skull_door3,
};