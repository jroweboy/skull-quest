// LEVELS
#include "I-CHR/cemetery.pngE/cemetery.h"
#include "I-CHR/church-interior.pngE/temple.h"
#include "I-CHR/temple2/temple2.h"
#include "I-CHR/temple3/temple3.h"
#include "I-CHR/temple4/temple4.h"
#include "I-CHR/title_screen.pngE/story.h"
#include "I-CHR/title_screen.pngE/title_screen.h"

// TEST
#include "I-CHR/test/bombable.h"

const unsigned char altar[396] = {
    0x02, 0x01, 0x02, 0x2a, 0x8f, 0x01, 0x02, 0x09, 0x8d, 0x01, 0x01, 0x8e, 0x01, 0x02, 0x0b, 0x8f,
    0x01, 0x02, 0x09, 0x8d, 0x01, 0x02, 0x05, 0x9d, 0x9e, 0x01, 0x01, 0x8d, 0x01, 0x02, 0x0e, 0x9f,
    0x01, 0x02, 0x09, 0x9f, 0x01, 0x02, 0x0c, 0xad, 0xae, 0xad, 0xae, 0xad, 0xae, 0xad, 0xaf, 0xae,
    0xad, 0xae, 0xad, 0xae, 0xad, 0xae, 0xad, 0xae, 0xad, 0xaf, 0xae, 0xad, 0xae, 0x01, 0x02, 0x25,
    0x9b, 0x9c, 0x01, 0x02, 0x19, 0x9b, 0x9c, 0x01, 0x01, 0x89, 0x8a, 0x01, 0x02, 0x0b, 0x84, 0x94,
    0x01, 0x02, 0x0b, 0x89, 0x8a, 0x01, 0x01, 0x99, 0x9a, 0x01, 0x02, 0x0a, 0xb3, 0xa4, 0xb9, 0xb4,
    0x01, 0x02, 0x0a, 0x99, 0x9a, 0x01, 0x01, 0xa9, 0xaa, 0x01, 0x02, 0x04, 0xb1, 0x01, 0x01, 0xb1,
    0x01, 0x01, 0xb1, 0x8b, 0x8c, 0xb1, 0x01, 0x01, 0xb1, 0x01, 0x01, 0xb1, 0x01, 0x02, 0x04, 0xa9,
    0xaa, 0x01, 0x01, 0xab, 0xac, 0x01, 0x02, 0x04, 0xb2, 0x01, 0x01, 0xb2, 0x01, 0x01, 0xb2, 0x01,
    0x01, 0xb2, 0x01, 0x01, 0xb2, 0x01, 0x01, 0xb2, 0x01, 0x02, 0x04, 0xab, 0xac, 0x01, 0x02, 0x06,
    0x00, 0x00, 0xb1, 0x00, 0x00, 0xb1, 0x00, 0x00, 0xb1, 0xa2, 0xa3, 0xb1, 0x00, 0x00, 0xb1, 0x00,
    0x00, 0xb1, 0x00, 0x00, 0x01, 0x02, 0x09, 0x00, 0x00, 0x01, 0x02, 0x13, 0x00, 0x00, 0x01, 0x02,
    0x30, 0x81, 0x82, 0x81, 0x82, 0x81, 0x82, 0x01, 0x02, 0x18, 0x80, 0x91, 0x85, 0x86, 0x87, 0x88,
    0x93, 0x80, 0x01, 0x02, 0x16, 0x80, 0xa0, 0x81, 0x95, 0x96, 0x97, 0x98, 0x83, 0x90, 0x80, 0x01,
    0x02, 0x15, 0x90, 0xa0, 0x91, 0xa5, 0xa6, 0xa7, 0xa8, 0x92, 0xa0, 0xa0, 0x01, 0x02, 0x15, 0xa0,
    0xb0, 0xa1, 0xb5, 0xb6, 0xb7, 0xb8, 0x82, 0xb0, 0xa0, 0x01, 0x02, 0x15, 0xb0, 0x00, 0x91, 0x93,
    0x91, 0x93, 0x91, 0x93, 0x00, 0xb0, 0x01, 0x02, 0x17, 0x00, 0xba, 0xbb, 0xbc, 0xbd, 0x00, 0x01,
    0x02, 0x10, 0x00, 0x00, 0x01, 0x02, 0x07, 0xba, 0xbb, 0xbb, 0xbd, 0x01, 0x02, 0x07, 0x00, 0x00,
    0x01, 0x02, 0x07, 0xbe, 0xbe, 0x01, 0x02, 0x13, 0xbe, 0xbe, 0x01, 0x02, 0x07, 0xbf, 0xbf, 0x00,
    0x00, 0x01, 0x02, 0x0f, 0x00, 0x00, 0xbf, 0xbf, 0x01, 0x02, 0x09, 0xbe, 0xbe, 0x01, 0x02, 0x0f,
    0xbe, 0xbe, 0x01, 0x02, 0x0b, 0xbf, 0xbf, 0x00, 0x02, 0x0f, 0xbf, 0xbf, 0x01, 0x02, 0x0d, 0xbe,
    0x02, 0x0f, 0x01, 0x02, 0x0f, 0xbf, 0x02, 0x0f, 0x01, 0x02, 0x47, 0x00, 0x8a, 0x88, 0x30, 0x20,
    0xc8, 0x22, 0x20, 0xf0, 0x0f, 0x02, 0x04, 0x0b, 0xf0, 0x00, 0x02, 0x02, 0x03, 0x00, 0x02, 0x04,
    0x55, 0x02, 0x05, 0x00, 0x00, 0x55, 0x02, 0x05, 0x00, 0x00, 0x75, 0x55, 0x02, 0x03, 0xd5, 0x00,
    0x00, 0x0c, 0xf5, 0x02, 0x03, 0x03, 0x00, 0x02, 0x07, 0x00, 0x02, 0x00};