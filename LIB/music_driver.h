
#ifndef _MUSIC_DRIVER_H

#define _MUSIC_DRIVER_H

#include "MMC3/mmc3_code.h"

#define FAMISTUDIO_PLATFORM_PAL 0
#define FAMISTUDIO_PLATFORM_NTSC 1

#pragma wrapped-call(push, bank_trampoline, bank)

#ifdef USE_FAMITONE2
void __fastcall__ set_music_speed(unsigned char tempo);
// this will alter the tempo of music, range 1-12 are reasonable, low is faster
// default is 6
// music_play also sets the tempo, and any Fxx effect in the song will too
// you will probably have to repeatedly set_music_speed() every frame
// music_stop() and music_pause() also overwrite this value
#endif  // USE_FAMITONE2

extern unsigned char music_data[]; // Added to test, doesn't work :/

// play a music in FamiTone format
void __fastcall__ music_play(unsigned char song);

// stop music

void __fastcall__ music_stop(void);

// pause and unpause music

void __fastcall__ music_pause(unsigned char pause);

// Init
void __fastcall__ music_init(unsigned char platform, void* music_data);

// Update
void __fastcall__ music_update(void);

// play FamiTone sound effect on channel 0..3

void __fastcall__ sfx_play(unsigned char sound, unsigned char channel);

// play a DPCM sample, 1..63

void __fastcall__ sample_play(unsigned char sample);

#pragma wrapped-call(pop)

#endif