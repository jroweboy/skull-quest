void set_skeleton(unsigned char p_index, unsigned char p_x, unsigned char p_y) {
    actors.x[p_index] = p_x;
    actors.y[p_index] = p_y;
    actors.width[p_index] = 0x04;
    actors.height[p_index] = 0x14;
    actors.bbox_x[p_index] = 0x01;
    actors.bbox_y[p_index] = 0x02;
    actors.xSpeed[p_index] = 10;
    actors.maxSpeed[p_index] = 20;
    actors.animation_delay[p_index] = 16;
    actors.state[p_index] = DEAD;
    actors.type[p_index] = TYPE_SKELETON;
}

void set_torch(unsigned char p_index, unsigned char p_x, unsigned char p_y) {
    actors.x[p_index] = p_x;
    actors.y[p_index] = p_y;
    actors.animation_delay[p_index] = 8;
    actors.state[p_index] = IDLE;
    actors.type[p_index] = TYPE_TORCHES;
}

void set_crow(unsigned char p_x, unsigned char p_y) {
    actors.x[CROW] = p_x;
    actors.y[CROW] = p_y;
    actors.state[CROW] = IDLE2;  // IDLE state of crow is 2... don't ask!
    actors.animation_delay[CROW] = 64;
    actors.xSpeed[CROW] = 128;
    actors.ySpeed[CROW] = 32;
    actors.type[CROW] = TYPE_CROW;
    actors.width[CROW] = 16;
    actors.height[CROW] = 16;
}

void set_stars() {
    actors.x[STARS] = ITEM_X + 2;
    actors.y[STARS] = ITEM_Y + 2;
    actors.animation_delay[STARS] = 8;
    actors.state[STARS] = INACTIVE;
    actors.type[STARS] = TYPE_STARS;
}

