
static unsigned char level_names[][16] = {
    "               ",
    "  Old Cemetery ",
    "  Temple Ruins ",
    " Temple Crypte ",
    " Temple Madness",
    "  Haunted town ",
    " Tharsh Bridge "
};

#define LEVEL_NUMBER 64
typedef struct {
    unsigned char done[LEVEL_NUMBER];
    unsigned char level_index[LEVEL_NUMBER];
} Level;

Level levels;

void show_map() {
    oam_clear();

    // Set Skull coordinates
    // actors.x[SKULL] = ;
    // actors.y[SKULL] = ;
}

void hide_map() {
    pal_bg(current_background_palette);
    set_chr_mode_4(chr_4_index);
    set_chr_mode_5(chr_5_index);
    set_scroll_y(255);
}

void load_map() {
    ppu_off();
    vram_adr(NAMETABLE_C);
    vram_unrle(map);
    multi_vram_buffer_horz(level_names[current_level], sizeof(level_names[current_level]), NTADR_C(8, 26));
    set_chr_mode_4(8);
    set_chr_mode_5(9);
    pal_bg(pal_map);
    set_scroll_y(239);    
    ppu_on_all();
}

// INVENTORY


void load_inventory() {
    ppu_off();
    vram_adr(NAMETABLE_C);
    vram_unrle(inventory);
    set_chr_mode_4(8);
    set_chr_mode_5(9);
    pal_bg(pal_map);
    set_scroll_y(239);    
    ppu_on_all();
}

// current_selection is the selected item index, set to current_item before entering here
void manage_inventory() {
    // Show items
    for (i = 0; i < ITEM_NUMBER; ++i) {
        if (items.is_active) {
            temp3 = 69 + i * 24;
            oam_spr(temp3, 44, items.sprite[i], 0x00);
        }
    }

    if (actors.y[CURSOR] == SAVE_BUTTON_Y) {
        if (pad1_new & PAD_A) {
            // TODO Save game!
            // TODO Write "Game Saved!" ... temporary??
            // TODO Play sound fx?
        }

        if (pad1_new & PAD_UP) {
            actors.state[CURSOR] = CURSOR_SMALL;
            // Update cursor x,y
            temp3 = current_selection * 24;
            actors.x[CURSOR] = 56 + temp3;
            actors.y[CURSOR] = INVENTORY_ITEM_Y;
        }
    } else {
        // Update cursor x
        temp3 = current_selection * 24;
        actors.x[CURSOR] = 56 + temp3;
        
        if (pad1_new & PAD_LEFT) {
            if (current_selection == 0) {
                current_selection = 5;
            } else {
                --current_selection;
            }
            // TODO: Play sound
        }

        if (pad1_new & PAD_RIGHT) {
            if (current_selection == 5) {
                current_selection = 0;
            } else {
                ++current_selection;
            }
            // TODO: Play sound
        }

        if (pad1_new & PAD_A) {
            current_item = current_selection;
            exit_inventory = TRUE;
        }

        if (pad1_new & PAD_B) {
            exit_inventory = TRUE;
        }

        if (pad1_new & PAD_DOWN) {
            actors.x[CURSOR] = SAVE_BUTTON_X;
            actors.y[CURSOR] = SAVE_BUTTON_Y;
            actors.state[CURSOR] = CURSOR_LARGE;
        }
    }

    if (pad1_new & PAD_SELECT) {
        exit_inventory = TRUE;
    }

    if (exit_inventory) {
        hide_map();
        actors.state[CURSOR] = INACTIVE;
        exit_inventory = FALSE;
        game_state = MAIN;
    }
}