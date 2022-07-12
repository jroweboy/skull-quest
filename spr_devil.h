const unsigned char devil_8_data[] = {

    0, -1, 0x84, 2,
    5, 7, 0x88, 2,
    0, 6, 0x85, 2,
    0, 12, 0x86, 2,
    -7, 10, 0x87, 2,
    128

};

const unsigned char devil_9_data[] = {

    0, -2, 0x84, 2,
    5, 7, 0x88, 2,
    0, 6, 0x85, 2,
    0, 12, 0x86, 2,
    -7, 10, 0x87, 2,
    128

};

const unsigned char devil_10_data[] = {

    0, -3, 0x84, 2,
    5, 6, 0x88, 2,
    0, 12, 0x86, 2,
    -7, 9, 0x87, 2,
    0, 5, 0x85, 2,
    128

};

const unsigned char devil_attack[] = {

    0, -3, 0x84, 2,
    8, 6, 0x88, 2,
    1, 5, 0x89, 2,
    0, 12, 0x86, 2,
    -7, 9, 0x87, 2,
    0, 5, 0x85, 2,
    128

};

const unsigned char devil_face[]={
	  0,  0,0xd6,0,
	  8,  0,0xd7,0,
	  8,  8,0xe7,0,
	  0,  8,0xe6,0,
	  0, 16,0xf6,0,
	  8, 16,0xf7,0,
	128
};


// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char devil_animation_index[] = {
    0, 5,  // IDLE
    5, 1,  // ATTACKING
    0, 5   // IDLE

};

const unsigned char* const devil_animation[] = {

    devil_8_data,
    devil_9_data,
    devil_10_data,
    devil_9_data,
    devil_8_data,

    devil_attack

};