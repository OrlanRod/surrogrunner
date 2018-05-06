#include <Arduino.h>
#include <Arduboy2.h>
#include <ArduboyTones.h>
#include <stdio.h>
#include "enums.h"
//#include <MemoryFree.h>
#include "player.h"
#include "flag.h"
#include "hazards.h"
#include "assets.h"
#include "draw.h"
#include "soundfx.h"
#include <time.h>

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
extern Player player;
extern Flag flag;
extern Hazards hazardsList[];
extern Hazards projectileList[];
//Changes 0.6:
//Optimizations.
//Removed pointers from flag, player, hazards, and projectile objects.
//Change string memobjmanager and the soundfx functions to enum type.

bool invincible = false;
unsigned char gameState = 0; //1 = game start. 2 = stage clear. 3 = lost a continue,respawn. 4 = game over. 5 = game victory.
unsigned char menuIndex = 0; //Used for menus
unsigned char minutes = 1;
unsigned char seconds = 60;
char buffer[3];
unsigned int plScore = 0; //current player's score.
unsigned char continues = 2; //Player's continues until game over.
unsigned char stage = 1; //current game stage. max 99.
unsigned char screen = 1; //1 = title, 2 = options, 3 = game.
unsigned char flagGoal = 0; //The amount of flags to get before a stage clear.
unsigned char flagsPerStage = 1; //The amount of flags to spawn per stage.
unsigned char hazardsPerStage;
unsigned char hazardsPerScreen;
unsigned char numOfFlagsSpawned = 0; //Keeps track of the amount of flags spawned.
unsigned char numOfHazardsSpawned = 0; //Keeps track of the amount of hazards spawned.
unsigned char flagCount = 0; //the current count of the flags acquired.
unsigned char gameHazards = 2; //the max amount of hazards to spawn per screen.
unsigned char numOfScreensH = 2; //The sizes of the game world.
unsigned char numOfScreensV = 2;
unsigned char roomSwitchX = 8; //Used for room lock switch.
unsigned char roomSwitchY = 8;
unsigned char flagSpawnID  = 1;
unsigned char hazardSpawnID  = 1;
bool roomLock = false; //To check/set when player warps for spawning hazards and picups.
bool spawnedFlag = false;
bool spawnedHazard = false;
bool spawnedSwitch = false;
bool spawnedExit = false;
bool resetBG = false; //Used to reset the bg screen tiles back to default.
bool startSet = false;
unsigned char flandHazSet = 0;


unsigned char screenMap[8][16] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},

};

unsigned char roomMap[10][10] = { // 0 = empty. 1 = flag-with hazard, 2 = no flag-hazard, 3 = bonus, 4 = exit, 5 = start area.
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0},

};


void resetVars(void);
void resetSwitch(void);
void hazardAIs(void);
void spawn(void);
void genMap(void);
void resetMaps(String type);
void memManageOBJ(Names obj, Names act){

if (act == DEACTIVATE){//deactivate

    if (obj == FLAG){ //flag

      if (flag.active == true){

        numOfFlagsSpawned--;
        flag.active = false;

      }

      if (roomLock == false) spawnedFlag = false;

    }else if (obj == HAZARDS){ //Hazards
      for(unsigned char ii=0; ii < 6; ii++){

        if (hazardsList[ii].active == true) {
          numOfHazardsSpawned--;
          hazardsList[ii].active = false;
        }

      }
      //spawnedHazard = false;
    }else if (obj == BULLETS){ //bullets

      for(unsigned char ii=0; ii < 24; ii++){

        if (projectileList[ii].active == true) projectileList[ii].active = false;

      }

    }

  }

  if (act == RESET){//reset
    if (obj == HAZARDS){
      //reset all active turrets.
      for (unsigned char iii = 0; iii < 6; iii++){
        if (hazardsList[iii].hType == 2){
          hazardsList[iii].fireIndex = 0;
          hazardsList[iii].projIndex = 0;
        }
      }
    }
  }


}

