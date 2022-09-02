
#ifndef _BANK4_H
#define _BANK4_H

// Input and collision routines

// External calls
#pragma wrapped-call(push, bank_trampoline, bank)
void check_main_input();
void update_skull();
unsigned char get_collision_type();
#pragma wrapped-call(pop)

void add_x_speed(unsigned char val);
void subtract_x_speed(unsigned char val);
void add_y_speed(unsigned char val);
void subtract_y_speed(unsigned char val);
void move_horizontal_paddle();
void move_vertical_paddle();
void check_paddle_collision();
void check_enemy_collision();
char is_skull_collision_paddle();
char is_paddle_collision_skull();
unsigned char create_item_at_skull_pos();
void hit_brick(char tile_type);
void remove_brick(char tile_type);
void add_xp(unsigned char value, unsigned char pos);
unsigned char set_collision_data();

char skull_was_beside() {
    return actors.x[SKULL] + 1 > actors.x[pad_index] + actors.width[pad_index] + actors.bbox_x[pad_index] ||
           actors.x[SKULL] + 7 < actors.x[pad_index] + actors.bbox_x[pad_index];
}

char skull_was_above() {  // or below !!!
    return actors.y[SKULL] + 1 > actors.y[pad_index] + actors.height[pad_index] + actors.bbox_y[pad_index] ||
           actors.y[SKULL] + 7 < actors.y[pad_index] + actors.bbox_y[pad_index];
}

void check_main_input() {
    for (pad_index = 0; pad_index < paddle_count; ++pad_index) {
        param1 = pad_index;
        if (actors.type[pad_index] == TYPE_PAD_HORZ) {
            // Horizontal paddle
            if (actors.counter[pad_index]) {
                --actors.counter[pad_index];
            } else {
                if (pad1 & PAD_LEFT) {
                    add_x_speed(80);
                    actors.xDir[pad_index] = LEFT;
                }
                if (pad1 & PAD_RIGHT) {
                    add_x_speed(80);
                    actors.xDir[pad_index] = RIGHT;
                }
            }
            move_horizontal_paddle();
        } else {
            if (actors.counter[pad_index]) {
                --actors.counter[pad_index];
            } else {
                // Vertical paddle
                if (pad1 & PAD_UP) {
                    add_y_speed(80);
                    actors.yDir[pad_index] = UP;
                }
                if (pad1 & PAD_DOWN) {
                    add_y_speed(80);
                    actors.yDir[pad_index] = DOWN;
                }
            }
            move_vertical_paddle();
        }
    }

    if (pad1_new & PAD_A) {
        if (skull_launched) {
            actors.xVelocity[0] = PAD_VELOCITY;
            actors.xVelocity[1] = PAD_VELOCITY;
            actors.yVelocity[2] = PAD_VELOCITY;
            actors.yVelocity[3] = PAD_VELOCITY;
        } else {
            skull_launched = TRUE;
            actors.state[SKULL] = ROTATE_H;
        }
    }

    if (pad1_new & PAD_B) {
        // TEST THINGS
        // actors.state[SORCERER] = APPEARING;

        // Crow chasing skull:
        // actors.counter[CROW] = NULL;
        // actors.current_frame[CROW] = NULL;
        // actors.state[CROW] = CHASING;
        // actors.animation_delay[CROW] = 8;
        // /////////

        switch (items.type[current_item]) {
            case TYPE_ITEM_MAGNET:
                actors.yDir[SKULL] = DOWN;
                actors.xDir[SKULL] = actors.x[SKULL] < actors.x[0] ? RIGHT : LEFT;
                break;
            case TYPE_ITEM_BOMB:
                create_item_at_skull_pos();
                break;
        }
    }

    if (pad1_new & PAD_SELECT) {
        game_state = INVENTORY;
    }

    if (pad1_new & PAD_START) {
        game_state = MAP;
    }
}

