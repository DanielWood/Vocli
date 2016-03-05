#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cmu.h"
#include "vocli.h"
#include "voice_edit.h"
#include "mystring.h"

/* Initialize CMU dictionary */
int cmu_init(const char *filename, CMUDict *dictionary)
{
    FILE *fh = fopen(filename, "r");
    if (fh == NULL)
    {
        fprintf(stderr, "Unable to open file '%s' for reading\n", filename);
        return FAIL;
    }

    char linebuf[CMU_LINE_MAX];

    // Count lines
    int lines = 0;
    while(fgets(linebuf, CMU_LINE_MAX, fh) != NULL)
    {
        // Skip comments, special characters and [multiple-definitions]
        if (isalnum(linebuf[0]) && /*||*/ strchr(linebuf, '(') == NULL)
            lines++;
    }
    fseek(fh, 0, SEEK_SET);

    // Allocate definitions array
    dictionary->def = malloc((lines + 1) * sizeof(CMUDef));
    dictionary->size = lines;

    // Read dictionary
    int i = 0;
    for (; fgets(linebuf, CMU_LINE_MAX, fh) != NULL && i < lines; /*i++*/)
    {
        // Skip comments and special characters
        if (!isalnum(linebuf[0]))
            continue;

        // Ignore multiple definitions (for now)
        if (strstr(linebuf, "(") != NULL)
            continue;

        // Separate word from definition
        char *word = strtok(linebuf, "  ");
        if (word == NULL)
            continue;
        else
            strncpy(dictionary->def[i].word, word, CMU_WORD_MAX);

        // Collect phonemes
        char *ARPAsym = strtok(NULL, " ");
        int j = 0;
        for (; ARPAsym != NULL; ARPAsym = strtok(NULL, " "), j++)
        {
            strchomp(ARPAsym);

            // Extract vowel stress
            dictionary->def[i].stress[j] = 0;
            for (int k = strlen(ARPAsym); k > 0; k--)
            {
                if (isdigit(ARPAsym[k]))
                {
                    dictionary->def[i].stress[j] = ARPAsym[k] - '0';
                    ARPAsym[k] = '\0';
                    break;
                }
            }

            // Get phoneme ID from symbol
            uint8_t phonID = get_phonID(ARPAsym);
            if (phonID >= NUM_PHONEMES)
            {
                fprintf(stderr, "Discovered invalid phoneme '%s' in definition of '%s'\n",
                        ARPAsym, dictionary->def[i].word);
                fclose(fh);
                free(dictionary->def);
                return FAIL;
            }

            dictionary->def[i].phonIDs[j] = phonID;
        }

        // Record phoneme count
        dictionary->def[i].num_phonemes = j;

        // Linked list stuff
        dictionary->def[i].next = &dictionary->def[i + 1];
        i++;
    }

    dictionary->def[i].next = NULL;

    return SUCCESS;
}

/* Destroy CMU dictionary */
void cmu_destroy(CMUDict *dictionary)
{
    free(dictionary->def);
}

/* Look up a word */
CMUDef *cmu_find_word(const char *word, const CMUDict *dictionary)
{
    struct CMUDef *p;
    for (p = dictionary->def; p->next != NULL; p = p->next)
    {
        if (strcmp(word, p->word) == 0)
            return p;
    }

    return NULL;
}