void resetVars(void){

  gameState = 0; //1 = game start. 2 = stage clear. 3 = lost a continue,respawn. 4 = game over. 5 = game victory.
  menuIndex = 0; //Used for menus
  minutes = 1;
  seconds = 60;
  plScore = 0; //current player's score.
  stage = 1; //current game stage. max 99.
  screen = 1; //1 = title, 2 = options, 3 = game.
  flagGoal = 0; //The amount of flags to get before a stage clear.
  flagsPerStage = 1; //The amount of flags to spawn per stage.
  hazardsPerStage = 0;
  hazardsPerScreen = 0;
  numOfFlagsSpawned = 0; //Keeps track of the amount of flags spawned.
  numOfHazardsSpawned = 0; //Keeps track of the amount of hazards spawned.
  flagCount = 0; //the current count of the flags acquired.
  gameHazards = 2; //the max amount of hazards to spawn per screen.
  numOfScreensH = 2; //The sizes of the game world.
  numOfScreensV = 2;
  roomSwitchX = 8; //Used for room lock switch.
  roomSwitchY = 8;
  roomLock = false; //To check/set when player warps for spawning hazards and picups.
  spawnedFlag = false;
  spawnedHazard = false;
  spawnedSwitch = false;
  spawnedExit = false;
  resetBG = false; //Used to reset the bg screen tiles back to default.
  startSet = false;
  flandHazSet = 0;
  flagSpawnID  = 1;
  hazardSpawnID  = 1;

}

void hazardAIs(void){

  for (unsigned i = 0; i < 6; i++){

      hazardsList[i].hazardAI();

  }

  for (unsigned i = 0; i < 24; i++){

      projectileList[i].hazardAI();

  }

}

void resetMaps(String type){

  if (type == "SCREEN"){

    //memset(screenMap, 960, sizeof(screenMap));

    for(unsigned char i = 0; i < 16; i++){
      screenMap[1][i] = 0;
      screenMap[2][i] = 0;
      screenMap[3][i] = 0;
      screenMap[4][i] = 0;
      screenMap[5][i] = 0;
      screenMap[6][i] = 0;
      screenMap[7][i] = 0;
    }

  }

  if (type == "ROOM"){

    memset(roomMap, 0, sizeof(roomMap));

  }

}

void resetSwitch(void){

  resetMaps("SCREEN");
  if (roomMap[player.plPosOnMapV][player.plPosOnMapH] != 5 && roomMap[player.plPosOnMapV][player.plPosOnMapH] != 4){

    while (spawnedSwitch == false){

      if (player.plDir == 0){

        roomSwitchX = 15;
        roomSwitchY = random()% 7 + 1;

      }else if (player.plDir == 2){

        roomSwitchX = random()% 16 + 0;
        roomSwitchY = 7;

      }else if (player.plDir == 4){

        roomSwitchX = 0;
        roomSwitchY = random()% 7 + 1;

      }else {

        roomSwitchX = random()% 16 + 0;
        roomSwitchY = 1;

      }

      if (screenMap[roomSwitchY][roomSwitchX] != 40 && screenMap[roomSwitchY][roomSwitchX] != 60){
        screenMap[roomSwitchY][roomSwitchX] = 60;
        spawnedSwitch = true;
      }


    }

  }


}

