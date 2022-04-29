const unsigned char angelic_idle1[]={
	  0,  0,0xd3,3,
	  7,  0,0xd3,3|OAM_FLIP_H,
	  0,  8,0xc2,3,
	  8,  8,0xc3,3,
	  0, 16,0xc0,3,
	  8, 16,0xc1,3,
	128
};

const unsigned char angelic_idle2[]={
	  0,  1,0xd3,3,
	  7,  1,0xd3,3|OAM_FLIP_H,
	  0,  9,0xc2,3,
	  8,  9,0xc3,3,
	  0, 17,0xc0,3,
	  8, 17,0xc1,3,
	128
};

const unsigned char angelic_idle3[]={
	  0,  2,0xd3,3,
	  7,  2,0xd3,3|OAM_FLIP_H,
	  0, 10,0xc2,3,
	  8, 10,0xc3,3,
	  0, 18,0xc0,3,
	  8, 18,0xc1,3,
	128
};

const unsigned char angelic_face[]={
	- 2, 12,0xd2,3,
	  6,  0,0xd0,3,
	 14,  0,0xd1,3,
	  6,  8,0xe0,3,
	 14,  8,0xe1,3,
	 22,  0,0xe3,3,
	  6, 16,0xf0,3,
	 14, 18,0xf1,3,
	 14, 16,0xf2,3,
	 22, 15,0xf3,3,
	 22,  8,0xe2,3,
	128
};

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char angelic_animation_index[][2] = {
    { 0, 6 } // IDLE
};

const unsigned char* const angelic_animation[] = {
    angelic_idle1,
    angelic_idle2,
    angelic_idle3,
    angelic_idle3,
    angelic_idle2,
    angelic_idle1,

};