// Paddle collision with wall and skull
// param1: actor index
void move_horizontal_paddle() {
    temp_x_col = actors.x[pad_index];
    temp_y_col = actors.y[pad_index];

    // Check wall collision:
    if (actors.xDir[pad_index] == LEFT) {
        --temp_x_col;
        if (get_collision_type()) {
            ++temp_x_col;
        } else {
            temp_x_col = actors.x[pad_index] + get_x_speed();
        }
    } else {
        temp_x_col += actors.width[pad_index];
        if (get_collision_type()) {
            temp_x_col = actors.x[pad_index];
        } else {
            temp_x_col = actors.x[pad_index] + get_x_speed();
        }
    }

    // Hit the skull
    if (is_paddle_collision_skull()) {
        if (pad1 & 0b00000011 && skull_was_beside()) {
            actors.xVelocity[SKULL] = 70;
        }
    } else {
        // Everything's fine, update x
        actors.x[pad_index] = temp_x_col;
    }

    // FRICTION
    if (actors.xSpeed[pad_index]) {
        subtract_x_speed(16);
    }
}

// param1: actor index
void move_vertical_paddle() {
    temp_x_col = actors.x[pad_index];
    temp_y_col = actors.y[pad_index];

    // Check wall collision:
    if (actors.yDir[pad_index] == UP) {
        --temp_y_col;
        if (get_collision_type()) {
            ++temp_y_col;
        } else {
            temp_y_col = actors.y[pad_index] + get_y_speed();
        }
    } else {
        temp_y_col += actors.height[pad_index];
        if (get_collision_type()) {
            temp_y_col = actors.y[pad_index];
        } else {
            temp_y_col = actors.y[pad_index] + get_y_speed();
        }
    }

    // Hit the skull
    if (is_paddle_collision_skull()) {
        if ((pad1 & 0b00001100) && skull_was_above()) {
            actors.yVelocity[SKULL] = 70;
        }
    } else {
        // Everything's fine, update x
        actors.y[pad_index] = temp_y_col;
    }

    // FRICTION
    if (actors.ySpeed[pad_index]) {
        subtract_y_speed(16);
    }
}

char is_beside_brick() {
    // Old row
    temp = actors.y[SKULL];
    if (actors.yDir[SKULL] == DOWN) {
        temp += 6;
    } else {
        ++temp;
    }
    temp = temp >> 3;

    // New row
    temp2 = temp_y_col >> 3;

    temp3 = temp == temp2;
    return temp3;

    // return temp == temp2;
}

