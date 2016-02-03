// Voc TTS interface
#include "include/vocsin.h"
#include "/usr/local/include/csound/csound.h"


// Orchestra file
const char *orc_path = "vocsin.orc";

int main (int argc, char *argv[])
{
  // Program banner
  printf("----------------\n"
	 "-- VocSin TTS --\n"
	 "----------------\n\n");
  
  printf("Initializing CMU dictionary...\n");
  CMUDict dictionary;
  int res = cmu_init("cmudict-0.7b", &dictionary);

  if (res != SUCCESS)
    {
      fprintf(stderr, "Failed to initialize CMU dictionary!\n");
      return res;
    }

  printf("Initializing Csound...\n\n");
  CSOUND *csound = csoundCreate(NULL);
  
  csoundSetOption(csound, "-odac");    // Real time output
  
  // Open orchestra file
  FILE *fh = fopen(orc_path, "r");
  if (fh == NULL)
    {
      fprintf(stderr, "Unable to open orchestra file '%s'\n", orc_path);
      goto error;
    }

  // Get file size
  fseek(fh, 0, SEEK_END);
  size_t orc_size = ftell(fh);

  // Read orchestra
  char *orchestra = malloc(orc_size * sizeof(char));
  fseek(fh, 0, SEEK_SET);
  size_t n_read = fread(orchestra, 1, orc_size, fh);
  fclose(fh);

  if (n_read != orc_size)
    {
      fprintf(stderr, "Invalid number of bytes read from orchestra '%s': %d -- (Expected: %d)\n", orc_path, n_read, orc_size);
      goto error;
    }

  // Compile orchestra
  int result = csoundCompileOrc(csound, orchestra);
  if (result != 0)
    {
      fprintf(stderr, "Error compiling csound orchestra '%s'\n", orc_path);
      goto error;
    }

  // Deallocate old orchestra buffer
  free(orchestra);

  
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
	break;

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
	  CMUDef *def = cmu_findword(word, &dictionary);
	  if (def != NULL)
	    {
	      for (int j = 0; j < def->num_phonemes; j++)
		{
		  // Pass score event
		  char start = i ? '0' : '+';

		  
		  const ARPAsym *sym = &ARPAbet[def->phonIDs[j]];
		  printf("%s", sym->name);

		  // Vowel stress
		  if (sym->type == PHON_VOWL)
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

 error:
  csoundDestroy(csound);       // Destroy Csound
  cmu_destroy(&dictionary);    // Destroy CMU dictionary
  return FAIL;
}

int make_score_event(const PhonDef *phoneme, const uint8_t stress)
{
  if (sym == NULL || voice == NULL)
    {
      return FAIL;
    }

  char eventbuf[128];
  //;1 start dur   db	pitch	a/d	port	vibatk	vibhz	vibstr	mouth	fqc1	bw1	dBoffs1	fqc2	bw2	dBoffs2	fqc3	bw3	dBoffs3	special
  snprintf(eventbuf, 128, "i1 %c %f 6.04 .05 %f %f %f 4 %f %d %d %d %d %d %d %d %d %d 1\n",
	   
}
