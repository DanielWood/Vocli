/* Do not include this file directly. 
 * Include vocli.h instead.
 */
#ifndef __VOICE_EDIT_H__
#define __VOICE_EDIT_H__

#include "voice_types.h"

extern void new_voice(VoiceDef *voice, const char *name); // Rename to init_voice
extern int read_voice(const char *filename, VoiceDef *voice);
extern int write_voice(const char *filename, VoiceDef *voice);
extern int vrfy_voice(const VoiceDef *voice); // Rename to verify_voice

extern void print_phoneme(const PhonDef phoneme);
extern unsigned char get_phonID(const char *ARPAsym); // Rename to get_phoneme_ID

#endif//__VOICE_EDIT_H__
