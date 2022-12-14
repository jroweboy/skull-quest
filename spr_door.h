const unsigned char house_door_idle[] = {

    6, 5, 0x5a, 1,
    6, 13, 0x6a, 1,
    6, 21, 0x7a, 1,
    128

};

const unsigned char house_door_open1[] = {

    6, 5, 0x5b, 1,
    6, 13, 0x7b, 1,
    4, 16, 0x7b, 1 | OAM_FLIP_H | OAM_FLIP_V,
    128

};

const unsigned char house_door_open2[] = {

    0, 6, 0x53, 1,
    0, 12, 0x53, 1,
    6, 16, 0x53, 1 | OAM_FLIP_H | OAM_FLIP_V,
    128

};

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char house_door_animation_index[] = {
    0, 1,  // IDLE
    1, 2,  // OPENING
    3, 1   // OPENED
};

const unsigned char* const house_door_animation[] = {

    house_door_idle,
    house_door_open1,
    house_door_open2,
    no_sprite

};