void update_skull() {
    // TODO: We should probably check to make sure skull is within boundaries
    backup_col_type = NULL;

    if (skull_launched) {
        // Required for get_x_speed()
        param1 = SKULL;

        temp_x = actors.x[SKULL] + get_x_speed();
        temp_y = actors.y[SKULL] + get_y_speed();

        if (actors.xDir[SKULL] == RIGHT) {
            if (actors.yDir[SKULL] == DOWN) {
                // Going RIGHT DOWN
                temp_x_col = temp_x + 6;
                temp_y_col = temp_y + 6;
                has_corner_col = get_collision_type();

                // Check left down
                temp_x_col = temp_x; ++temp_x_col;
                if (set_collision_data()) {
                    if (has_corner_col == FALSE || temp_x % 8 < 5) {
                        if (backup_col_type != COL_TYPE_SOFT) {
                            actors.yDir[SKULL] = UP;
                            temp_y -= temp_y_col % 8; --temp_y;
                        }
                        has_corner_col = FALSE;
                        do_skull_tile_collision();
                    }
                }
                
                // Check right up
                temp_x_col = temp_x + 6;
                temp_y_col = temp_y; ++temp_y_col;
                if (set_collision_data()) {
                    if (has_corner_col == FALSE || temp_y % 8 < 5) {
                        if (backup_col_type != COL_TYPE_SOFT) {
                            actors.xDir[SKULL] = LEFT;
                            temp_x -= temp_x_col % 8; --temp_x;
                        }
                        has_corner_col = FALSE;
                        do_skull_tile_collision();
                    }
                } 
                
                if (has_corner_col) {
                    // Check right down
                    temp_x_col = temp_x + 6;
                    temp_y_col = temp_y + 6;
                    set_collision_data();
                    // TODO? Check hit exact corner, reverse both directions...
                    if (backup_col_type != COL_TYPE_SOFT) {
                        if (is_beside_brick()) {
                            actors.xDir[SKULL] = LEFT;
                            temp_x -= temp_x_col % 8; --temp_x;
                        } else {
                            actors.yDir[SKULL] = UP;
                            temp_y -= temp_y_col % 8; --temp_y;
                        }
                    }
                    do_skull_tile_collision();
                }
            } else {
                // Going RIGHT UP
                temp_x_col = temp_x + 6;
                temp_y_col = temp_y; ++temp_y_col;
                has_corner_col = get_collision_type();

                // Check right down
                temp_y_col = temp_y + 6;
                if (set_collision_data()) {
                    if (has_corner_col == FALSE || temp_y % 8 > 5) {
                        if (backup_col_type != COL_TYPE_SOFT) {
                            actors.xDir[SKULL] = LEFT;
                            temp_x -= temp_x_col % 8; --temp_x;
                        }
                        has_corner_col = FALSE;
                        do_skull_tile_collision();
                    }
                } 
                
                // Check left up
                temp_x_col = temp_x; ++temp_x_col;
                temp_y_col = temp_y; ++temp_y_col;
                if (set_collision_data()) {
                    if (has_corner_col == FALSE || temp_x % 8 < 5) {
                        if (backup_col_type != COL_TYPE_SOFT) {
                            actors.yDir[SKULL] = DOWN;
                            temp_y += 8 - (temp_y % 8);
                        }
                        has_corner_col = FALSE;
                        do_skull_tile_collision();
                    }
                }

                // Check right up
                if (has_corner_col) {
                    temp_x_col = temp_x + 6;
                    temp_y_col = temp_y; ++temp_y_col;
                    set_collision_data();
                    // TODO? Check hit exact corner, reverse both directions...
                    if (backup_col_type != COL_TYPE_SOFT) {
                        if (is_beside_brick()) {
                            actors.xDir[SKULL] = LEFT;
                            temp_x -= temp_x_col % 8; --temp_x;
                        } else {
                            actors.yDir[SKULL] = DOWN;
                            temp_y += 8 - (temp_y % 8);
                        }
                    }
                    do_skull_tile_collision();
                }
            }
        } else {
            if (actors.yDir[SKULL] == DOWN) {
                // Going LEFT DOWN
                temp_x_col = temp_x; ++temp_x_col;
                temp_y_col = temp_y + 6;
                has_corner_col = get_collision_type();

                // Check right down
                temp_x_col = temp_x + 6;
                if (set_collision_data()) {
                    if (has_corner_col == FALSE || temp_x % 8 > 5) {
                        if (backup_col_type != COL_TYPE_SOFT) {
                            actors.yDir[SKULL] = UP;
                            temp_y -= temp_y_col % 8; --temp_y;
                        }
                        has_corner_col = FALSE;
                        do_skull_tile_collision();
                    }
                }

                // Check left up
                temp_x_col = temp_x; ++temp_x_col;
                temp_y_col = temp_y; ++temp_y_col;
                if (set_collision_data()) {
                    if (has_corner_col == FALSE || temp_y % 8 < 5) {
                        if (backup_col_type != COL_TYPE_SOFT) {
                            actors.xDir[SKULL] = RIGHT;
                            temp_x += 8 - temp_x % 8;
                        }
                        has_corner_col = FALSE;
                        do_skull_tile_collision();
                    }
                } 
                
                // Check left down
                if (has_corner_col) {
                    temp_x_col = temp_x; ++temp_x_col;
                    temp_y_col = temp_y + 6;
                    set_collision_data();
                    // TODO? Check hit exact corner, reverse both directions...
                    if (backup_col_type != COL_TYPE_SOFT) {
                        if (is_beside_brick()) {
                            actors.xDir[SKULL] = RIGHT;
                            temp_x += 8 - temp_x % 8;
                        } else {
                            actors.yDir[SKULL] = UP;
                            temp_y -= temp_y_col % 8; --temp_y;
                        }
                    }
                    do_skull_tile_collision();
                }
            } else {
                // Going LEFT UP
                temp_x_col = temp_x; ++temp_x_col;
                temp_y_col = temp_y; ++temp_y_col;
                has_corner_col = get_collision_type();

                // Check right up
                temp_x_col = temp_x + 6;
                if (set_collision_data()) {
                    if (has_corner_col == FALSE || temp_x % 8 >= 5) {
                        if (backup_col_type != COL_TYPE_SOFT) {
                            actors.yDir[SKULL] = DOWN;
                            temp_y += 8 - (temp_y % 8);
                        }
                        has_corner_col = FALSE;
                        do_skull_tile_collision();
                    }
                }

                // Check left down
                temp_x_col = temp_x; ++temp_x_col;
                temp_y_col = temp_y + 6;
                if (set_collision_data()) {
                    if (has_corner_col == FALSE || temp_y % 8 >= 5) {
                        if (backup_col_type != COL_TYPE_SOFT) {
                            actors.xDir[SKULL] = RIGHT;
                            temp_x += 8 - (temp_x % 8);
                        }
                        has_corner_col = FALSE;
                        do_skull_tile_collision();
                    }
                } 
                
                // Check left up
                if (has_corner_col) {
                    temp_x_col = temp_x; ++temp_x_col;
                    temp_y_col = temp_y; ++temp_y_col;
                    set_collision_data();
                    // TODO? Check hit exact corner, reverse both directions...
                    if (backup_col_type != COL_TYPE_SOFT) {
                        if (is_beside_brick()) {
                            actors.xDir[SKULL] = RIGHT;
                            temp_x += 8 - (temp_x % 8);
                        } else {
                            actors.yDir[SKULL] = DOWN;
                            temp_y += 8 - (temp_y % 8);
                        }
                    }
                    do_skull_tile_collision();
                }
            }
        }

        check_paddle_collision();

        check_enemy_collision();

    } else {
        // Skull not launched, follow the paddle
        temp_x = actors.x[PADDLE] + (actors.width[PADDLE] >> 1) - (actors.width[SKULL] >> 1);
        temp_y = actors.y[PADDLE] + actors.bbox_y[PADDLE] - actors.height[SKULL] - actors.bbox_x[SKULL];
    }

    debug_show_byte(temp_x);

    // All is fine, we can update x and y
    actors.x[SKULL] = temp_x;
    actors.y[SKULL] = temp_y;
}

