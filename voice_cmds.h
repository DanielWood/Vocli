/* Do not include this file directly. 
 * Include vocli.h instead.
 */
#ifndef __VOICE_CMDS_H__
#define __VOICE_CMDS_H__

#include "voice_types.h"

// Voice command hash
typedef struct VoiceCmd
{
    const char *name;
    int (*callback)(char *, VoiceDef *);
} VoiceCmd;

// Command prototypes
extern int ve_cmd_print(char *input, VoiceDef *voice);
extern int ve_cmd_edit(char *input, VoiceDef *voice);
extern int ve_cmd_name(char *input, VoiceDef *voice);
extern int ve_cmd_save(char *input, VoiceDef *voice);
extern int ve_cmd_help(char *input, VoiceDef *voice);
extern int ve_cmd_quit(char *input, VoiceDef *voice);

// TODO!
// cmd_load -- load a voice from disk

extern const VoiceCmd VoiceCommands[];
extern const int num_voice_cmds;

#endif//__VOICE_CMDS_H__
