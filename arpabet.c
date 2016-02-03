#include "arpabet.h"

// Phoneme types in string format
const char *Phontype[] =
  {
    "Vowel", "Stop", "Fricative", "Affricate",
    "Liquid", "Nasal", "Semivowel", "Aspirate"
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
    {"B", 1},    {"D", 1},    {"G", 1},
    {"K", 1},    {"P", 1},    {"T", 1},

    // Fricatives
    {"DH", 2},    {"F", 2},    {"S", 2},    {"SH", 2},
    {"TH", 2},    {"V", 2},    {"Z", 2},    {"ZH", 2},

    // Affricates
    {"CH", 3},    {"JH", 3},

    // Liquids
    {"L", 4},    {"R", 4},

    // Nasals
    {"M", 5},    {"N", 5},    {"NG", 5},

    // Semivowels
    {"W", 6},    {"Y", 6},

    // Aspirates
    {"HH", 7}
  };

