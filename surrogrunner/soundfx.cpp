#include "soundfx.h"
#include <Arduboy2.h>
#include "assets.h"
#include "enums.h"

unsigned char sfxType = 0;
unsigned int sfxFreq = 0;
unsigned int sfxTime = 0;
unsigned int sTime = 0;
bool runOnce = false;


extern Arduboy2 arduboy;

void setMSCInfo(SfxNames type){
  sfxType = type;
}
void setSfxInfo(SfxNames type, unsigned int freq, unsigned int stime){
  sfxType = type;
  sfxFreq = freq;
  sfxTime = stime;
}

void sfxPlay(){

  if (sfxType != 0){//None
    if (runOnce == false){
        if (sfxType == 1) sound.tone(sfxFreq, sfxTime, 50, 400); //explode
        if (sfxType == 2) sound.tone(sfxFreq, sfxTime); //Switch
        if (sfxType == 3) sound.tone(sfxFreq, sfxTime); //Pickup
        if (sfxType == 4) sound.tone(sfxFreq, sfxTime); //Light
        if (sfxType == 5) sound.tones(stcleartrk); //Stagetune

        runOnce = true;
    }
  }

  //effect properties.
  //fall
  if (sfxType == 1) if (sfxFreq > 20) sfxFreq-= 20;

    if (sound.playing() == false && sfxType != 0){

      sound.noTone();
      sfxFreq = 0;
      runOnce = false;
      sfxType = 0;

    }

}