void genMap(void){

  unsigned char genX;
  unsigned char genY;
  static unsigned char flagGVal;


  if (stage < 10){
    numOfScreensH = 2;
    numOfScreensV = 2;
    hazardsPerStage = 3;
    if(stage == 1 || stage == 2) hazardsPerScreen = 1;
    if(stage == 3 || stage == 4) hazardsPerScreen = 2;
    if(stage == 5 || stage == 6) hazardsPerScreen = 3;
    if(stage >= 7) hazardsPerScreen = 4;
    if(stage > 8)
    minutes = 1;
    seconds = 60;
    flagGoal = 1;
  } else if (stage >= 10 && stage < 20){
    hazardsPerScreen = 5;
    numOfScreensH = 5;
    numOfScreensV = 5;
    hazardsPerStage = 8;
    hazardsPerScreen = 4;
    minutes = 3;
    seconds = 60;
    flagGoal = 2;
  } else if (stage >= 20 && stage < 30){
    numOfScreensH = 6;
    numOfScreensV = 6;
    hazardsPerStage = 16;
    minutes = 6;
    seconds = 60;
    flagGoal = 3;
  } else if (stage >= 30 && stage < 40){
    numOfScreensH = 7;
    numOfScreensV = 7;
    hazardsPerStage = 24;
    minutes = 9;
    seconds = 60;
    flagGoal = 4;
  }else if (stage >= 40 && stage < 50){
    numOfScreensH = 8;
    numOfScreensV = 8;
    hazardsPerStage = 32;
    minutes = 12;
    seconds = 60;
    flagGoal = 5;
  }else if (stage >= 50 && stage < 60){
    numOfScreensH = 9;
    numOfScreensV = 9;
    hazardsPerStage = 74;
    hazardsPerScreen = 4;
    minutes = 15;
    seconds = 60;
    flagGoal = 6;
  }else if (stage >= 60){
    numOfScreensH = 10;
    numOfScreensV = 10;
    hazardsPerStage = 48;
    hazardsPerScreen = 4;
    minutes = 18;
    seconds = 60;
    flagGoal = 7;
  }

 // 0 = empty. 1 = flag-no hazard, 2 = flag-with hazard, 3 = no flag-with hazar. 4 = exit, 5 = start area.
 while (flandHazSet < hazardsPerStage){

   if (startSet == false){

     genX = random(numOfScreensH);
     genY = random(numOfScreensV);

     if (roomMap[genY][genX] == 0){

      roomMap[genY][genX] = 5;
      player.plPosOnMapH = genX;
      player.plPosOnMapV = genY;
      startSet = true;

      }

   }else{

     //Spawn flags with hazards or just hazards.

       for (unsigned char i = 0; i < numOfScreensV+1; i++){
         for(unsigned char ii = 0; ii < numOfScreensH+1; ii++){
           if (roomMap[i][ii] == 0){

               if (flagGVal < flagGoal) roomMap[i][ii] = 1;
               if (flagGVal >= flagGoal && roomMap[i][ii] == 0) roomMap[i][ii] = 2;
               flagGVal++;
               flandHazSet++;

             }
         }
       }
   }

 }
flagGVal = 0;

}

