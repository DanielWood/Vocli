#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "vocli.h"
#include "arpabet.h"
#include "voice_edit.h"

/* Initialize an empty voicedef */
void new_voice(VoiceDef *voice, const char *name)
{
    // Set voice name
    if (name[0] != '\0')
        strncpy(voice->name, name, strlen(name));
    else
    {
        // Default name if an empty string is entered
        char *new_name = "New Voice";
        strncpy(voice->name, new_name, strlen(new_name));
    }

    // Empty filename string
    voice->filename[0] = '\0';

    // Set up phoneme table
    for (int i = 0; i < NUM_PHONEMES; i++)
    {
        // Initialize all values to 0
        memset(&voice->Phoneme[i], 0, sizeof(PhonDef));

        // Assign ID to phoneme
        voice->Phoneme[i].ID = (uint8_t)i;
        voice->Phoneme[i].ARPAsym = ARPAbet[i].name;    // Is this still stupid?
    }
}

/* Read a voicedef from disk */
int read_voice(const char *filename, VoiceDef *voice)
{
    // Open target file
    FILE *fh = fopen(filename, "r");
    if (fh == NULL)
    {
        fprintf(stderr, "Unable to open file '%s' for reading\n");
        return FAIL;
    }

    // Read file header
    uint8_t header[5] = "\0\0\0\0";
    if (fread(header, 1, 4, fh) != 4)
        goto read_fail;

    if (strcmp(header, "VDEF") != 0)
    {
        fprintf(stderr, "Invalid header ('%s') in file '%s'\n", header, filename);
        return FAIL;
    }

    // Get phoneme count
    uint8_t phoncount = 0;
    if (fread(&phoncount, 1, 1, fh) != 1)
        goto read_fail;

    // TODO: Selectively read matching phonemes? (Allows version compatibility and smaller? filesize)
    if (phoncount != NUM_PHONEMES)
    {
        fprintf(stderr, "Invalid phoneme count %" PRIu8 " (Expected: %d) in file '%s'\n", phoncount, NUM_PHONEMES, filename);
        fprintf(stderr, "Version mismatch?\n");
        fclose(fh);
        return FAIL;
    }

    // Clear target voicedef
    new_voice(voice, "");

    // Save filename to voicedef
    strncpy(voice->filename, filename, PATH_MAX);

    // Read speaker name
    char name[SPEAKER_MAX];
    int i;
    for (i = 0; (name[i] = fgetc(fh)) != '\0'; i++)
    {
        if (i >= SPEAKER_MAX)
        {
            fprintf(stderr, "Speaker name exceeds maximum length: %d (Max: %d)\n", i + 1, SPEAKER_MAX);
            goto read_fail;
        }
    }

    // Verify and save speaker name
    if (i > 0)
        strncpy(voice->name, name, i + 1);
    else
    {
        fprintf(stderr, "Unable to read speaker name\n");
        goto read_fail;
    }

    // Read data header
    uint8_t datahead[5] = "\0\0\0\0";
    if (fread(datahead, 1, 4, fh) != 4)
        goto read_fail;

    if (strcmp(datahead, "DATA") != 0)
    {
        fprintf(stderr, "Invalid data header ('%s') in file '%s'\n", datahead, filename);
        goto read_fail;
    }

    // Read phoneme data
    for (int i = 0; i < NUM_PHONEMES; i++)
    {
        PhonDef *phoneme = &voice->Phoneme[i];

        // Read ID
        if (fread(&phoneme->ID, 1, 1, fh) != 1)
            goto read_fail;

        // Check ID
        if (phoneme->ID != i)
        {
            fprintf(stderr, "Invalid phoneme ID: %" PRIu8 " (Expected: %d)\n", phoneme->ID, i);
            goto read_fail;
        }

        // Read formants
        for (int j = 0; j < NUM_FORMANTS; j++)
        {
            FmtDef *formant = &phoneme->Formant[j];

            if (fread(&formant->fqc, 4, 1, fh) != 1)    goto read_fail;
            if (fread(&formant->bw, 4, 1, fh) != 1)     goto read_fail;
            if (fread(&formant->dBoffs, 4, 1, fh) != 1) goto read_fail;
        }

        // This is pointless, in my opinion
        phoneme->ARPAsym = ARPAbet[phoneme->ID].name;
    }

    // Close file
    fclose(fh);

    // Verify voicedef
    if (vrfy_voice(voice) != SUCCESS)
    {
        //new_voice(voice, "");
        fprintf(stderr, "Failed to verify VoiceDef after successful read!\n");
        fprintf(stderr, "This shouldn't happen!\n");
        fprintf(stderr, "Can you email me with a log of this screen? (d.ryan.wood@gmail.com)\n");
        fprintf(stderr, "Thanks\n");
        return FAIL;
    }

    printf("Read voice '%s' from file '%s'\n", voice->name, voice->filename);

    return SUCCESS;

    // In case the read fails
read_fail:
    new_voice(voice, "");
    fprintf(stderr, "Error reading VoiceDef from file '%s'\n", voice->filename);
    fclose(fh);
    return FAIL;
}

