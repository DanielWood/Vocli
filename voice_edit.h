/* Do not include this file directly. 
 * Include vocli.h instead.
 */
#ifndef __VOICE_EDIT_H__
#define __VOICE_EDIT_H__

#include "voice_types.h"

extern void init_voice(VoiceDef *voice, const char *name);
extern int read_voice(const char *filename, VoiceDef *voice);
extern int write_voice(const char *filename, VoiceDef *const voice);
extern int verify_voice(const VoiceDef *const voice);

extern void print_phoneme(const PhonDef phoneme);
extern unsigned char get_phoneme_ID(const char *const ARPAsym);

#endif//__VOICE_EDIT_H__
