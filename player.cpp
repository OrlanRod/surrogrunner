#include <Arduboy2.h>
#include <player.h>
#include <garbage.h>

//Player *players[2] = {0,0};
Player *player;

extern const unsigned char assets[];

extern unsigned char numOfScreensH; //The sizes of the game world.
extern unsigned char numOfScreensV;
extern bool roomLock;
extern unsigned char roomMap[10][10];
extern unsigned int screenMap[8][16];
extern bool resetBG;
extern unsigned char roomSwitchX; //Used for unlocking the room.
extern unsigned char roomSwitchY;
extern unsigned char numOfFlagsSpawned;
extern unsigned char numOfHazardsSpawned;
extern void removeObjects(String type);
extern bool spawnedSwitch;
extern void resetSwitch(void);
extern bool spawnedHazard;
extern Arduboy2 arduboy;

//#define ARDBITMAP_SBUF arduboy.getBuffer()
//#include <ArdBitmap.h>

//ArdBitmap<WIDTH,HEIGHT> ardbitmap;


Player::Player(){

}

Player::~Player(){

}

bool Player::collision(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2){

    //return (x1+2 <= x2+4 && x1+2 >= x2-4 && y1+2 <= y2+4 && y1+2 >= y2-4);
    return (x1+2 >= x2 && x1+5 <= x2+7 && y1+4 >= y2 && y1+5 <= y2+7);

}


