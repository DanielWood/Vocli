#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "vocli.h"
#include "voice_edit.h"
#include "voice_cmds.h"
#include "cmds.h"
#include "utils.h"

// Command table
// Should at least remove command name from description
const Cmd Commands[] = {
    {"ve",      &cmd_ve,    "ve [voice]\t--\tLaunch the voice editor"},
    {"help",    &cmd_help,  "help [command]\t--\tDisplay this help text or show info on a specific command"},
};

// Command count
const int num_cmds = sizeof(Commands) / sizeof(Commands[0]);

/* Voice editor command */
int cmd_ve(char *input)
{
    // Help text
    if (input == NULL)
    {
        fprintf(stderr,
                "ve [voice]\n"
                "\t\tLaunches the voice editor, taking a voice name as an optional argument.\n"
                "\t\tIf [voice] has been specified, the corresponding voice is selected for editing.\n"
                "\t\tIf [voice] does not correspond to an existing voice, a new one is created by that name.\n"
                "\t\tIf no voice is specified, a new voice is created by the name \"New Voice\"\n");
        return FAIL;
    }

    strtok(input, " ");
    char *name = strtok(NULL, " ");

    // Banner
    putchar('\n');
    printf("------------------\n");
    printf("-- Voice editor --\n");
    printf("------------------\n\n");

    VoiceDef voice = {};
    if (name == NULL || read_voice(name, &voice) == FAIL)
        init_voice(&voice, name);

    // Input loop 
    bool running = true;
    char ve_input[512];
    while (running)
    { 
        printf("%s> ", voice.name);
        fgets(ve_input, 512, stdin);

        strchomp(ve_input);

        if (strncmp(ve_input, "quit", strlen("quit")) == 0)
            running = false;
        else
            for (int i = 0; i < num_voice_cmds; i++)
            {
                const char *name = VoiceCommands[i].name;
                if (strncmp(ve_input, name, strlen(name)) == 0)
                    VoiceCommands[i].callback(ve_input, &voice);
            }
    }

    return SUCCESS;
}

int cmd_help(char *input)
{

}