void init_level_specifics() {
    level_bank = 2;
    switch (current_level) {
        case LVL_ALTAR:
            current_nametable = altar;
            current_collision_map = cemetery_col;
            current_background_palette = pal_altar_bg;
            current_sprite_palette = pal_altar_spr;
            chr_4_index = 2;

            // Skull doesn't appear immediately
            actors.x[SKULL] = 125;
            actors.y[SKULL] = 130;
            actors.state[SKULL] = INACTIVE;

            // Lightning
            LIGHTNING = 12;
            actors.x[LIGHTNING] = 128;
            actors.y[LIGHTNING] = 108;
            actors.animation_delay[LIGHTNING] = 8;
            actors.type[LIGHTNING] = TYPE_LIGHTNING;
            actors.state[LIGHTNING] = IDLE;

            // Torches:
            set_torch(7, 64, 72);
            set_torch(8, 88, 72);
            set_torch(9, 160, 72);
            set_torch(10, 184, 72);

            // Necromancer:
            NECROMANCER = 6;
            actors.x[NECROMANCER] = 120;
            actors.y[NECROMANCER] = 142;
            actors.animation_delay[NECROMANCER] = 20;
            actors.state[NECROMANCER] = IDLE;
            actors.type[NECROMANCER] = TYPE_NECROMANCER;

            // Hero
            HERO = 11;
            actors.animation_delay[HERO] = 1;
            actors.x[HERO] = 120;
            actors.y[HERO] = 72;
            actors.state[HERO] = IDLE;
            actors.type[HERO] = TYPE_HERO;
            break;
        case LVL_CEMETERY:
            // Achievement 1 : Scarecrow
            // Achievement 2 : Skeleton buster

            current_nametable = cemetery;
            current_collision_map = cemetery_col;
            current_background_palette = pal_cemetery_bg;
            current_sprite_palette = pal_cemetery_spr;

            chr_4_index = 2;
            chr_5_index = 3;

            // Paddle
            actors.x[0] = 0x70;
            actors.y[0] = 0xD0;
            actors.state[0] = PAD_LONG;
            actors.type[0] = TYPE_PAD_HORZ;

            // CROW
            CROW = 6;
            set_crow(207, 117);

            // Skeleton index 6 & 7
            SKELETON1 = 7;
            SKELETON2 = 8;
            set_skeleton(SKELETON1, 40, 72);
            actors.xDir[SKELETON1] = LEFT;
            set_skeleton(SKELETON2, 160, 104);
            actors.xDir[SKELETON2] = RIGHT;

            // GATE
            GATE = 9;
            actors.x[GATE] = 120;
            actors.y[GATE] = 48;
            actors.animation_delay[GATE] = 16;
            actors.type[GATE] = TYPE_GATE;
            actors.state[GATE] = IDLE;

            // TREE
            STILL_DECORATION = 10;
            actors.x[STILL_DECORATION] = 219;
            actors.y[STILL_DECORATION] = 61;
            actors.state[STILL_DECORATION] = IDLE;
            actors.type[STILL_DECORATION] = TYPE_TREE;

            // ANGELIC
            GHOST = 11;
            actors.x[GHOST] = 120;
            actors.y[GHOST] = 92;
            actors.animation_delay[GHOST] = 16;
            actors.state[GHOST] = IDLE;
            actors.type[GHOST] = TYPE_ANGELIC;

            break;
        case LVL_TEMPLE1:
            // Achievement 1: Glass Hash (break 3 stained glasses)
            // Achievement 2: Paralyzed by Death (waking the sorcerer by hiting pile of skulls)

            current_nametable = temple;
            current_collision_map = temple_col;
            current_background_palette = pal_temple_bg;
            current_sprite_palette = pal_temple_spr;

            chr_4_index = 0x0A;
            chr_5_index = 0x0B;

            // Paddles
            actors.x[0] = 0x70;  // 14
            actors.y[0] = 0xD0;  // 26
            actors.state[0] = PAD_LONG;
            actors.type[0] = TYPE_PAD_HORZ;

            // Stained glasses
            actors.x[6] = 96;
            actors.x[7] = 120;
            actors.x[8] = 144;
            for (i = 6; i < 9; ++i) {
                actors.animation_delay[i] = 12;
                actors.y[i] = 32;
                actors.type[i] = TYPE_GLASS;
                actors.width[i] = 16;
                actors.height[i] = 50;
                actors.state[i] = IDLE;
            }

            // Pile of skulls
            STILL_DECORATION = 10;
            actors.x[STILL_DECORATION] = 184;
            actors.y[STILL_DECORATION] = 82;
            actors.width[STILL_DECORATION] = 16;
            actors.height[STILL_DECORATION] = 8;
            actors.state[STILL_DECORATION] = IDLE;
            actors.type[STILL_DECORATION] = TYPE_SKULL_PILE;

            // Sorcerer
            SORCERER = 9;
            actors.x[SORCERER] = actors.x[STILL_DECORATION] + 8;
            actors.y[SORCERER] = actors.y[STILL_DECORATION] - 20;
            actors.animation_delay[SORCERER] = 12;
            actors.state[SORCERER] = DEAD;
            actors.type[SORCERER] = TYPE_SORCERER;
            break;
        case LVL_TEMPLE2:
            // Achievement 1:
            // Achievement 2:
            current_nametable = temple2;
            current_collision_map = temple2_col;
            current_background_palette = pal_temple_bg;
            current_sprite_palette = pal_temple_spr;

            chr_4_index = 0x0A;
            chr_5_index = 0x0B;

            // Paddle 1
            actors.x[0] = 14 * 8;
            actors.y[0] = 24 * 8;
            actors.state[0] = PAD_LONG;
            actors.type[0] = TYPE_PAD_HORZ;

            // Paddle 2
            actors.x[1] = 14 * 8;
            actors.y[1] = 9 * 8;
            actors.state[1] = PAD_LONG;
            actors.type[1] = TYPE_PAD_HORZ;
            break;
        case LVL_TEMPLE3:
            // Achievement 1:
            // Achievement 2:
            current_nametable = temple3;
            current_collision_map = temple3_col;
            current_background_palette = pal_temple_bg;
            current_sprite_palette = pal_temple_spr;

            chr_4_index = 0x0A;
            chr_5_index = 0x0B;

            // Paddle 1
            actors.x[0] = 15 * 8;
            actors.y[0] = 12 * 8;
            actors.state[0] = PAD_SHORT;
            actors.type[0] = TYPE_PAD_HORZ;

            // Paddle up
            actors.x[1] = 15 * 8;
            actors.y[1] = 21 * 8;
            actors.state[1] = PAD_SHORT;
            actors.type[1] = TYPE_PAD_HORZ;

            // Paddle left
            actors.x[2] = 8 * 8;
            actors.y[2] = 16 * 8;
            actors.state[2] = PAD_SHORT;
            actors.type[2] = TYPE_PAD_VERT;

            // Paddle right
            actors.x[3] = 23 * 8;
            actors.y[3] = 16 * 8;
            actors.state[3] = PAD_SHORT;
            actors.type[3] = TYPE_PAD_VERT;

            // Skull Door
            DOOR1 = 6;
            actors.x[DOOR1] = 120;
            actors.y[DOOR1] = 21;
            actors.type[DOOR1] = TYPE_SKULL_DOOR;
            actors.state[DOOR1] = IDLE;
            actors.animation_delay[DOOR1] = 32;

            // Torches
            set_torch(7, 104, 32);
            set_torch(8, 144, 32);

            break;
        case LVL_TEMPLE4:
            // Achievement 1:
            // Achievement 2:
            current_nametable = temple4;
            current_collision_map = temple4_col;
            current_background_palette = pal_temple_bg;
            current_sprite_palette = pal_temple_spr;

            chr_4_index = 0x0A;
            chr_5_index = 0x0B;

            // Paddle 1
            actors.x[0] = 14 * 8;
            actors.y[0] = 26 * 8;
            actors.state[0] = PAD_LONG;
            actors.type[0] = TYPE_PAD_HORZ;

            // Paddle up
            actors.x[1] = 9 * 8;
            actors.y[1] = 12 * 8;
            actors.state[1] = PAD_SHORT;
            actors.type[1] = TYPE_PAD_HORZ;

            // Paddle left
            actors.x[2] = 21 * 8;
            actors.y[2] = 12 * 8;
            actors.state[2] = PAD_SHORT;
            actors.type[2] = TYPE_PAD_HORZ;

            // Skull Door
            DOOR1 = 6;
            actors.x[DOOR1] = 120;
            actors.y[DOOR1] = 21;
            actors.type[DOOR1] = TYPE_SKULL_DOOR;
            actors.state[DOOR1] = IDLE;
            actors.animation_delay[DOOR1] = 32;

            // Torches
            set_torch(7, 104, 32);
            set_torch(8, 144, 32);
            break;
        case LVL_TOWN:
            // Achievement 1: Door knocker
            // Achievement 2: Devil slayer

            current_nametable = town_ruins;
            current_collision_map = town_col;
            current_background_palette = pal_town_bg;
            current_sprite_palette = pal_town_spr;

            chr_4_index = 0x0a;
            chr_5_index = 0x0b;

            // Paddle
            actors.x[0] = 0x70;
            actors.y[0] = 0xD0;

            // Doors
            DOOR1 = 6;
            DOOR2 = 7;
            DOOR3 = 8;
            actors.x[DOOR1] = 30;
            actors.y[DOOR1] = 87;
            actors.width[DOOR1] = 16;
            actors.height[DOOR1] = 50;
            actors.state[DOOR1] = IDLE;
            actors.type[DOOR1] = TYPE_HOUSE_DOOR;

            actors.x[DOOR2] = 110;
            actors.y[DOOR2] = 71;
            actors.width[DOOR2] = 16;
            actors.height[DOOR2] = 50;
            actors.state[DOOR2] = IDLE;
            actors.type[DOOR2] = TYPE_HOUSE_DOOR;

            actors.x[DOOR3] = 182;
            actors.y[DOOR3] = 87;
            actors.width[DOOR3] = 16;
            actors.height[DOOR3] = 50;
            actors.state[DOOR3] = IDLE;
            actors.type[DOOR3] = TYPE_HOUSE_DOOR;

            // Crates
            CRATE1 = 9;
            CRATE2 = 10;
            CRATE3 = 11;

            actors.x[CRATE1] = 16;
            actors.y[CRATE1] = 112;
            actors.x[CRATE2] = 48;
            actors.y[CRATE2] = 112;
            actors.x[CRATE3] = 224;
            actors.y[CRATE3] = 112;
            temp = 15;
            actors.width[CRATE1] = temp;
            actors.width[CRATE2] = temp;
            actors.width[CRATE3] = temp;
            actors.height[CRATE1] = temp;
            actors.height[CRATE2] = temp;
            actors.height[CRATE3] = temp;
            temp = 1;
            actors.bbox_x[CRATE1] = temp;
            actors.bbox_y[CRATE1] = temp;
            actors.bbox_x[CRATE2] = temp;
            actors.bbox_y[CRATE2] = temp;
            actors.bbox_x[CRATE3] = temp;
            actors.bbox_y[CRATE3] = temp;
            actors.state[CRATE1] = IDLE;
            actors.state[CRATE2] = IDLE;
            actors.state[CRATE3] = IDLE;
            actors.type[CRATE1] = TYPE_CRATE;
            actors.type[CRATE2] = TYPE_CRATE;
            actors.type[CRATE3] = TYPE_CRATE;

            // DEVIL
            DEVIL = 12;
            actors.x[DEVIL] = 12;
            actors.y[DEVIL] = 152;
            actors.width[DEVIL] = 15;
            actors.height[DEVIL] = 15;
            actors.animation_delay[DEVIL] = 16;
            actors.state[DEVIL] = IDLE;
            actors.type[DEVIL] = TYPE_DEVIL;
            break;
        case LVL_FARM:
            // Achievement 1: Harvester of sorrow
            // Achievement 2: Don't fear the reaper / Scared Scarecrow

            current_nametable = farm;
            current_collision_map = farm_col;
            current_background_palette = pal_town_bg2;
            current_sprite_palette = pal_town_spr2;

            chr_4_index = 0x0a;
            chr_5_index = 0x0b;

            // TODO maybe check for achievement instead, so coming back to level later won't trigger grakk....
            level_condition1 = FALSE;

            // Paddle 1
            actors.x[0] = 6 * 8;
            actors.y[0] = 26 * 8;
            actors.state[0] = PAD_LONG;
            actors.type[0] = TYPE_PAD_HORZ;

            // Paddle up
            actors.x[1] = 9 * 8;
            actors.y[1] = 7 * 8;
            actors.state[1] = PAD_LONG;
            actors.type[1] = TYPE_PAD_HORZ;

            // Paddle right
            actors.x[2] = 23 * 8;
            actors.y[2] = 17 * 8;
            actors.state[2] = PAD_SHORT;
            actors.type[2] = TYPE_PAD_HORZ;

            // Scarecrow
            STILL_DECORATION = 4;
            actors.x[STILL_DECORATION] = 24 * 8;
            actors.y[STILL_DECORATION] = 8 * 8;
            actors.state[STILL_DECORATION] = IDLE;
            actors.type[STILL_DECORATION] = TYPE_SCARECROW;

            // STARS
            STARS = 6;
            set_stars();

            // Doors
            DOOR1 = 7;
            actors.x[DOOR1] = 56;
            actors.y[DOOR1] = 136;
            actors.width[DOOR1] = 16;
            actors.height[DOOR1] = 27;
            actors.animation_delay[DOOR1] = 16;
            actors.state[DOOR1] = IDLE;
            actors.type[DOOR1] = TYPE_HOUSE_DOOR;

            // Crow
            CROW = 8;
            set_crow(192, 60);

            // TRIGGER Entry to crow zone
            TRIGGER = 10;
            actors.x[TRIGGER] = 20 * 8;
            actors.y[TRIGGER] = 13 * 8;
            actors.state[TRIGGER] = IDLE;
            actors.type[TRIGGER] = TYPE_TRIGGER;
            actors.width[TRIGGER] = 8;
            actors.height[TRIGGER] = 16;

            // FARMING AREA!!!
            TRIGGER2 = 11;
            actors.x[TRIGGER2] = 4 * 8;
            actors.y[TRIGGER2] = 8 * 8;
            actors.state[TRIGGER2] = IDLE;
            actors.type[TRIGGER2] = TYPE_TRIGGER2;
            actors.width[TRIGGER2] = 15 * 8;
            actors.height[TRIGGER2] = 3 * 8;
            break;
        case LVL_TEST:
            current_nametable = bombable;
            current_collision_map = bombable_test;
            current_background_palette = pal_temple_bg;
            current_sprite_palette = pal_temple_spr;

            chr_4_index = 0x0A;
            chr_5_index = 0x0B;
            // Paddle
            actors.x[0] = 14 * 8;
            actors.y[0] = 26 * 8;
            actors.state[0] = PAD_LONG;
            actors.type[0] = TYPE_PAD_HORZ;

            actors.x[1] = 14 * 8;
            actors.y[1] = 7 * 8;
            actors.state[1] = PAD_LONG;
            actors.type[1] = TYPE_PAD_HORZ;
            break;
    }
}