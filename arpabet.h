#ifndef __ARPABET_H__
#define __ARPABET_H__

#include "voice_types.h"

// Phoneme type enumeration
#define PHON_MONO   0    // Monopthong
#define PHON_DIPTHO 1    // Dipthong
#define PHON_STOP   2    // Stop
#define PHON_FRIC   3    // Fricative
#define PHON_AFFRIC 4    // Affricate
#define PHON_LIQUID 5    // Liquid
#define PHON_NASAL  6    // Nasal
#define PHON_SEMI   7    // Semivowel
#define PHON_ASPIR  8    // Aspirate
#define PHON_MAX    9

// A letter of the ARPAbet
typedef struct ARPAsym
{
    char name[4];
    int type;
} ARPAsym;

// Phoneme types in string format
extern const char *Phontype[];

// Default phoneme duration (Sagan) -- May vary at different points in word.
extern const float Phontime[];

// TODO: Dipthong map
extern const ARPAsym Dipthomap[];

// The ARPAbet
extern const ARPAsym ARPAbet[NUM_PHONEMES];
#endif//__ARPABET_H__