// Skull collision with paddle
void check_paddle_collision() {
    for (pad_index = 0; pad_index < paddle_count; ++pad_index) {
        if (is_skull_collision_paddle()) {
            temp = actors.state[pad_index] == PAD_SHORT ? 0 : 4;
            if (actors.type[pad_index] == TYPE_PAD_HORZ) {
                // horizontal paddle
                if (temp_x <= actors.x[pad_index] + paddle_hit_zones[temp++]) {
                    actors.xDir[SKULL] = LEFT;
                    actors.xSpeed[SKULL] = 140;
                    actors.ySpeed[SKULL] = 60;
                } else if (temp_x <= actors.x[pad_index] + paddle_hit_zones[temp++]) {
                    actors.xDir[SKULL] = LEFT;
                    actors.xSpeed[SKULL] = 100;
                    actors.ySpeed[SKULL] = 100;
                } else if (temp_x <= actors.x[pad_index] + paddle_hit_zones[temp++]){
                    actors.xSpeed[SKULL] = 60;
                    actors.ySpeed[SKULL] = 140;
                } else if (temp_x <= actors.x[pad_index] + paddle_hit_zones[temp++]) {
                    actors.xDir[SKULL] = RIGHT;
                    actors.xSpeed[SKULL] = 140;
                    actors.ySpeed[SKULL] = 60;
                } else {
                    actors.xDir[SKULL] = RIGHT;
                    actors.xSpeed[SKULL] = 100;
                    actors.ySpeed[SKULL] = 100;
                }
                // Skull going up or down?
                actors.yDir[SKULL] = actors.y[SKULL] < actors.y[pad_index] ? UP : DOWN;
            } else {
                // vertical paddle
                if (temp_y <= actors.y[pad_index] + paddle_hit_zones[temp++]) {
                    actors.yDir[SKULL] = UP;
                    actors.xSpeed[SKULL] = 60;
                    actors.ySpeed[SKULL] = 140;
                } else if (temp_y <= actors.y[pad_index] + paddle_hit_zones[temp++]) {
                    actors.yDir[SKULL] = UP;
                    actors.xSpeed[SKULL] = 100;
                    actors.ySpeed[SKULL] = 100;
                } else if (temp_y <= actors.y[pad_index] + paddle_hit_zones[temp++]) {
                    actors.xSpeed[SKULL] = 140;
                    actors.ySpeed[SKULL] = 60;
                } else if (temp_y >= actors.y[pad_index] + paddle_hit_zones[temp++]) {
                    actors.yDir[SKULL] = DOWN;
                    actors.xSpeed[SKULL] = 60;
                    actors.ySpeed[SKULL] = 140;
                } else {
                    actors.yDir[SKULL] = DOWN;
                    actors.xSpeed[SKULL] = 100;
                    actors.ySpeed[SKULL] = 100;
                }
                // Skull going left or right?
                actors.xDir[SKULL] = actors.x[SKULL] < actors.x[pad_index] ? LEFT : RIGHT;
            }
        }
    }
}

