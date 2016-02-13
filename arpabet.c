#include "arpabet.h"

// Phoneme types in string format
const char *Phontype[] =
{
    "Monopthong", "Dipthong", "Stop",
    "Fricative", "Affricate", "Liquid",
    "Nasal", "Semivowel", "Aspirate"
};

// Default phoneme duration (Sagan) -- May vary at different points in word.
const float Phontime[] =
{
    .3f,    // Vowels
    .16f,   // Stops
    .13f,   // Fricatives
    .2f,    // Affricates
    .1f,    // Liquids
    .1f,    // Nasals
    .07f,   // Semivowels
    .05f    // Aspirates
};

// TODO: Dipthong map
const ARPAsym Dipthomap[] =
{

};

// The ARPAbet
const ARPAsym ARPAbet[NUM_PHONEMES] =
{
    // Monopthongs
    {"AA", 0},    {"AE", 0},    {"AH", 0},    {"AO", 0},    {"EH", 0},
    {"ER", 0},    {"IH", 0},    {"IY", 0},    {"UH", 0},    {"UW", 0},

    // Dipthongs
    {"EY", 1},    {"AY", 1},    {"OW", 1},    {"AW", 1},    {"OY", 1},

    // Stops
    {"B", 2},    {"D", 2},    {"G", 2},
    {"K", 2},    {"P", 2},    {"T", 2},

    // Fricatives
    {"DH", 3},    {"F", 3},    {"S", 3},    {"SH", 3},
    {"TH", 3},    {"V", 3},    {"Z", 3},    {"ZH", 3},

    // Affricates
    {"CH", 4},    {"JH", 4},

    // Liquids
    {"L", 5},    {"R", 5},

    // Nasals
    {"M", 6},    {"N", 6},    {"NG", 6},

    // Semivowels
    {"W", 7},    {"Y", 7},

    // Aspirates
    {"HH", 8}
};

