const unsigned char villager_beard_idle1[] = {

    0, 0, 0x94, 3,
    0, 8, 0xa4, 3,
    0, 16, 0xb4, 3,
    128

};

const unsigned char villager_beard_idle2[] = {

    0, 1, 0x94, 3,
    0, 9, 0xa4, 3,
    0, 17, 0xb4, 3,
    128

};

const unsigned char villager_beard_idle3[] = {

    0, 2, 0x94, 3,
    0, 10, 0xa4, 3,
    0, 18, 0xb4, 3,
    128

};

const unsigned char villager_beard_face[] = {

    0, 0, 0x95, 3,
    0, 8, 0xa5, 3,
    0, 16, 0xb5, 3,
    8, 0, 0x96, 3,
    8, 8, 0xa6, 3,
    8, 16, 0xb6, 3,
    128

};

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char villager_animation_index[] = {
    0, 6,  // IDLE

};

const unsigned char* const villager_animation[] = {

    villager_beard_idle1,
    villager_beard_idle2,
    villager_beard_idle3,
    villager_beard_idle3,
    villager_beard_idle2,
    villager_beard_idle1

};