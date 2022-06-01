const unsigned char fireball1[] = {
    0, 0, 0xad, 0,
    128};

const unsigned char fireball2[] = {
    0, 0, 0xac, 0,
    128};

const unsigned char fireball3[] = {
    0, 0, 0xbc, 0,
    128};

const unsigned char fireball1_right[] = {
    0, 0, 0xad, 0 | OAM_FLIP_H,
    128};

const unsigned char fireball2_right[] = {
    0, 0, 0xac, 0 | OAM_FLIP_H,
    128};

const unsigned char fireball3_right[] = {
    0, 0, 0xbc, 0 | OAM_FLIP_H,
    128};

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char fireball_animation_index[] = {
    0, 3,    // IDLE
};

const unsigned char* const fireball_animation[] = {
    fireball1,
    fireball2,
    fireball3,

    fireball1_right,
    fireball2_right,
    fireball3_right

};