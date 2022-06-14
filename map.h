
static unsigned char level_names[][16] = {
    "     ALTAR     ",
    "  Old cemetery ",
    "  Temple ruins ",
    " Temple crypte ",
    " Temple madness",
    "  Haunted town "
};

// #define MAP_LOCATION_NUMBER 8
// typedef struct {
//     unsigned char done[MAP_LOCATION_NUMBER];
//     unsigned char level_index[MAP_LOCATION_NUMBER];
// } MapLocation;

// MapLocation locations;

void set_map() {
    oam_clear();
    set_chr_mode_4(8);
    set_chr_mode_5(9);
    pal_col(0x00, 0x0f); // BG BLACK
    pal_col(0x01, 0x18); // MAP BACK COLOR
    pal_col(0x02, 0x16); // TEXT
    pal_col(0x03, 0x38); // INK COLOR
}

void show_map() {
    // NAME OF LEVEL
    multi_vram_buffer_horz(level_names[current_level], sizeof(level_names[current_level]), NTADR_C(8, 26));

    // Set Skull coordinates
    // actors.x[SKULL] = ;
    // actors.y[SKULL] = ;

    set_map();

    set_scroll_y(0x0100);    
}

void hide_map() {
    pal_bg(current_background_palette);
    set_chr_mode_4(chr_4_index);
    set_chr_mode_5(chr_5_index);
    set_scroll_y(0x0000);
}

void load_map() {
    ppu_off();
    vram_adr(NAMETABLE_C);
    vram_unrle(map);
    ppu_on_all();
}