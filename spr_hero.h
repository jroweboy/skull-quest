const unsigned char hero_head_up[]={

	  5,  3,0x35,2,
	  0,  0,0x34,2,
	  0,  8,0x44,2,
	  0, 16,0x54,2,
	  8,  8,0x45,2,
	  8, 16,0x55,2,
	  9,  0,0x34,2|OAM_FLIP_H,
	128

};

const unsigned char hero_head_down[]={

	  5,  4,0x36,2,
	  0,  0,0x34,2,
	  0,  8,0x44,2,
	  0, 16,0x54,2,
	  8,  8,0x45,2,
	  8, 16,0x55,2,
	  9,  0,0x34,2|OAM_FLIP_H,
	128

};

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char hero_animation_index[] = {
    0, 1,   // IDLE
    1, 1,   // DYING
    1, 1,   // DEAD
};

const unsigned char* const hero_animation[] = {
	hero_head_up,
	hero_head_down,
	hero_head_down
};

// const unsigned char skull_face[]={
// 	  8,  8,0xc4,0,
// 	 16,  8,0xc5,0,
// 	  8, 16,0xd4,0,
// 	 16, 16,0xd5,0,
// 	128
// };

