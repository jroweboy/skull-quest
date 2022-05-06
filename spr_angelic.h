const unsigned char angelic_idle1[]={
	  0,  0,0x93,3,
	  7,  0,0x93,3|OAM_FLIP_H,
	  0,  8,0x82,3,
	  8,  8,0x83,3,
	  0, 16,0x80,3,
	  8, 16,0x81,3,
	128
};

const unsigned char angelic_idle2[]={
	  0,  1,0x93,3,
	  7,  1,0x93,3|OAM_FLIP_H,
	  0,  9,0x82,3,
	  8,  9,0x83,3,
	  0, 17,0x80,3,
	  8, 17,0x81,3,
	128
};

const unsigned char angelic_idle3[]={
	  0,  2,0x93,3,
	  7,  2,0x93,3|OAM_FLIP_H,
	  0, 10,0x82,3,
	  8, 10,0x83,3,
	  0, 18,0x80,3,
	  8, 18,0x81,3,
	128
};

const unsigned char angelic_face[]={
	- 2, 12,0x92,3,
	  6,  0,0x90,3,
	 14,  0,0x91,3,
	  6,  8,0xa0,3,
	 14,  8,0xa1,3,
	 22,  0,0xa3,3,
	  6, 16,0xb0,3,
	 14, 18,0xb1,3,
	 14, 16,0xb2,3,
	 22, 15,0xb3,3,
	 22,  8,0xa2,3,
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
