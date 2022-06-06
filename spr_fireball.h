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

const unsigned char fireball_appear1[]={
    0,  0,0x97,0,
	128
};
const unsigned char fireball_appear2[]={
	  0,  0,0xbd,0,
	128
};




// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char fireball_animation_index[] = {
    0, 1,    // NO SPRITE
    2, 2,    // APPEARING
    6, 3     // IDLE
    
};

const unsigned char* const fireball_animation[] = {
    no_sprite,
    no_sprite,

    fireball_appear1,
    fireball_appear2,
    fireball_appear1,
    fireball_appear2,

    fireball1,
    fireball2,
    fireball3,
    fireball1_right,
    fireball2_right,
    fireball3_right

};