// We update actors.has_collision[i] at the end.
// So if actors.has_collision[i] == FALSE --> first time collision
void check_enemy_collision() {
    for (i = 6; i < ACTOR_NUMBER; ++i) {
        if (actors.state[i] != INACTIVE) {
            pad_index = i;
            if (is_skull_collision_paddle()) {
                switch (actors.type[i]) {
                    case TYPE_CROW:
                        if (actors.state[i] == IDLE2) {
                            actors.counter[i] = 0;
                            actors.state[i] = FLYING;
                            actors.animation_delay[i] = 8;
                        } else if (actors.state[i] == CHASING) {
                            actors.xDir[i] = LEFT;
                            actors.yDir[i] = UP;
                            actors.xSpeed[i] = 64;
                            actors.ySpeed[i] = 4;
                            actors.state[i] = KIDNAPPING;
                            actors.state[SKULL] = INACTIVE;
                        }
                        break;
                    case TYPE_SKELETON:
                        // TODO Verify if skeleton already dead...
                        actors.counter[i] = 0;
                        actors.current_frame[i] = 0;
                        actors.state[i] = DYING;
                        actors.xDir[SKULL] = -actors.xDir[SKULL];
                        actors.yDir[SKULL] = -actors.yDir[SKULL];
                        break;
                    case TYPE_GLASS:
                        // Play sound
                        if (actors.state[i] == IDLE) {
                            ++actors.state[i];
                        }
                        break;
                    case TYPE_CRATE:
                        // Play sound
                        // TODO Random power up appear?
                        if (skull_was_beside()) {
                            actors.xDir[SKULL] = -actors.xDir[SKULL];
                        } else {
                            actors.yDir[SKULL] = -actors.yDir[SKULL];
                        }
                        break;
                    case TYPE_HOUSE_DOOR:
                        // Play sound
                        if (actors.has_collision[i] == FALSE && actors.state[i] != IDLE2) {
                            story_step = STORY_LEVEL_EVENT;
                            game_state = STORY;
                        }
                        break;
                    case TYPE_SKULL_PILE:
                        if (actors.state[SORCERER] == DEAD) {
                            actors.state[SORCERER] = APPEARING;
                            if (actors.x[SORCERER] > actors.x[0]) {
                                actors.xDir[SORCERER] = LEFT;
                            } else {
                                actors.xDir[SORCERER] = RIGHT;
                            }
                        }
                        break;
                    case TYPE_TRIGGER:
                        if (current_level == LVL_FARM) {
                            // When skull enters crow zone
                            actors.state[CROW] = CHASING;
                            actors.counter[CROW] = NULL;
                            actors.animation_delay[CROW] = 8;
                            level_condition1 = TRUE; // To activate grakk' second dialog
                            // if (story_step > STORY_LEVEL_EVENT) { // Means we already talked to Grakk
                            //      level_condition1 = TRUE; // To activate grakk' second dialog
                            // }
                            actors.state[TRIGGER] = INACTIVE;
                        }
                        break;
                    case TYPE_TRIGGER2:
                        if ((pad1_new & PAD_B) && items.type[current_item] == TYPE_ITEM_SEED && items.is_active[current_item] == TRUE) {
                            // CREATE PUMPKIN
                            SPECIAL_ITEM = create_item_at_skull_pos();                                            
                            actors.animation_delay[SPECIAL_ITEM] = 12;
                            items.is_active[current_item] = FALSE;
                            current_item = 0;
                        }
                        break;
                }
                actors.has_collision[i] = TRUE;
            } else {
                actors.has_collision[i] = FALSE;
            }
        }
    }
}

