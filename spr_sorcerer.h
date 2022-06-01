const unsigned char sorcerer_appear0_left[]={

	  3, 20,0x97,2,
	128

};

const unsigned char sorcerer_appear1_left[]={

	  6, 20,0xa7,2,
	128

};

const unsigned char sorcerer_appear2_left[]={

	  0, 11,0x98,2,
	  7, 12,0x99,2,
	  7, 20,0xa8,2,
	128

};

const unsigned char sorcerer_appear3_left[]={

	  6,  4,0xb7,2,
	  9, 12,0xb9,2,
	  3, 12,0xb8,2,
	  9, 20,0xa9,2,
	128

};

const unsigned char sorcerer_appear4_left[]={

	  6,  3,0x9a,2,
	 14,  3,0x9b,2,
	  5, 11,0xaa,2,
	 13, 11,0xab,2,
	  5, 19,0xba,2,
	 13, 19,0xbb,2,
	128

};

const unsigned char sorcerer_appear5_left[]={

	  6,  2,0x9a,2,
	 14,  2,0x9b,2,
	  5, 10,0xaa,2,
	 13, 10,0xab,2,
	  5, 18,0xba,2,
	 13, 18,0xbb,2,
	128

};

const unsigned char sorcerer_appear6_left[]={

	  6,  1,0x9a,2,
	 14,  1,0x9b,2,
	  5,  9,0xaa,2,
	 13,  9,0xab,2,
	  5, 17,0xba,2,
	 13, 17,0xbb,2,
	128

};

const unsigned char sorcerer_attack0_left[]={

	  6,  1,0x9a,2,
	 14,  1,0x9b,2,
	  5,  9,0x9c,2,
	 13,  9,0xab,2,
	  5, 17,0xba,2,
	 13, 17,0xbb,2,
	128

};

const unsigned char sorcerer_attack1_left[]={

	  6,  1,0x9a,2,
	 14,  1,0x9b,2,
	  5,  9,0xaa,2,
	 13,  9,0x9d,2,
	  5, 17,0xba,2,
	 13, 17,0xbb,2,
	128

};

const unsigned char sorcerer_appear0_right[]={

	 11, 20,0x97,2|OAM_FLIP_H,
	128

};

const unsigned char sorcerer_appear1_right[]={

	  8, 20,0xa7,2|OAM_FLIP_H,
	128

};

const unsigned char sorcerer_appear2_right[]={

	 14, 11,0x98,2|OAM_FLIP_H,
	  7, 12,0x99,2|OAM_FLIP_H,
	  7, 20,0xa8,2|OAM_FLIP_H,
	128

};

const unsigned char sorcerer_appear3_right[]={

	  8,  4,0xb7,2|OAM_FLIP_H,
	  5, 12,0xb9,2|OAM_FLIP_H,
	 11, 12,0xb8,2|OAM_FLIP_H,
	  5, 20,0xa9,2|OAM_FLIP_H,
	128

};

const unsigned char sorcerer_appear4_right[]={

	  8,  3,0x9a,2|OAM_FLIP_H,
	  0,  3,0x9b,2|OAM_FLIP_H,
	  9, 11,0xaa,2|OAM_FLIP_H,
	  1, 11,0xab,2|OAM_FLIP_H,
	  9, 19,0xba,2|OAM_FLIP_H,
	  1, 19,0xbb,2|OAM_FLIP_H,
	128

};

const unsigned char sorcerer_appear5_right[]={

	  8,  2,0x9a,2|OAM_FLIP_H,
	  0,  2,0x9b,2|OAM_FLIP_H,
	  9, 10,0xaa,2|OAM_FLIP_H,
	  1, 10,0xab,2|OAM_FLIP_H,
	  9, 18,0xba,2|OAM_FLIP_H,
	  1, 18,0xbb,2|OAM_FLIP_H,
	128

};

const unsigned char sorcerer_appear6_right[]={

	  8,  1,0x9a,2|OAM_FLIP_H,
	  0,  1,0x9b,2|OAM_FLIP_H,
	  9,  9,0xaa,2|OAM_FLIP_H,
	  1,  9,0xab,2|OAM_FLIP_H,
	  9, 17,0xba,2|OAM_FLIP_H,
	  1, 17,0xbb,2|OAM_FLIP_H,
	128

};

const unsigned char sorcerer_attack0_right[]={

	  8,  1,0x9a,2|OAM_FLIP_H,
	  0,  1,0x9b,2|OAM_FLIP_H,
	  9,  9,0x9c,2|OAM_FLIP_H,
	  1,  9,0xab,2|OAM_FLIP_H,
	  9, 17,0xba,2|OAM_FLIP_H,
	  1, 17,0xbb,2|OAM_FLIP_H,
	128

};

const unsigned char sorcerer_attack1_right[]={

	  8,  1,0x9a,2|OAM_FLIP_H,
	  0,  1,0x9b,2|OAM_FLIP_H,
	  9,  9,0xaa,2|OAM_FLIP_H,
	  1,  9,0x9d,2|OAM_FLIP_H,
	  9, 17,0xba,2|OAM_FLIP_H,
	  1, 17,0xbb,2|OAM_FLIP_H,
	128

};


// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char sorcerer_animation_index[] = {
    0, 1,   // GONE
    2, 10,   // APPEAR
    22, 1,  // ATTACK
    24, 10,  // DISAPPEAR
    0, 1    // DEAD (GONE)
};

const unsigned char* const sorcerer_animation[] = {
    // 0- GONE 0
    no_sprite,
    no_sprite,

    // 1- APPEAR 2
    sorcerer_appear0_left,
    sorcerer_appear1_left,
    sorcerer_appear2_left,
    sorcerer_appear3_left,
    sorcerer_appear4_left,
    sorcerer_appear5_left,
    sorcerer_appear6_left,
    sorcerer_appear5_left,
    sorcerer_appear4_left,
    sorcerer_appear5_left,
    sorcerer_appear0_right,
    sorcerer_appear1_right,
    sorcerer_appear2_right,
    sorcerer_appear3_right,
    sorcerer_appear4_right,
    sorcerer_appear5_right,
    sorcerer_appear6_right,
    sorcerer_appear5_right,
    sorcerer_appear4_right,
    sorcerer_appear5_right,
    // 2- ATTACK 22
    sorcerer_attack0_left,
    sorcerer_attack0_right,
    // 3- DISAPPEAR 12
    sorcerer_attack1_left,
    sorcerer_appear5_left,
    sorcerer_appear4_left,
    sorcerer_appear4_left,
    sorcerer_appear4_left,
    sorcerer_appear4_left,
    sorcerer_appear3_left,
    sorcerer_appear2_left,
    sorcerer_appear1_left,
    sorcerer_appear0_left,
    sorcerer_attack1_right,  
    sorcerer_appear5_right,
    sorcerer_appear4_right,
    sorcerer_appear4_right,
    sorcerer_appear4_right,
    sorcerer_appear4_right,
    sorcerer_appear3_right,
    sorcerer_appear2_right,
    sorcerer_appear1_right,
    sorcerer_appear0_right

};