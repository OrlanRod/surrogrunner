#include <ArduboyTones.h>
extern ArduboyTones sound;

#ifndef SOUNDFX_H
#define SOUNDFX_H

void setSfxInfo(String type, unsigned int freq, unsigned int stime);
void setMSCInfo(String type);
void sfxPlay();

#endif
