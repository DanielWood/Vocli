#include <stdio.h>
#include <string.h>
#include "vocli.h"
#include "voice_edit.h"
#include "voice_cmds.h"
#include "cmds.h"

// Command table
// Should at least remove command name from description
const Cmd Commands[] = {
    {"ve",      &cmd_ve,    "ve [voice]\t--\tLaunch the voice editor"},
    {"help",    &cmd_help,  "help [command]\t--\tDisplay this help text or show info on a specific command"},
    {"quit",    &cmd_quit,  "quit\t--\tQuit Vocli"}
};

// Command count
const int num_cmds = sizeof(Commands) / sizeof(Commands[0]);

/* Voice editor command */
int cmd_ve(char *input)
{
    // Help text
    if (input == NULL)
    {
        printf("ve [voice]\n"
                "\t\tLaunches the voice editor, taking a voice name as an optional argument.\n"
                "\t\tIf [voice] has been specified, the corresponding voice is selected for editing.\n"
                "\t\tIf [voice] does not correspond to an existing voice, a new one is created by that name.\n"
                "\t\tIf no voice is specified, a new voice is created by the name \"New Voice\"\n");
        return FAIL;
    }

    strtok(input, " ");

    char *name = strtok(NULL, " ");

    VoiceDef voice;
    
    if (read_voice(name, &voice) == FAIL)
        init_voice(&voice, name);

    while (0)
    { 
        
    }

    return SUCCESS;
}

int cmd_help(char *input)
{

}


int cmd_quit(char *input)
{

}
