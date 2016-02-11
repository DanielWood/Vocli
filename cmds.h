#ifndef __CMDS_H__
#define __CMDS_H__

// Vocli command hash
typedef struct Cmd
{
    char *name;
    int (*callback)(char *);
    char *description;
} Cmd;

// Command prototypes 
extern int cmd_ve(char *input);
extern int cmd_help(char *input);
extern int cmd_quit(char *input);

extern const Cmd Commands[];
extern const int num_cmds; 
#endif//__CMDS_H__
