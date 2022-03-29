const unsigned char skeleton_right_still[] = {
    1, 0, 0xdd, 3,
    0, 12, 0xd8, 3,
    0, 5, 0xd9, 3,
    8, 4, 0xdc, 3,
    128

};

const unsigned char skeleton_right_walk1[] = {
    1, -1, 0xdd, 3,
    0, 12, 0xda, 3,
    0, 4, 0xd9, 3,
    8, 3, 0xdc, 3,
    128

};

const unsigned char skeleton_right_walk2[] = {
    2, 0, 0xdd, 3,
    0, 12, 0xdb, 3,
    1, 5, 0xd9, 3,
    9, 4, 0xdc, 3,
    128

};

const unsigned char skeleton_right_walk3[] = {
    1, 12, 0xde, 3,
    1, 4, 0xd9, 3,
    9, 2, 0xdc, 3,
    2, -1, 0xdd, 3,
    128

};

const unsigned char skeleton_right_walk4[] = {
    1, 12, 0xdf, 3,
    1, 5, 0xd9, 3,
    9, 3, 0xdc, 3,
    2, 0, 0xdd, 3,
    128

};

const unsigned char skeleton_right_turn[] = {
    -1, 0, 0xdd, 3,
    0, 12, 0xd8, 3 | OAM_FLIP_H,
    0, 5, 0xd9, 3 | OAM_FLIP_H,
    -8, 4, 0xdc, 3 | OAM_FLIP_H,
    128

};

const unsigned char skeleton_left_still[] = {
    -1, 0, 0xdd, 3 | OAM_FLIP_H,
    0, 12, 0xd8, 3 | OAM_FLIP_H,
    0, 5, 0xd9, 3 | OAM_FLIP_H,
    -8, 4, 0xdc, 3 | OAM_FLIP_H,
    128

};

const unsigned char skeleton_left_walk1[] = {
    -1, -1, 0xdd, 3 | OAM_FLIP_H,
    0, 12, 0xda, 3 | OAM_FLIP_H,
    0, 4, 0xd9, 3 | OAM_FLIP_H,
    -8, 3, 0xdc, 3 | OAM_FLIP_H,
    128

};

const unsigned char skeleton_left_walk2[] = {
    -2, 0, 0xdd, 3 | OAM_FLIP_H,
    0, 12, 0xdb, 3 | OAM_FLIP_H,
    -1, 5, 0xd9, 3 | OAM_FLIP_H,
    -9, 4, 0xdc, 3 | OAM_FLIP_H,
    128

};

const unsigned char skeleton_left_walk3[] = {
    -1, 12, 0xde, 3 | OAM_FLIP_H,
    -1, 4, 0xd9, 3 | OAM_FLIP_H,
    -9, 2, 0xdc, 3 | OAM_FLIP_H,
    -2, -1, 0xdd, 3 | OAM_FLIP_H,
    128

};

const unsigned char skeleton_left_walk4[] = {
    0, 12, 0xdf, 3 | OAM_FLIP_H,
    0, 5, 0xd9, 3 | OAM_FLIP_H,
    -8, 3, 0xdc, 3 | OAM_FLIP_H,
    -1, 0, 0xdd, 3 | OAM_FLIP_H,
    128

};

const unsigned char skeleton_left_turn[] = {
    1, 0, 0xdd, 3 | OAM_FLIP_H,
    0, 12, 0xd8, 3,
    0, 5, 0xd9, 3,
    8, 4, 0xdc, 3,
    128
};

const unsigned char skeleton_dying_right1[]={
	  1, 12,0xdf,3,
	  1,  7,0xd9,3,
	  9,  4,0xdc,3,
	  2,  0,0xdd,3,
	128
};

const unsigned char skeleton_dying_right2[]={
	  1,  9,0xd9,3,
	  1, 12,0xdf,3,
	  9,  6,0xdc,3,
	  2,  1,0xdd,3,
	128
};

const unsigned char skeleton_dying_right3[]={
	  1, 11,0xd9,3,
	  1, 12,0xdf,3,
	  9,  8,0xdc,3,
	  2,  3,0xdd,3,
	128
};

const unsigned char skeleton_dying_right4[]={
	  0, 12,0xd9,3,
	  1, 12,0xdf,3,
	  9, 12,0xdc,3,
	  2,  7,0xdd,3,
	128
};