/* Write a voicedef to disk */
int write_voice(const char *filename, VoiceDef *voice)
{
    // Verify voicedef
    if (vrfy_voice(voice) != SUCCESS)
    {
        fprintf(stderr, "Unable to verify VoiceDef before writing\n");
        return FAIL;
    }

    // Open target file
    FILE *fh = fopen(filename, "w");
    if (fh == NULL)
    {
        fprintf(stderr, "Unable to open file '%s' for writing\n", filename);
        return FAIL;
    }

    // Save filename in voice
    strncpy(voice->filename, filename, PATH_MAX);

    // Write file header
    if (fwrite("VDEF", 1, 4, fh) != 4)
        goto write_fail;

    // Phoneme count
    if (fputc((uint8_t)NUM_PHONEMES, fh) == EOF)
        goto write_fail;

    // Speaker name
    size_t name_len = strlen(voice->name);
    if (fwrite(voice->name, 1, name_len + 1, fh) != name_len + 1)
        goto write_fail;

    // Data header
    if (fwrite("DATA", 1, 4, fh) != 4)
        goto write_fail;

    // Write phonemes
    for (int i = 0; i < NUM_PHONEMES; i++)
    {
        // ID
        if (fwrite(&voice->Phoneme[i].ID, 1, 1, fh) != 1)
            goto write_fail;

        // Formants
        for (int j = 0; j < NUM_FORMANTS; j++)
        {
            FmtDef *formant = &voice->Phoneme[i].Formant[j];

            if (fwrite(&formant->fqc, 4, 1, fh) != 1)    goto write_fail;
            if (fwrite(&formant->bw, 4, 1, fh) != 1)     goto write_fail;
            if (fwrite(&formant->dBoffs, 4, 1, fh) != 1) goto write_fail;
        }
    }

    // Close file
    fclose(fh);

    printf("Voice '%s' saved to file '%s'\n", voice->name, filename);

    return SUCCESS;

    // In case the write fails
write_fail:
    fprintf(stderr, "Error writing VoiceDef '%s' to file '%s'\n", voice->name, filename);
    fclose(fh);
    return FAIL;
}

/* Verify a voicedef */
int vrfy_voice(const VoiceDef *voice)
{
    // Pointer checkpp
    if (!voice || !voice->name || !*voice->name)
        return FAIL;

    // Name check
    size_t name_len = strlen(voice->name);
    if (name_len >= SPEAKER_MAX)
    {
        fprintf(stderr, "Speaker name exceeds maxmimum length: %d (Max: %d)\n", name_len, SPEAKER_MAX);
        return FAIL;
    }

    // Check the ID of each phoneme
    for (int i = 0; i < NUM_PHONEMES; i++)
    {
        if (voice->Phoneme[i].ID != i)
            return FAIL;
    }

    return SUCCESS;
}


/* Print a phondef */
// TODO: Make this take a pointer?
void print_phoneme(const PhonDef phoneme)
{
    printf("\n--------------------------------\n");

    printf("ID: %" PRIu8 "\n", phoneme.ID);
    printf("Name: %s\n", ARPAbet[phoneme.ID].name);
    printf("Type: %s", Phontype[ARPAbet[phoneme.ID].type]);

    for (int i = 0; i < NUM_FORMANTS; i++)
    {
        printf("\n\nFormant #%d\n", i + 1);
        printf("Frequency: %" PRIu32 "Hz\n", phoneme.Formant[i].fqc);
        printf("Bandwidth: %" PRIu32 "Hz\n", phoneme.Formant[i].bw);
        printf("Amp Offset: %" PRId32 "dB", phoneme.Formant[i].dBoffs);
    }

    printf("\n--------------------------------\n\n");
}

/* Find phoneme ID with ARPAbet symbol */
unsigned char get_phonID(const char *ARPAsym)
{
    for (int i = 0; i < NUM_PHONEMES; i++)
    {
        if (strcmp(ARPAsym, ARPAbet[i].name) == 0)
            return i;
    }

    return 255;
}
