/* Do not include this file directly. 
 * Include vocli.h instead.
 */
#ifndef __VOICE_TYPES_H__
#define __VOICE_TYPES_H__

#include <stdint.h>
#include <limits.h>

#define SPEAKER_MAX 24     // Max length of speaker name
#define NUM_FORMANTS 3     // Number of formants per phoneme
#define NUM_PHONEMES 39    // Number of phonemes per voice (this is really a CMU thing)

// A single formant
typedef struct FmtDef
{
    uint32_t fqc;    // Center frequency
    uint32_t bw;     // Bandwidth
    int32_t dBoffs;  // Amplitude offset (dB)
} FmtDef;

// A phoneme definition
typedef struct PhonDef
{
    uint8_t ID;                   // ARPAbet phoneme ID
    FmtDef Formant[NUM_FORMANTS]; // Formant resonances
    float duration;               // Default phoneme duration
    const char *ARPAsym;          // Corresponding ARPAbet symbol -- is this useful?
} PhonDef;

// A voice definition
// TODO: Handle affricates and dipthongs
// IE: Phoneme[NUM_PHONEMES][2], the second could be ignored in the case of singular phonemes?
// Need a way to combine two phonemes into one, only in certain cases, and limit the editing to only some?
// There are singular phonemes and compound phonemes, perhaps they could be separate from eachother
typedef struct VoiceDef
{
    PhonDef Phoneme[NUM_PHONEMES];    // One phoneme for each letter in the ARPAbet
    char name[SPEAKER_MAX];           // Speaker's name
    char filename[PATH_MAX];          // Filename
} VoiceDef;

#endif//__VOICE_TYPES_H__
