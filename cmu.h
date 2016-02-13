#ifndef __CMU_H__
#define __CMU_H__

#include "arpabet.h"

// Limit of letters/phonemes per word
#define CMU_WORD_MAX 32
#define CMU_LINE_MAX 128

// A CMU word definition
// TODO: Make CMUDefs into a linked list
typedef struct CMUDef
{
    char word[CMU_WORD_MAX];       // Word
    char phonIDs[CMU_WORD_MAX];    // Pronunciation
    char stress[CMU_WORD_MAX];     // Vowel stress
    int num_phonemes;           // Phoneme count
    //CMUDef *next;
} CMUDef;

// The CMU dictionary
typedef struct CMUDict
{
    CMUDef *def;    // Definition array (I dont like this name) -- Dict is nicer
    int size;    // Dictionary size / word count
    //int num_words; <-- Better!
} CMUDict;

/* -- Forward declarations -- */
extern int cmu_init(const char *filename, CMUDict *dictionary);
extern void cmu_destroy(CMUDict *dictionary);
extern CMUDef *cmu_find_word(const char *word, const CMUDict *dictionary); // Todo: this should return int

#endif//__CMU_H__
