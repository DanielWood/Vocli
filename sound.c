#include <stdio.h>
#include <stdlib.h>

#include "vocli.h"
#include "sound.h"

/* Load and initialize Csound orchestra */
int orc_init(CSOUND *csound, const char *filename)
{
    if (csound == NULL || filename == NULL)
        return FAIL;

    // Open orchestra file
    FILE *fh = fopen(filename, "r");
    if (fh == NULL)
    {
        fprintf(stderr, "Unable to open orchestra file '%s'\n", filename);
        return FAIL;
    }

    // Allocate orchestra buffer
    fseek(fh, 0, SEEK_END);
    size_t orc_size = ftell(fh);
    char *orchestra = malloc(orc_size * sizeof(char));
    fseek(fh, 0, SEEK_SET);

    // Read orchestra
    size_t n_read = fread(orchestra, 1, orc_size, fh);
    fclose(fh);
    if (n_read != orc_size)
    {
        fprintf(stderr, "Invalid number of bytes read from orchestra '%s': %d "
                "-- (Expected: %d)\n", filename, n_read, orc_size);
        return FAIL;
    }

    // Compile orchestra
    int result = csoundCompileOrc(csound, orchestra);
    if (result != 0)
    {
        fprintf(stderr, "Unable to compile csound orchestra '%s'\n", filename);
        return FAIL;
    }

    // Deallocate old orchestra buffer
    free(orchestra);

    return SUCCESS;
}

int phon_score_event(CSOUND *csound, const ARPAsym *sym, int tied)
{
    float dur = Phontime[sym->type];
    float atkdec = .01;
    float port = dur / 10;
    float mouth = sym->type >= PHON_STOP;
    
//start dur db	pitch	a/d	port	vibatk	vibhz	vibstr	mouth	fqc1	bw1	dBoffs1	fqc2	bw2	dBoffs2	fqc3	bw3	dBoffs3	special
    return 0;
}
