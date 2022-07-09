// COLLISION
#include "Collision/master_collision.h"
// LEVELS
#include "I-CHR/temple2/temple2.h"
#include "I-CHR/temple3/temple3.h"
#include "I-CHR/temple4/temple4.h"
#include "I-CHR/title_screen.pngE/story.h"
#include "I-CHR/town-ruins.pngE/town_ruins.h"
#include "I-CHR/map.pngE/inventory.h"
#include "I-CHR/town-farm/farm.h"

// TEST
#include "I-CHR/test/bombable.h"

// SPRITES
#include "spr_devil.h"
#include "spr_door.h"
#include "spr_fireball.h"
#include "spr_skull_door.h"
#include "spr_sorcerer.h"
#include "spr_staff.h"
#include "spr_stainedglass.h"
#include "spr_villagers.h"
#include "spr_bomb.h"
#include "spr_cursor.h"
#include "spr_farm.h"

// MAP Logic
#include "map_logic.h"

void set_animation_info() {
    j = actors.state[draw_index] << 1;
    switch (actors.type[draw_index]) {
        case TYPE_SKELETON:
            animation_index = skeleton_animation_index[j];
            ++j;
            frame_count = skeleton_animation_index[j];
            animation_array = skeleton_animation;
            break;
        case TYPE_GLASS:
            animation_index = glass_animation_index[j];
            ++j;
            frame_count = glass_animation_index[j];
            animation_array = glass_animation;
            break;
        case TYPE_CRATE:
            animation_index = 0;
            frame_count = 1;
            animation_array = crate_animation;
            break;
        case TYPE_HOUSE_DOOR:
            animation_index = house_door_animation_index[j];
            ++j;
            frame_count = house_door_animation_index[j];
            animation_array = house_door_animation;
            break;
        case TYPE_CROW:
            animation_index = crow_animation_index[j];
            ++j;
            frame_count = crow_animation_index[j];
            animation_array = crow_animation;
            break;
        case TYPE_DEVIL:
            animation_index = devil_animation_index[j];
            ++j;
            frame_count = devil_animation_index[j];
            animation_array = devil_animation;
            break;
        case TYPE_SKULL:
            animation_index = skull_animation_index[j];
            ++j;
            frame_count = skull_animation_index[j];
            animation_array = skull_animation;
            break;
        case TYPE_GATE:
            animation_index = gate_animation_index[j];
            ++j;
            frame_count = gate_animation_index[j];
            animation_array = gate_animation;
            break;
        case TYPE_TREE:
            animation_index = 0;
            frame_count = 1;
            animation_array = tree_animation;
            break;
        case TYPE_ANGELIC:
            animation_index = angelic_animation_index[j];
            ++j;
            frame_count = angelic_animation_index[j];
            animation_array = angelic_animation;
            break;
        case TYPE_SKULL_PILE:
            animation_index = 0;
            frame_count = 1;
            animation_array = skull_pile_animation;
            break;
        case TYPE_LIGHTNING:
            animation_index = lightning_animation_index[j];
            ++j;
            frame_count = lightning_animation_index[j];
            animation_array = lightning_animation;
            break;
        case TYPE_NECROMANCER:
            animation_index = necromancer_animation_index[j];
            ++j;
            frame_count = necromancer_animation_index[j];
            animation_array = necromancer_animation;
            break;
        // case TYPE_STARS:
        //     animation_index = stars_animation_index[j];
        //     ++j;
        //     frame_count = stars_animation_index[j];
        //     animation_array = stars_animation;
        //     break;
        case TYPE_VILLAGER1:
            animation_index = villager_animation_index[j];
            ++j;
            frame_count = villager_animation_index[j];
            animation_array = villager_animation;
            break;
        case TYPE_TORCHES:
            animation_index = torch_animation_index[j];
            ++j;
            frame_count = torch_animation_index[j];
            animation_array = torch_animation;
            break;
        case TYPE_HERO:
            animation_index = hero_animation_index[j];
            ++j;
            frame_count = hero_animation_index[j];
            animation_array = hero_animation;
            break;
        case TYPE_SORCERER:
            animation_index = sorcerer_animation_index[j];
            ++j;
            frame_count = sorcerer_animation_index[j];
            animation_array = sorcerer_animation;
            break;
        case TYPE_PARALYZER:
            animation_index = fireball_animation_index[j];
            ++j;
            frame_count = fireball_animation_index[j];
            animation_array = fireball_animation;
            break;
        case TYPE_SKELETON_KING:
            animation_index = 0;
            frame_count = 1;
            animation_array = skeleton_king_animation;
            break;
        case TYPE_SKULL_DOOR:
            animation_index = skull_door_animation_index[j];
            ++j;
            frame_count = skull_door_animation_index[j];
            animation_array = skull_door_animation;
            break;
        case TYPE_ITEM_BOMB:
            animation_index = bomb_animation_index[j];
            ++j;
            frame_count = bomb_animation_index[j];
            animation_array = bomb_animation;
            break;
        case TYPE_CURSOR:
            animation_index = cursor_animation_index[j];
            ++j;
            frame_count = cursor_animation_index[j];
            animation_array = cursor_animation;
            break;
        case TYPE_SCARECROW:
            animation_index = 0;
            frame_count = 1;
            animation_array = scarecrow_animation;
            break;
        case TYPE_PUMPKIN:
            animation_index = pumpkin_animation_index[j];
            ++j;
            frame_count = pumpkin_animation_index[j];
            animation_array = pumpkin_animation;
            break;
        case TYPE_TRIGGER:
            animation_index = 0;
            frame_count = 1;
            animation_array = trigger_animation;
            break;
    }
}


void init_paddles() {
    // Init paddles
    for (i = 0; i < paddle_count; ++i) {
        if (actors.type[i] == TYPE_PAD_HORZ) {
            actors.width[i] = actors.state[i]; // State is PAD_SHORT 16 or PAD_LONG 32
            actors.height[i] = 0x04;  // 4
            actors.bbox_y[i] = 0x02;
        } else {
            actors.width[i] = 0x04;
            actors.height[i] = actors.state[i];
            actors.bbox_x[i] = 0x02;
        }
        actors.maxSpeed[i] = 250;
    }
}

void init_skull() {
    actors.width[SKULL] = 6;
    actors.height[SKULL] = 6;
    actors.bbox_x[SKULL] = 1;
    actors.bbox_y[SKULL] = 1;
    actors.xDir[SKULL] = RIGHT;
    actors.yDir[SKULL] = UP;
    actors.xSpeed[SKULL] = 100;
    actors.ySpeed[SKULL] = 100;
    actors.minSpeed[SKULL] = 64;
    actors.maxSpeed[SKULL] = 250;
    actors.animation_delay[SKULL] = 8;
    actors.type[SKULL] = TYPE_SKULL;
    actors.state[SKULL] = INACTIVE;
}
