#include "hazards.h"
#include "player.h"
#include "soundfx.h"

extern Arduboy2 arduboy;
extern Player player;
extern Hazards hazardsList[];
extern unsigned char continues;
extern unsigned char gameState;
extern unsigned char invincible;

Hazards::Hazards(){

}

Hazards::~Hazards(){

}

char Hazards::getDistance(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2){
return sqrt(pow(x1-x2,2) + pow(y1-y2,2));
}

void Hazards::hazardAI(void){

  if (hType == 1){//MINE

    if (active == true){


      if (hAI == 1){//moving h or v.

        static unsigned char aiDir = random(1);

        if (arduboy.everyXFrames(stepTimer)){

          if (dirCycle == false){//h
            if (aiDir == 0) {
              if (hX < 120){
                hX+= 8;
              }else {
                aiDir = 1;
              }
            }else {
              if (hX > 0){
                hX-= 8;
              }else {
                aiDir = 0;
              }
            }
          }else{//v
            if (aiDir == 0) {
              if (hY < 56){
                hY+= 8;
              }else {
                aiDir = 1;
              }
            }else {
              if (hY > 8){
                hY-= 8;
              }else {
                aiDir = 0;
              }
            }
          }

        }

      }


      if (hAI == 2){//move towards player.

        if (arduboy.everyXFrames(stepTimer)){

          if (player.plX > hX && hX < 120) hX+= 8;
          if (player.plX < hX && hX > 0) hX-= 8;
          if (player.plY < hY && hY > 8) hY-= 8;
          if (player.plY > hY && hY < 56) hY+= 8;

        }

      }

    }


  }else if (hType == 2){//Turret

    if (hAI == 1){ // fire bullets in 4 diag directions.

      if(fire == false && projIndex == 0 && arduboy.everyXFrames(120)){

          fire = true;
          fireIndex = 0;
          projIndex = 0;

      }

    }


    if (hAI == 2){ // fire bullets in 4 diag or straight directions.

      if(fire == false && projIndex == 0 && arduboy.everyXFrames(120)){

          fire = true;
          fireIndex = 0;
          projIndex = 0;

          if (fireIndex >= 4) fireStyle++;
          if (fireStyle > 2) fireStyle = 1;
      }

    }


  } else if (hType == 3 && active == true){//Pit

    if (hX == player.plX && hY == player.plY){

      if (hAI == 1){ //cut player speed ro stop if at base speed.

        if (pitTrigger == false){

          if (player.resetRunTimer < 10) {

            player.resetRunTimer += 2;
            player.runTimer = player.resetRunTimer;

          } else if (player.resetRunTimer >=10) player.plSP = 0;

          pitTrigger = true;
        }

      }

      if (hAI == 2){ //teleport player to random x/y spot.

        player.plX = random() % (15 + 0) * 8;
        player.plY = random() % (7 + 1) * 8;


      }


    }else {
      pitTrigger = false;
    }


  } else if (hType == 4){//Bullet


      if ((hX < 8 || hX >= 128 || hY < 8 || hY >= 64) && active == true){

        for (unsigned char i = 0; i < 6; i++){
          if (hID == hazardsList[i].hID && hazardsList[i].hType == 2){
            hazardsList[i].fireIndex--;
            hazardsList[i].projIndex--;
            active = false;
          }
        }

      }

      if (stepTimer < projMoveTimerSet){

        stepTimer++;

      }else{

        if (hDir == 0) {

          hX += cellSteps;

        }else if (hDir == 1){

          hX += cellSteps;
          hY += cellSteps;

        }else if (hDir == 2){

          hY += cellSteps;

        }else if(hDir == 3){

          hX -= cellSteps;
          hY += cellSteps;

        }else if(hDir == 4){

          hX -= cellSteps;

        }else if(hDir == 5){

          hX -= cellSteps;
          hY -= cellSteps;

        }else if(hDir == 6){

          hY -= cellSteps;

        } else if (hDir == 7){

          hX += cellSteps;
          hY -= cellSteps;

        }
        stepTimer = 0;
      }

  }

}