// !!! width initialized at 0 !!! (serve as timer...)
// animation_delay default is 8
unsigned char create_item_at_skull_pos() {
    temp = get_inactive_actor_index();
    if (temp) {
        actors.x[temp] = actors.x[SKULL];
        actors.y[temp] = actors.y[SKULL];
        actors.animation_delay[temp] = 8;
        actors.width[temp] = NULL; // -> for example, serves as timer before bomb explodes!!!
        actors.state[temp] = IDLE;
        actors.type[temp] = items.type[current_item];
    }
    return temp;
}

char is_skull_collision_paddle() {
    // Skull temp_x and temp_y already include the bbox
    return (temp_x < actors.x[pad_index] + actors.width[pad_index] + actors.bbox_x[pad_index] &&
            temp_x + actors.width[SKULL] > actors.x[pad_index] + actors.bbox_x[pad_index] &&
            temp_y < actors.y[pad_index] + actors.height[pad_index] + actors.bbox_y[pad_index] &&
            temp_y + actors.height[SKULL] > actors.y[pad_index] + actors.bbox_y[pad_index]);
}

char is_paddle_collision_skull() {
    // Skull bbox is hard-coded here
    return (actors.x[SKULL] + 7 > temp_x_col + actors.bbox_x[pad_index] &&
            actors.y[SKULL] + 7 > temp_y_col + actors.bbox_y[pad_index] &&
            actors.x[SKULL] + 1 < temp_x_col + actors.width[pad_index] + actors.bbox_x[pad_index] &&
            actors.y[SKULL] + 1 < temp_y_col + actors.height[pad_index] + actors.bbox_y[pad_index]);
}

void add_x_speed(unsigned char val) {
    temp = actors.xSpeed[param1];
    actors.xSpeed[param1] += val;
    if (actors.xSpeed[param1] > actors.maxSpeed[param1] || actors.xSpeed[param1] < temp) {
        actors.xSpeed[param1] = actors.maxSpeed[param1];
    }
}

void subtract_x_speed(unsigned char val) {
    temp = actors.xSpeed[param1];
    actors.xSpeed[param1] -= val;
    if (actors.xSpeed[param1] < actors.minSpeed[param1] || actors.xSpeed[param1] > temp) {
        actors.xSpeed[param1] = actors.minSpeed[param1];
    }
}

void add_y_speed(unsigned char val) {
    temp = actors.ySpeed[param1];
    actors.ySpeed[param1] += val;
    if (actors.ySpeed[param1] > actors.maxSpeed[param1] || actors.ySpeed[param1] < temp) {
        actors.ySpeed[param1] = actors.maxSpeed[param1];
    }
}

void subtract_y_speed(unsigned char val) {
    temp = actors.ySpeed[param1];
    actors.ySpeed[param1] -= val;
    if (actors.ySpeed[param1] < actors.minSpeed[param1] || actors.ySpeed[param1] > temp) {
        actors.ySpeed[param1] = actors.minSpeed[param1];
    }
}