void Player::plControls(){

                if (plX == 120 && arduboy.pressed(RIGHT_BUTTON) && plPosOnMapH < numOfScreensH && roomLock == false){

                    if (numOfFlagsSpawned > 0) memManageOBJ("FLAG","REMOVEALL");
                    memManageOBJ("HAZARDS","REMOVEALL");
                    memManageOBJ("BULLETS","REMOVEALL");
                    numOfHazardsSpawned = 0;
                    plX = 0;
                    delay = true;
                    plPosOnMapH++;
                    spawnedSwitch = false;
                    resetSwitch();
                    spawnedHazard = false;
                    if (roomMap[plPosOnMapV][plPosOnMapH] != 5 && roomMap[plPosOnMapV][plPosOnMapH] != 4) roomLock = true;

                } else if (plX == 0 && arduboy.pressed(LEFT_BUTTON) && plPosOnMapH > 0 && roomLock == false){

                    if (numOfFlagsSpawned > 0) memManageOBJ("FLAG","REMOVEALL");
                    memManageOBJ("HAZARDS","REMOVEALL");
                    memManageOBJ("BULLETS","REMOVEALL");
                    numOfHazardsSpawned = 0;
                    plX = 120;
                    delay = true;
                    plPosOnMapH--;
                    spawnedSwitch = false;
                    resetSwitch();
                    spawnedHazard = false;
                    if (roomMap[plPosOnMapV][plPosOnMapH] != 5 && roomMap[plPosOnMapV][plPosOnMapH] != 4) roomLock = true;

                } else if (plY == 56 && arduboy.pressed(DOWN_BUTTON) && plPosOnMapV < numOfScreensV && roomLock == false) {

                    if (numOfFlagsSpawned > 0) memManageOBJ("FLAG","REMOVEALL");
                    memManageOBJ("HAZARDS","REMOVEALL");
                    memManageOBJ("BULLETS","REMOVEALL");
                    numOfHazardsSpawned = 0;
                    plY = 8;
                    delay = true;
                    plPosOnMapV++;
                    spawnedSwitch = false;
                    resetSwitch();
                    spawnedHazard = false;
                    if (roomMap[plPosOnMapV][plPosOnMapH] != 5 && roomMap[plPosOnMapV][plPosOnMapH] != 4) roomLock = true;

                } else if (plY == 8 && arduboy.pressed(UP_BUTTON) && plPosOnMapV > 0 && roomLock == false) {

                    if (numOfFlagsSpawned > 0) memManageOBJ("FLAG","REMOVEALL");
                    memManageOBJ("HAZARDS","REMOVEALL");
                    memManageOBJ("BULLETS","REMOVEALL");
                    numOfHazardsSpawned = 0;
                    plY = 56;
                    delay = true;
                    plPosOnMapV--;
                    spawnedSwitch = false;
                    resetSwitch();
                    spawnedHazard = false;
                    if (roomMap[plPosOnMapV][plPosOnMapH] != 5 && roomMap[plPosOnMapV][plPosOnMapH] != 4) roomLock = true;

                }

                if (delay == true && arduboy.everyXFrames(35)) delay = false;



        if (isHit == false){

             if (plSP > 0){
                    plAction = 1;
                    if (resetRunTimer == 10)  plframeExpo = 10;
                    if (resetRunTimer == 8) plframeExpo = 7;
                    if (resetRunTimer == 6) plframeExpo = 6;
                    if (resetRunTimer == 4) plframeExpo = 5;
                    if (resetRunTimer == 2) plframeExpo = 4;
            }else{

                    plframeExpo = 20;
                    plAction = 0;
            }

            arduboy.pollButtons();


            if (arduboy.justReleased(A_BUTTON)){

                if (plSP != 8)  plSP = 8;
                if (runTimer > resetRunTimer)  runTimer -= 2;

            }else if (arduboy.justReleased(B_BUTTON)){

                if (runTimer < 10) {
                  runTimer += 2;
                }else{
                  plSP = 0;
                  plAction = 0;
                }

                plFrameInd = 0;
                plTimer = 0;


            }

            if (arduboy.justReleased(LEFT_BUTTON) || arduboy.justReleased(RIGHT_BUTTON) || arduboy.justReleased(UP_BUTTON) || arduboy.justReleased(DOWN_BUTTON)){
                plTimer = 0;
                plFrameInd = 0;
            }

            //8-dir controls.
            if ((arduboy.pressed(LEFT_BUTTON) && arduboy.notPressed(RIGHT_BUTTON) && arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(DOWN_BUTTON)) && (isHit == false)){

                plDir = 4;

            } else if (arduboy.pressed(RIGHT_BUTTON) && arduboy.notPressed(LEFT_BUTTON) && arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(DOWN_BUTTON) && (isHit == false)){

                plDir = 0;

            } else if (arduboy.pressed(UP_BUTTON) && arduboy.notPressed(RIGHT_BUTTON) && arduboy.notPressed(LEFT_BUTTON) && arduboy.notPressed(DOWN_BUTTON) && (isHit == false)){

                plDir = 6;

            } else if (arduboy.pressed(DOWN_BUTTON) && arduboy.notPressed(RIGHT_BUTTON) && arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(UP_BUTTON) && (isHit == false)){

                plDir = 2;

            }

                 if (arduboy.everyXFrames(runTimer)){

                   if (plDir == 0){

                      if (roomLock == false && delay == false ){

                        if (plX < 120) plX+= plSP;


                      }

                      if (roomLock == true && delay == false){

                          if (plX < 120) plX+= plSP;


                      }

                      if (plX >= 127 && plPosOnMapH == numOfScreensH){

                        plX-= plSP;

                      }


                  }

                  if (plDir == 2){

                      if (roomLock == false && delay == false) {

                        if (plY < 56) plY+= plSP;


                      }

                      if (roomLock == true && delay == false){

                        if (plY < 56) plY+= plSP;


                      }

                      if (plY >= 64 && plPosOnMapV == numOfScreensV){

                        plY-= plSP;

                      }


                  }

                  if (plDir == 4){

                      if (roomLock == false && delay == false) {

                          if (plX > 0) plX-= plSP;


                      }
                      if (roomLock == true && delay == false){

                          if (plX > 0) plX-= plSP;


                      }

                      if (plX < 0 && plPosOnMapH == 0){

                        plX+= plSP;

                      }


                  }

                  if (plDir == 6){

                      if (roomLock == false && delay == false){

                          if (plY > 8) plY-= plSP;


                      }
                      if(roomLock == true && delay == false) {

                          if (plY > 8) plY-= plSP;


                      }

                      if (plY <= 1 && plPosOnMapV == 0){

                        plY+= plSP;

                      }

                  }

                 }


        }


}
