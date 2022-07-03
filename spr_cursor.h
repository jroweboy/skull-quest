const unsigned char cursor_0_data[]={

	  5,  4,0x08,0,
	 20,  4,0x08,0|OAM_FLIP_H,
	  5, 20,0x08,0|OAM_FLIP_V,
	 20, 20,0x08,0|OAM_FLIP_H|OAM_FLIP_V,
	128

};

const unsigned char cursor_1_data[]={

	  6,  5,0x08,0,
	 19,  5,0x08,0|OAM_FLIP_H,
	  6, 19,0x08,0|OAM_FLIP_V,
	 19, 19,0x08,0|OAM_FLIP_H|OAM_FLIP_V,
	128

};

const unsigned char cursor_2_data[]={

	  3,  3,0x08,0,
	 53,  3,0x08,0|OAM_FLIP_H,
	  3, 29,0x08,0|OAM_FLIP_V,
	 53, 29,0x08,0|OAM_FLIP_H|OAM_FLIP_V,
	128

};

const unsigned char cursor_3_data[]={

	  4,  4,0x08,0,
	 52,  4,0x08,0|OAM_FLIP_H,
	  4, 28,0x08,0|OAM_FLIP_V,
	 52, 28,0x08,0|OAM_FLIP_H|OAM_FLIP_V,
	128

};

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char cursor_animation_index[] = {
    0, 2,  // CURSOR_SMALL
    0, 2,  //
    2, 2   // CURSOR_LARGE
};

const unsigned char* const cursor_animation[] = {
    cursor_0_data,
    cursor_1_data,
    cursor_2_data,
    cursor_3_data

};