void do_skull_tile_collision() {
    switch (backup_col_type) {
        case COL_TYPE_SOLID:
            break;
        case COL_TYPE_HURT:
            if (p1_health) {
                --p1_health;
            }
            update_health();
            // TODO!!!
            // Check death
            // play hurt or death sound ?
            // shake screen ??
            // invincible for a few frames ??
            break;
        case COL_TYPE_LONG:
            remove_brick(TILE_BACK);
            backup_nt_index % 2 ? --backup_nt_index : ++backup_nt_index;
            hit_brick(TILE_BACK);
            add_xp(1, HUNDREDS);
            break;
        case COL_TYPE_TALL:
            remove_brick(TILE_BACK);
            temp = temp_y_col >> 3;
            if (temp % 2) {
                temp -= 1;
                backup_col_index -= 16;
            } else {
                temp += 1;
                backup_col_index += 16;
            }
            backup_nt_index = NTADR_A(temp_x_col >> 3, temp);
            hit_brick(TILE_BACK);
            add_xp(1, HUNDREDS);
            break;
        case COL_TYPE_SMALL:
            hit_brick(TILE_BACK);
            add_xp(5, TENS);
            break;
        case COL_TYPE_SOFT:
            // Foliage
            hit_brick(TILE_BACK_GRASS);
            break;
        case COL_TYPE_TOMBSTONE:
            // Tombstone
            c_map[backup_col_index] = 0x11;
            temp = 0b01010101;
            if (((temp_y_col >> 3) - 5) % 2) {
                if (backup_col_index == 182 || backup_col_index == 185) {
                    temp = 0b10100101;
                }
                backup_col_index += 16;
            } else {
                if (backup_col_index == 198 || backup_col_index == 201) {
                    temp = 0b10100101;
                }
                backup_col_index -= 16;
            }
            c_map[backup_col_index] = 0x11;

            one_vram_buffer(temp, (backup_nt_index & 0x2C00) | 0x3C0 | ((backup_nt_index >> 4) & 0x38) | ((backup_nt_index >> 2) & 0x07));
            brick_counter -= 4;  // TODO Dynamic tombstone size to allow more design ideas...
            break;
        case COL_TYPE_BOMBABLE:
            if (actors.counter[SKULL]) {
                temp = TILE_BACK;
            } else {
                temp = TILE_BACK_GRASS;
            }
            hit_brick(temp);
            break;
        case COL_TYPE_2HIT_LONG:
            first_hit_long();
            add_xp(1, HUNDREDS);
            break;
        case COL_TYPE_2HIT_SMALL:
            first_hit_small();
            add_xp(5, TENS);
            break;
        case 0x0B:
            break;
        case 0x0C:
            break;
        case 0x0D:
            break;
        case 0x0E:
            break;
        case 0x0F:
            break;
    }
}

void add_xp(unsigned char value, unsigned char pos) {
    unsigned char result = exp[pos] + value - 0x30;
    if (result > 9) {
        if (pos == TEN_MILLIONS) {
            result = 9;
        } else {
            add_xp(1, pos - 1);
            result -= 10;
        }
    }
    exp[pos] = 0x30 + result;
    update_xp();
}


void hit_brick(char tile_type) {
    remove_brick(tile_type);
    // Play sound
    // Update score
    // Spawn random coin ??
}

void remove_brick(char tile_type) {
    one_vram_buffer(tile_type, backup_nt_index);
    c_map[backup_col_index] &= backup_nt_index % 2 ? 0b11110000 : 0b00001111;
    --brick_counter;
}


// draw_index: already defined in the loop
// pad_index: object to verify collision
char has_collision() {
    return (actors.x[draw_index] + actors.bbox_x[draw_index] < actors.x[pad_index] + actors.width[pad_index] + actors.bbox_x[pad_index] &&
            actors.x[draw_index] + actors.bbox_x[draw_index] + actors.width[draw_index] > actors.x[pad_index] + actors.bbox_x[pad_index] &&
            actors.y[draw_index] + actors.bbox_y[draw_index] < actors.y[pad_index] + actors.height[pad_index] + actors.bbox_y[pad_index] &&
            actors.y[draw_index] + actors.bbox_y[draw_index] + actors.height[draw_index] > actors.y[pad_index] + actors.bbox_y[pad_index]);
}

unsigned char get_collision_type() {
    collision_index = (temp_x_col >> 4) + (((temp_y_col >> 3) - 5) * 16);
    return (temp_x_col >> 3) % 2 ? c_map[collision_index] & 0x0F : c_map[collision_index] >> 4;
}

unsigned char set_collision_data() {
    backup_nt_index = NTADR_A((temp_x_col >> 3), (temp_y_col >> 3));
    backup_col_type = get_collision_type();
    backup_col_index = collision_index;
    return backup_col_type;
}

#endif // _BANK4_H