/*****************************************
* Vocli: Csound-based speech synthesizer
* Written by Daniel Wood (2016)
*****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <csound.h>

#include "vocli.h"
#include "mystring.h"
#include "voice_types.h"
#include "arpabet.h"
#include "voice_edit.h"
#include "cmu.h"
#include "sound.h"

// Orchestra file path
const char *orc_path = "res/vocli.orc";

int main (int argc, char *argv[])
{
    // Program banner
    printf(
    "----------------------------------\n"
    "---  Vocli Speech Synthesizer  ---\n"
    "----------------------------------\n"
    "- Version: v%s%*c-\n"
    "- Email: d.ryan.wood@gmail.com   -\n"
    "----------------------------------\n\n",
    vocli_version,
    abs(21 - strlen(vocli_version)), ' ');

    // Initialize resources
    printf("Initializing CMU dictionary...\n");
    CMUDict dictionary;
    cmu_init("res/cmudict-0.7b", &dictionary);

    printf("Creating Csound instance...\n");
    CSOUND *csound = csoundCreate(NULL);
    csoundSetOption(csound, "-odac");    // Real time output
    if (csound == NULL)
    {
        fprintf(stderr, "FATAL: Unable to create Csound\n");
        cmu_destroy(&dictionary);
        return FAIL;
    }

    printf("Initializing Vocli orchestra...\n");
    int res = orc_init(csound, orc_path);
    if (res != SUCCESS)
    {
        fprintf(stderr, "FATAL: Unable to initialize Vocli orchestra\n");
        csoundDestroy(csound);
        cmu_destroy(&dictionary);
        return FAIL;
    }

    // Special character whitelist
    char ok[] = "'_-";
    size_t oklen = strlen(ok);

    // Interactive console
    char input[512];
    printf("\nType '!' to exit\n\n");
    while (1)
    {
        // Prompt string
        printf("Enter some text> ");

        // Read text from input
        fgets(input, 512, stdin);

        // Handle quit command
        if (input[0] == '!')
        {
            putchar('\n');
            break;
        }

        // Reformat string to match the dictionary
        for (int i = 0, len = strlen(input); i < len; i++)
        {
            if (isalnum(input[i]))
                input[i] = toupper(input[i]);
            else
                for (int j = 0; j < oklen; j++)
                    if (input[i] == ok[j])
                        break;
                    else if (j == oklen - 1)
                        input[i] = ' ';
        }

        // Look up and print each word
        int i;
        char *word = strtok(input, " ");
        for (i = 0; word != NULL; i++)
        {
            CMUDef *def = cmu_find_word(word, &dictionary);
            if (def != NULL)
            {
                for (int j = 0; j < def->num_phonemes; j++)
                {
                    // Pass score event
                    char start = i ? '0' : '+';

                    const ARPAsym *sym = &ARPAbet[def->phonIDs[j]];
                    printf("%s", sym->name);

                    // Vowel stress
                    if (sym->type == PHON_MONO || sym->type == PHON_DIPTHO)
                        putchar(def->stress[j] + '0');

                    putchar(' ');
                }
            }

            // Separate each word
            if ((word = strtok(NULL, " ")) != NULL)
            printf(" - ");
        }

        putchar('\n');
    }

    // Clean up
    csoundDestroy(csound);       // Destroy Csound
    cmu_destroy(&dictionary);    // Destroy CMU dictionary

    return SUCCESS;
}
