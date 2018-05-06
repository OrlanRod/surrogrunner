#include "hazards.h"
#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "player.h"
#include "soundfx.h"

extern Arduboy2 arduboy;
extern ArduboyTones sound;
extern Player *players[];
extern const unsigned char assets[];
extern unsigned char continues;
extern unsigned char gameState;

class Mine : public Hazards{

public:
  unsigned char mProxy = 0;
  unsigned char frameIndex = 0;
  unsigned char frameTimer = 10;
  bool explode = false;


  Mine(){

  }
  ~Mine(){

  }

  char getDistance(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2){
    return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
  }

  void hazardAI(void){

    if (active == true){

      if (hAI == 1){//Idle, no proxy.


        if (getDistance(hX, players[0]->plX, hY, players[0]->plY) < 8 ){

            //sound.tone(40, 300);
            if (explode == false){
              setSfxInfo(EXPLODE, 600, 10);
              //sound.tone(60, 800, 40, 5000);

              explode = true;
            }
            frameIndex = 0;
            frameTimer = 10;
            players[0]->isHit = true;
            players[0]->plAction = 2;
            active = false;
            if (continues > 0){
              continues -= 1;
              gameState = 3;
            }

        }
      }

    }
  }

  void hazardDraw(unsigned char x, unsigned char y){


    if (explode == true){

      if (frameTimer > 0){
        frameTimer--;
      }else{
        if (frameIndex < 6) frameIndex++;
        frameTimer = 5;
      }

      arduboy.drawBitmap(x, y, &assets[576+(8*frameIndex)], 8, 8, WHITE);


    }

    if(explode == false){

      if (frameTimer > 0){
        frameTimer--;
      }else{
        frameIndex++;
        frameTimer = 10;
      }

      if (frameIndex == 2) frameIndex = 0;
      arduboy.drawBitmap(x, y, &assets[192+(8*frameIndex)], 8, 8, WHITE);

    }

  }

};
