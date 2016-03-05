#ifndef __ARPABET_H__
#define __ARPABET_H__

#include "voice_types.h"

// Phoneme type enumeration
enum PHONTYPE
{
    /* Voiced. */
    PHON_MONO   = 0,    // Monopthong
    PHON_DIPTHO,        // Dipthong
    PHON_LIQUID,        // Liquid
    PHON_NASAL,         // Nasal
    PHON_SEMI,          // Semivowel

    /* Mouthed. */
    PHON_STOP,          // Stop
    PHON_FRIC,          // Fricative
    PHON_AFFRIC,        // Affricate
    PHON_ASPIR,         // Aspirate
    PHON_MAX
};

// A letter of the ARPAbet
typedef struct ARPAsym
{
    char name[4];
    int type;
} ARPAsym;

// Phoneme types in string format
extern const char *Phontype[];

// Default phoneme duration
extern const float Phontime[];

// TODO: Dipthong map
extern const ARPAsym Dipthomap[];

// The ARPAbet
extern const ARPAsym ARPAbet[NUM_PHONEMES];
#endif//__ARPABET_H__