void spawn(void){

  static unsigned char flagsSpwnd;
  static unsigned char hazardsSpwnd;
  static unsigned char hazardType = 1; //1 = mine, 2 = turret, 3 = pit.
  unsigned char spawnX;
  unsigned char spawnY;

  if (roomLock == true && (roomMap[player.plPosOnMapV][player.plPosOnMapH] == 1)){


    if (spawnedFlag == false){ //spawn flags.

            spawnX = random() % 14 + 2;
            spawnY = random() % 5 + 2;

            if (screenMap[spawnY][spawnX] != 30){ //30 = flag.

              flag.active = true;
              flag.iX = spawnX*8;
              flag.iY = spawnY*8;
              flag.iId = flagSpawnID;
              flagsSpwnd++;
              flagSpawnID++;
              screenMap[spawnY][spawnX] = 30;
              numOfFlagsSpawned++;

            }

      }

      if (flagsSpwnd == 1) {
        spawnedFlag = true;
        flagsSpwnd = 0;
      }
  }


  if ((roomMap[player.plPosOnMapV][player.plPosOnMapH] == 1 || roomMap[player.plPosOnMapV][player.plPosOnMapH] == 2) && continues > 0 && roomLock == true){ //spawn hazards.

    if (spawnedHazard == false){

      for (unsigned char i = 0; i < 6; i++){
        spawnX = random() % 13 + 2;
        spawnY = random() % 5 + 2;
        unsigned char ran = random(10);
        if (screenMap[spawnY][spawnX] != 40 && screenMap[spawnY][spawnX] != 60 && screenMap[spawnY][spawnX] != 30 && hazardsSpwnd < hazardsPerScreen && roomLock == true){ //40 = hazard, 60 = switch.

                srand(time(NULL));
               if (stage < 10) hazardType = 1;
               if (stage >= 10 && stage < 20) hazardType = random() % 2 + 1;
               if (stage >= 20) hazardType = random() % 3 + 1;
               hazardsList[hazardsSpwnd].active = true;
               if (hazardType == 1) {
                 hazardsList[i].hType = 1;
                 if (ran <= 5 ) {
                   hazardsList[i].dirCycle = false;
                 }else{
                   hazardsList[i].dirCycle = true;
                 }

               }else if (hazardType == 2) {

                  hazardsList[i].hType = 2;
                  hazardsList[i].fire = false;
                  hazardsList[i].projIndex = 0;
                  hazardsList[i].fireIndex = 0;

               }else if (hazardType == 3) {
                 hazardsList[i].hType = 3;

               }

               hazardsList[i].hX = spawnX*8;
               hazardsList[i].hY = spawnY*8;
               hazardsList[i].hID = hazardSpawnID;
               if (stage < 10) hazardsList[i].stepTimer = 20; //30
               if (stage >= 10 && stage < 20) hazardsList[i].stepTimer = 10;
               if (stage >= 20) hazardsList[i].stepTimer = 5;
               if (stage < 10) hazardsList[i].hAI = 1;
               if (stage >= 10) hazardsList[i].hAI = random()% 2+1;
               hazardsSpwnd++;
               hazardSpawnID++;
               screenMap[spawnY][spawnX] = 40;
               numOfHazardsSpawned++;

        }
      }

    }

    if (hazardsSpwnd == hazardsPerScreen){

      spawnedHazard = true;
      resetSwitch();
      hazardsSpwnd = 0;

    }

    for(unsigned char i = 0; i < 6; i++){ //spawn projectile.

      if (hazardsList[i].active == true && hazardsList[i].hType == 2 && hazardsList[i].fire == true && (hazardsList[i].hAI == 1 || hazardsList[i].hAI == 2)){

          for (unsigned char ii = 0; ii < 24; ii++){

            if (hazardsList[i].fireIndex < 4 && projectileList[ii].active == false){

                  projectileList[ii].hType = 4;
                  projectileList[ii].hX = hazardsList[i].hX;
                  projectileList[ii].hY = hazardsList[i].hY;
                  projectileList[ii].cellSteps = hazardsList[i].cellSteps;
                  projectileList[ii].projMoveTimerSet = hazardsList[i].stepTimer;
                  if (hazardsList[i].fireStyle == 1) projectileList[ii].hDir = 1+(2*hazardsList[i].fireIndex);
                  if (hazardsList[i].fireStyle == 2) projectileList[ii].hDir = 0+(2*hazardsList[i].fireIndex);
                  projectileList[ii].hID = hazardsList[i].hID;
                  projectileList[ii].active = true;
                  hazardsList[i].fireIndex++;
                  hazardsList[i].projIndex++;

            }

          }

          if (hazardsList[i].hAI == 2){
            if (hazardsList[i].fireIndex >= 4) hazardsList[i].fireStyle++;
            if (hazardsList[i].fireStyle > 2) hazardsList[i].fireStyle = 1;
          }

          hazardsList[i].fire = false;

      }


    }

      }

}


void setup() {
    // put your setup code here, to run once:
    srand(time(NULL));
    if (EEPROM.read(0) != 131){ //check if the game has written to it before.
      EEPROM.write(1, 2); //set lives to default.
      EEPROM.write(2, 1); //turn on sound effects.
      EEPROM.write(3, 1); //turn on music.
      EEPROM.write(0, 131);
    }else{
      continues = EEPROM.read(1);
    }


    //arduboy.begin();
    arduboy.boot(); // raw hardware
    arduboy.display(); // blank the display (sBuffer is global, so cleared automatically)
    arduboy.flashlight(); // light the RGB LED and screen if UP button is being held.
    // check for and handle buttons held during start up for system control
    arduboy.systemButtons();
    arduboy.audio.begin();
    //bootLogo();
    arduboy.waitNoButtons(); // wait for all buttons to be released
    sound.volumeMode(VOLUME_ALWAYS_HIGH);
    arduboy.setFrameRate(30);
    //Serial.begin(9600);

}

