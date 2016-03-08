/* Do not include this file directly. 
 * Include vocli.h instead.
 */
#ifndef VOCLI__SOUND_H__
#define VOCLI__SOUND_H__

#include <csound.h>
#include "arpabet.h"

extern int orc_init(CSOUND *csound, const char *filename);
extern int phoneme_score_event(CSOUND *csound, const ARPAsym *sym, int tie);

#endif//VOCLI__SOUND_H__