const unsigned char skeleton_dying_right5[]={
	- 1, 12,0xd9,3,
	  1, 12,0xdf,3,
	  9, 12,0xdc,3,
	  6,  9,0xdd,3|OAM_FLIP_H|OAM_FLIP_V,
	128
};

const unsigned char skeleton_dying_right6[]={
	- 2, 12,0xd9,3,
	  1, 12,0xdf,3,
	  9, 12,0xdc,3,
	  7, 12,0xdd,3,
	128
};

const unsigned char skeleton_dying_right7[]={
	- 2, 12,0xd9,3,
	  1, 12,0xdf,3,
	  9, 12,0xdc,3,
	  9, 12,0xdd,3|OAM_FLIP_V,
	128
};

const unsigned char skeleton_dying_left1[]={
	  1, 12,0xdf,3|OAM_FLIP_H,
	  1,  7,0xd9,3|OAM_FLIP_H,
	- 7,  4,0xdc,3|OAM_FLIP_H,
	  0,  0,0xdd,3|OAM_FLIP_H,
	128
};

const unsigned char skeleton_dying_left2[]={
	  1,  9,0xd9,3|OAM_FLIP_H,
	  1, 12,0xdf,3|OAM_FLIP_H,
	- 7,  6,0xdc,3|OAM_FLIP_H,
	  0,  1,0xdd,3|OAM_FLIP_H,
	128
};

const unsigned char skeleton_dying_left3[]={
	  1, 11,0xd9,3|OAM_FLIP_H,
	  1, 12,0xdf,3|OAM_FLIP_H,
	- 7,  8,0xdc,3|OAM_FLIP_H,
	  0,  3,0xdd,3|OAM_FLIP_H,
	128
};

const unsigned char skeleton_dying_left4[]={
	  2, 12,0xd9,3|OAM_FLIP_H,
	  1, 12,0xdf,3|OAM_FLIP_H,
	- 7, 12,0xdc,3|OAM_FLIP_H,
	  0,  7,0xdd,3|OAM_FLIP_H,
	128
};

const unsigned char skeleton_dying_left5[]={
	  3, 12,0xd9,3|OAM_FLIP_H,
	  1, 12,0xdf,3|OAM_FLIP_H,
	- 7, 12,0xdc,3|OAM_FLIP_H,
	- 4,  9,0xdd,3|OAM_FLIP_V,
	128
};

const unsigned char skeleton_dying_left6[]={
	  4, 12,0xd9,3|OAM_FLIP_H,
	  1, 12,0xdf,3|OAM_FLIP_H,
	- 7, 12,0xdc,3|OAM_FLIP_H,
	- 5, 12,0xdd,3|OAM_FLIP_H,
	128
};

const unsigned char skeleton_dying_left7[]={
	  4, 12,0xd9,3|OAM_FLIP_H,
	  1, 12,0xdf,3|OAM_FLIP_H,
	- 7, 12,0xdc,3|OAM_FLIP_H,
	- 7, 12,0xdd,3|OAM_FLIP_H|OAM_FLIP_V,
	128
};

// 1st element: Animation index
// 2nd element: Number of frames in the animation
const unsigned char skeleton_animation_index[][2] = {
    { 0, 1 }, // IDLE
    { 2, 1}, // TURNING
    { 4, 4 }, // WALKING
    { 12, 7}, // DYING
    { 26, 1}  // DEAD
};

const unsigned char* const skeleton_animation[] = {
    skeleton_left_still,
    skeleton_right_still,

    skeleton_left_turn,
    skeleton_right_turn,

    skeleton_left_walk1,
    skeleton_left_walk2,
    skeleton_left_walk3,
    skeleton_left_walk4,
    skeleton_right_walk1,
    skeleton_right_walk2,
    skeleton_right_walk3,
    skeleton_right_walk4,

    skeleton_dying_left1,
    skeleton_dying_left2,
    skeleton_dying_left3,
    skeleton_dying_left4,
    skeleton_dying_left5,
    skeleton_dying_left6,
    skeleton_dying_left7,
    skeleton_dying_right1,
    skeleton_dying_right2,
    skeleton_dying_right3,
    skeleton_dying_right4,
    skeleton_dying_right5,
    skeleton_dying_right6,
    skeleton_dying_right7,
    
    skeleton_dying_left7,
    skeleton_dying_right7};