void loop() {
    // put your main code here, to run repeatedly:
    if (!(arduboy.nextFrame()))
    return;

    arduboy.clear();

    //Serial.write(arduboy.getBuffer(), 128 * 64 / 8);

    sfxPlay();

    if (gameState != 2 && gameState != 4  && gameState != 5 && screen == 3){

      spawn();

      //Edit screen map, as the player runs over a tile and add to score.
      if (player.plAction == 1){

        if (screenMap[(player.plY)/8][(player.plX)/8] != 8 && (player.collision(player.plX, player.plY, roomSwitchX*8, roomSwitchY*8) != 1)){

              screenMap[(player.plY)/8][(player.plX)/8] = 8;
              plScore+= 20-(player.resetRunTimer);
              setSfxInfo(LIGHT, 800, 10);

        }

      }

        if (player.isHit == false){

          if (arduboy.everyXFrames(30)){

            if (minutes > 0){

              if (seconds > 0){
                seconds --;
              }else{
                minutes--;
                seconds = 60;
              }
            }else{

              if (seconds > 0){
                seconds --;
              }else{
                if (gameState != 2){
                  if (minutes == 0 && seconds == 0 && gameState != 4) gameState = 4;
                }
              }

            }
          }
        }


        for (unsigned char i = 0; i < 2; i++){

          if (flag.active == true && gameState !=2 && gameState !=4){

              //check if player collided with flag, if so, remove flag from memory and flagmap, then add to flag goal.

                if (player.collision(player.plX, player.plY, flag.iX, flag.iY) == 1){

                  flagCount++;
                  //if (flag.active == true) flag.active = false;
                  memManageOBJ(FLAG,DEACTIVATE);
                  if (player.resetRunTimer > 2) player.resetRunTimer-= 2;
                  //if (player.runTimer > 2) player.runTimer-= 2;

                  if (EEPROM.read(2) == 1) setSfxInfo(PICKUP, 900, 100);

                }
                //if all flags are gone from the screen, reset room back to hazard room only.
                if ((roomMap[player.plPosOnMapV][player.plPosOnMapH] == 1 || roomMap[player.plPosOnMapV][player.plPosOnMapH] == 2) && numOfFlagsSpawned == 0){

                  spawnedFlag = false;
                  roomMap[player.plPosOnMapV][player.plPosOnMapH] = 2;

                }

            }

        }

        //check if run into bullet, if so trigger hit. Remove bullets.
        for (unsigned char i = 0; i < 24; i++){

          if (projectileList[i].active == true && invincible == false){

            if (player.collision(player.plX, player.plY, projectileList[i].hX, projectileList[i].hY) == 1 && player.isHit == false){

              if (player.resetRunTimer < 10) {
                player.runTimer += 2;
                player.resetRunTimer += 2;
              }
              player.isHit = true;
              player.plAction = 2;
              player.plFrameInd = 0;
              setSfxInfo(EXPLODE, 600, 10);
              if (continues > 0){
                continues -= 1;
                gameState = 3;
              }

              memManageOBJ(BULLETS, DEACTIVATE);

              for (unsigned ii = 0; ii < 6; ii++){
                if (hazardsList[ii].hType == 2) {
                  hazardsList[ii].fire = false;
                  hazardsList[ii].fireIndex = 0;
                  hazardsList[ii].projIndex = 0;
                }
              }

            }

          }

        }


        //check if run into mine or turret, if so trigger hit. Remove turret.
        for (unsigned char i = 0; i < 6; i++){

          if (hazardsList[i].active == true && invincible == false){

            if (player.collision(player.plX, player.plY, hazardsList[i].hX, hazardsList[i].hY) == 1 && hazardsList[i].active == true && hazardsList[i].hType == 1){


              if (player.resetRunTimer < 10) {
                player.runTimer += 2;
                player.resetRunTimer += 2;
              }
              player.isHit = true;
              player.plAction = 2;

              player.plFrameInd = 0;
              setSfxInfo(EXPLODE, 600, 10);
              if (continues > 0){
                continues -= 1;
                gameState = 3;
              }
              hazardsList[i].active = false;


            }else if (player.collision(player.plX, player.plY, hazardsList[i].hX, hazardsList[i].hY) == 1 && hazardsList[i].active == true && hazardsList[i].hType == 2){


              if (player.resetRunTimer < 10) {
                player.runTimer += 2;
                player.resetRunTimer += 2;
              }
              player.isHit = true;
              player.plAction = 2;

              player.plFrameInd = 0;
              setSfxInfo(EXPLODE, 600, 10);
              if (continues > 0){
                continues -= 1;
                gameState = 3;
              }

              memManageOBJ(HAZARDS,RESET);//reset turrets.
              memManageOBJ(BULLETS,DEACTIVATE);//remove bullets.

              hazardsList[i].active = false;

            }

          }

        }

        //If player runs into roomswitch, then turn off room lock, and remove any hazards onscreen.
        if (player.plX == roomSwitchX*8 && player.plY == roomSwitchY*8 && roomLock == true){

          memManageOBJ(HAZARDS,DEACTIVATE);//remove hazards
          memManageOBJ(BULLETS,DEACTIVATE);//remove bullets

          flagSpawnID = 1;
          hazardSpawnID = 1;
          if (numOfHazardsSpawned <= 0) numOfHazardsSpawned = 0;
          if (EEPROM.read(2) == 1) setSfxInfo(SWITCH, 700, 20);
          roomLock = false;

        }

        //create exit after reaching flag goal, then stage clear when player is at exit.
        if (flagCount >= flagGoal && gameState != 2){

          while(spawnedExit == false){

            for (unsigned char i = 0; i < numOfScreensV; i++){
              for (unsigned char ii = 0; ii < numOfScreensH; ii++){
                if (roomMap[i][ii] == 5){

                  roomMap[i][ii] = 4;
                  spawnedExit = true;

                }
              }
            }

          }

          if (roomMap[player.plPosOnMapV][player.plPosOnMapH] == 4) arduboy.drawBitmap(64, 24, &bgtiles[16], 8, 8, WHITE);

          if (player.plX == 64 && player.plY == 24 && roomMap[player.plPosOnMapV][player.plPosOnMapH] == 4){

              if (stage < 59) {

                setMSCInfo(STAGETUNE);
                gameState = 2;

              }else{

                gameState = 5;

              }

          }

        }

    }

    //game over if no more continues. reinit game and switch screens.
    if (continues <= 0 && gameState != 4 && screen == 3) {

      if (arduboy.everyXFrames(200))  gameState = 4;

    }


    //Stage cleared. Increase stage var, reset the rest of the vars and remove objects.
    if (gameState == 2 && gameState != 4 && stage < 60 && screen == 3){

      if (arduboy.everyXFrames(120)){

        resetMaps("ROOM");
        resetMaps("SCREEN");
        memManageOBJ(FLAG,DEACTIVATE);
        memManageOBJ(HAZARDS,DEACTIVATE);
        memManageOBJ(BULLETS,DEACTIVATE);
        spawnedFlag = false;

        stage+= 1;
        resetBG = true;
        flagCount = 0;
        spawnedFlag = false;
        spawnedHazard = false;
        spawnedSwitch = false;
        numOfFlagsSpawned = 0;
        numOfHazardsSpawned = 0;
        spawnedExit = false;
        player.plPosOnMapH = 0;
        player.plPosOnMapV = 0;
        player.plAction = 0;
        roomLock = false;
        startSet = false;
        flandHazSet = 0;
        genMap();
        gameState = 1;

      }
    }

    //titlescreen and options
    if (screen == 1) {
      arduboy.pollButtons();
      if (arduboy.justPressed(RIGHT_BUTTON)){
        if (menuIndex < 1) menuIndex++;
        if (EEPROM.read(2) == 1) sound.tone(200, 200);
      }
      if (arduboy.justPressed(LEFT_BUTTON)) {
        if (menuIndex > 0) menuIndex--;
        if (EEPROM.read(2) == 1) sound.tone(200, 200);
      }
      if (arduboy.justReleased(A_BUTTON)){

        if (menuIndex == 0){

          resetVars();

          memManageOBJ(FLAG,DEACTIVATE);
          memManageOBJ(HAZARDS,DEACTIVATE);
          memManageOBJ(BULLETS,DEACTIVATE);
          resetMaps("ROOM");
          resetMaps("SCREEN");

          stage = 1;
          player.active = true;
          genMap();
          screen = 3;
          gameState = 1;

        }

        if (menuIndex == 1){

          menuIndex = 0;
          screen = 2;

        }
      }


    }

    //options
    if (screen == 2){

      arduboy.pollButtons();
      if (arduboy.justPressed(UP_BUTTON)){
        if (menuIndex > 0) menuIndex--;
        if (EEPROM.read(2) == 1) sound.tone(200, 200);
      }
      if (arduboy.justPressed(DOWN_BUTTON)) {
        if (menuIndex < 3) menuIndex++;
        if (EEPROM.read(2) == 1) sound.tone(200, 200);
      }
      if (arduboy.justPressed(RIGHT_BUTTON)) {
        if (menuIndex == 0 && continues < 6) {
          continues++;
          EEPROM.write(1,continues);
        }
        if (menuIndex == 1 && EEPROM.read(2) != 1) {

          EEPROM.write(2,1);
        }
        if (menuIndex == 2 && EEPROM.read(3) != 1) {

          EEPROM.write(3,1);
        }
      }

      if (arduboy.justPressed(LEFT_BUTTON)) {
        if (menuIndex == 0 && continues > 1) {
          continues--;
          EEPROM.write(1,continues);
        }
        if (menuIndex == 1 && EEPROM.read(2) != 2) {

          EEPROM.write(2,2);
        }
        if (menuIndex == 2 && EEPROM.read(3) != 2) {

          EEPROM.write(3,2);
        }

      }

      if (arduboy.justReleased(A_BUTTON)) {

        if (menuIndex == 3){
          screen = 1;
          menuIndex = 0;
        }

      }

    }

    //reset player if hit.
    if (gameState == 3  && screen == 3 && gameState != 4 && continues > 0){
      static unsigned char resetTimer = 60;

      if (resetTimer > 0){
        resetTimer--;
      }else{

        for (unsigned char i = 0; i < 128; i++){
          unsigned char resetX = random()%15+1;
          unsigned char resetY = random()%7+1;

          if (screenMap[resetY][resetX] == 0){
            player.plX = 16;
            player.plY = 8;
            player.plAction = 0;
            player.plSP = 0;
            player.isHit = false;
            player.plFrameInd = 0;
            break;
          }

        }
        gameState = 1;
        resetTimer = 60;
      }

    }


    if (gameState == 4 || gameState == 5) {

      //reset vars then do a soft reset of arduboy.
      arduboy.pollButtons();
      if (arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON)){

        resetVars();
        asm volatile ("  jmp 0");

      }

    }

    if (screen == 3 && gameState != 4 && gameState != 5) {

        if (continues > 0 && gameState != 2) drawBG();
        drawObjects();
        drawPlayer();
        if (gameState != 2 && player.isHit == false) player.plControls();
        if (player.delay == false) hazardAIs();

    }

    if (screen == 1 || screen == 2 || screen == 3) drawHud();
    arduboy.display();
}
