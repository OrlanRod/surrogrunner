#include <ArduboyTones.h>
#include "enums.h"
extern ArduboyTones sound;

#ifndef SOUNDFX_H
#define SOUNDFX_H

void setSfxInfo(SfxNames type, unsigned int freq, unsigned int stime);
void setMSCInfo(SfxNames type);
void sfxPlay();

#endif
