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

const unsigned char pumpkin_1_data[]={

	  0,- 3,0x39,2,
	- 2,- 1,0x8a,2|OAM_FLIP_H,
	  4,- 1,0x8a,2,
	128

};

const unsigned char pumpkin_2_data[]={

	  5,- 1,0x8a,2,
	- 3,- 1,0x8a,2|OAM_FLIP_H,
	  1,- 1,0xf4,2,
	128

};

const unsigned char pumpkin_3_data[]={

	- 6,- 2,0xc7,2,
	  6,- 2,0xc7,2|OAM_FLIP_H,
	  2,- 1,0x8a,2,
	  2,- 2,0xd5,2,
	- 1,- 1,0x48,2,
	128

};

const unsigned char pumpkin_4_data[]={

	- 8,- 2,0xc7,2,
	 10,- 2,0xc6,2,
	- 1,- 2,0xc3,2,
	  3,- 2,0xc4,2,
	  4,- 8,0xc2,2,
	- 2,- 8,0xc1,2,
	128

};

const unsigned char pumpkin_5_data[]={

	-12,- 2,0xc5,2,
	 12,- 2,0xc6,2,
	- 4,- 2,0xc3,2,
	  4,- 2,0xc4,2,
	- 4,-10,0xc1,2,
	  4,-10,0xc2,2,
	128

};

const unsigned char pumpkin_6_data[]={

	-11,- 2,0xc5,2,
	 11,- 2,0xc6,2,
	- 4,- 2,0xc3,2,
	  4,- 2,0xc4,2,
	- 4,- 9,0xc1,2,
	  4,- 9,0xc2,2,
	128

};

const unsigned char pumpkin_7_data[]={

	-12,- 2,0xc5,2,
	 12,- 2,0xc6,2,
	- 4,- 2,0xc3,2,
	  4,- 2,0xc4,2,
	- 3,-13,0xc1,2,
	  4,-13,0xc2,2,
	- 4,- 6,0xe5,0,
	  3,- 6,0xe5,0|OAM_FLIP_H,
	128

};

const unsigned char pumpkin_8_data[]={

	-12,- 2,0xc5,2,
	 12,- 2,0xc6,2,
	- 4,- 1,0xc3,2,
	  4,- 1,0xc4,2,
	- 3,-18,0xc1,2,
	  5,-18,0xc2,2,
	- 4,- 5,0xe5,0,
	  3,- 5,0xe5,0|OAM_FLIP_H,
	  1,-15,0xe5,0,
	  2,-11,0x7c,0,
	128

};

const unsigned char pumpkin_9_data[]={

	-12,- 2,0xc5,2,
	 12,- 2,0xc6,2,
	- 4,- 1,0xc3,2,
	  4,- 1,0xc4,2,
	- 3,-21,0xc1,2,
	  5,-21,0xc2,2,
	- 4,- 5,0xe5,0,
	  3,- 5,0xe5,0|OAM_FLIP_H,
	  1,-15,0xe5,0,
	  1,-12,0x7c,0,
	128

};

const unsigned char* scarecrow_animation[] = {
    scarecrow_data};

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char pumpkin_animation_index[] = {
    0, 1,   // IDLE
    1, 5,   // APPEARING
    6, 1,   // IDLE2
    6, 1,   // IDLE2
    6, 1,   // IDLE2
    7, 4,   // EATING
    11, 2,  // CHEWING
	6, 1,
	6, 1
};

const unsigned char* pumpkin_animation[] = {
    // 0 -- 0-IDLE
    pumpkin_1_data,
    // 1 -- 1-APPEARING
    pumpkin_2_data,
    pumpkin_3_data,
    pumpkin_4_data,
    pumpkin_5_data,
    pumpkin_6_data,
    // 6 -- 2-IDLE2
    pumpkin_5_data,
    // 7 -- 3-EATING
    pumpkin_6_data,
    pumpkin_7_data,
    pumpkin_8_data,
    pumpkin_9_data,
    // 11 -- 4-CHEWING
    pumpkin_5_data,
    pumpkin_6_data,

};