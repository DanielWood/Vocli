#include <stdio.h>
#include "vocli.h"
#include "voice_cmds.h"
#include "cmds.h"

// Command table
const Cmd Commands[] = {
    {"ve", &cmd_ve,     "ve [voice]\t--\tLaunch the voice editor"},
    {"help", &cmd_help, "help\t--\tDisplay this help text\nhelp [command]\t--\tShow help on a specific command"},
    {"quit", &cmd_quit, "quit\t--\tQuit Vocli"}
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
                "\t\tIf executed without an argument, a new voice is created by the name \"New Voice\"\n");
        return FAIL;
    }

    return SUCCESS;
}

int cmd_help(char *input)
{

}


int cmd_quit(char *input)
{

}
