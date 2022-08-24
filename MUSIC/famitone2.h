
#ifndef _FAMITONE2_H
#define _FAMITONE2_H

// play a music in FamiTone format

void __fastcall__ music_play(unsigned char song);

// stop music

void __fastcall__ music_stop(void);

// pause and unpause music

void __fastcall__ music_pause(unsigned char pause);

// play FamiTone sound effect on channel 0..3

void __fastcall__ sfx_play(unsigned char sound, unsigned char channel);

// play a DPCM sample, 1..63

void __fastcall__ sample_play(unsigned char sample);

#endif // _FAMITONE2_H
