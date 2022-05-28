const unsigned char skull_rotate_left1[] = {
    0, 0, 0x00, 0b00000000,
    128

};

const unsigned char skull_rotate_left2[] = {
    0, 0, 0x02, OAM_FLIP_H,
    128

};

const unsigned char skull_rotate_left3[] = {
    0, 0, 0x03, 0b10000000,
    128

};

const unsigned char skull_rotate_left4[] = {
    0, 0, 0x02, 0b00000000,
    128

};
const unsigned char skull_rotate_left5[] = {
    0, 0, 0x00, 0b01000000,
    128

};
const unsigned char skull_rotate_left6[] = {
    0, 0, 0x01, 0b00000000,
    128

};

const unsigned char skull_rotate_right1[] = {
    0, 0, 0x00, 0b00000000,
    128

};

const unsigned char skull_rotate_right2[] = {
    0, 0, 0x01, 0b00000000,
    128

};

const unsigned char skull_rotate_right3[] = {
    0, 0, 0x00, 0b01000000,
    128

};

const unsigned char skull_rotate_right4[] = {
    0, 0, 0x02, 0b00000000,
    128

};
const unsigned char skull_rotate_right5[] = {
    0, 0, 0x03, 0b10000000,
    128

};
const unsigned char skull_rotate_right6[] = {
    0, 0, 0x02, 0b01000000,
    128

};

// const unsigned char skull_rotate_up1[] = {
//     0, 0, 0x01, 0b00000000,
//     128};

// const unsigned char skull_rotate_up2[] = {
//     0, 0, 0x03, 0b00000000,
//     128

// };

// const unsigned char skull_rotate_up3[] = {
//     0, 0, 0x04, 0b00000000,
//     128

// };

// const unsigned char skull_rotate_down1[] = {
//     0, 0, 0x01, 0b00000000,
//     128

// };

// const unsigned char skull_rotate_down2[] = {
//     0, 0, 0x04, 0b00000000,
//     128};

// const unsigned char skull_rotate_down3[] = {
//     0, 0, 0x03, 0b00000000,
//     128

// };

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char skull_animation_index[] = {
    0, 1,  // IDLE
    0, 0,  // *TURNING NOT USED
    2, 6  // ROTATE_H
};

const unsigned char* const skull_animation[] = {
    skull_rotate_left1,
    skull_rotate_right1,

    skull_rotate_left1,
    skull_rotate_left2,
    skull_rotate_left3,
    skull_rotate_left4,
    skull_rotate_left5,
    skull_rotate_left6,
    skull_rotate_right1,
    skull_rotate_right2,
    skull_rotate_right3,
    skull_rotate_right4,
    skull_rotate_right5,
    skull_rotate_right6,

    // skull_rotate_up1,
    // skull_rotate_up2,
    // skull_rotate_up3,
    // skull_rotate_down1,
    // skull_rotate_down2,
    // skull_rotate_down3,
};