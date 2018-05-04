#include <soundfx.h>
#include <string.h>
#include <Arduboy2.h>
#include <assets.h>

String sfxType = String("NONE");
unsigned int sfxFreq = 0;
unsigned int sfxTime = 0;
unsigned int sTime = 0;
bool runOnce = false;


extern Arduboy2 arduboy;

void setMSCInfo(String type){
  sfxType = type;
}
void setSfxInfo(String type, unsigned int freq, unsigned int stime){
  sfxType = type;
  sfxFreq = freq;
  sfxTime = stime;
}

void sfxPlay(){

  if (sfxType != "NONE"){
    if (runOnce == false){
        if (sfxType == "EXPLODE") sound.tone(sfxFreq, sfxTime, 50, 400);
        if (sfxType == "SWITCH") sound.tone(sfxFreq, sfxTime);
        if (sfxType == "FLAG") sound.tone(sfxFreq, sfxTime);
        if (sfxType == "LIGHT") sound.tone(sfxFreq, sfxTime);
        if (sfxType == "STAGETUNE") sound.tones(stcleartrk);

        runOnce = true;
    }
  }

  //effect properties.
  //fall
  if (sfxType == "EXPLODE") if (sfxFreq > 20) sfxFreq-= 20;

    if (sound.playing() == false && sfxType != "NONE"){

      sound.noTone();
      sfxFreq = 0;
      runOnce = false;
      sfxType = "NONE";

    }

}
