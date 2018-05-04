#include <draw.h>
#include <hazards.h>
#include <flag.h>
#include <Arduboy2.h>
#include <player.h>
#include <assets.h>


extern Arduboy2 arduboy;
extern unsigned char gameState;
extern bool roomLock;
extern unsigned char numOfScreensH;
extern unsigned char numOfScreensV;
extern unsigned char roomMap[10][10];
extern unsigned char screenMap[8][16];
extern unsigned char roomSwitchX;
extern unsigned char roomSwitchY;
extern unsigned char menuIndex;
extern unsigned char flagGoal;
extern unsigned char flagCount;
extern unsigned char minutes;
extern unsigned char seconds;
extern unsigned char stage;
extern unsigned int plScore;
extern char buffer[];
extern unsigned char screen;

extern Player *player;

Flag *flag;
Hazards *hazardsList[6] = {0,0,0,0,0,0};
Hazards *projectileList[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void hideOption(unsigned char x,unsigned char y, unsigned char w);

void hideOption(unsigned char x,unsigned char y, unsigned char w){
static bool flip = false;

  for (unsigned char i = 0; i < w; i++){
    if (flip == false){
      arduboy.drawBitmap(x+(i*8),y, &scanline[8], 8, 8,  BLACK);
    }else{
      arduboy.drawBitmap(x+(i*8),y, &scanline[0], 8, 8,  BLACK);
    }

  }

  if (arduboy.everyXFrames(2)) flip = !flip;
}

void drawHud(void){

  if (screen == 1){

    arduboy.drawBitmap(16, 16, &tlogo[0], 96, 32,  WHITE);
    hideOption(16,32,128);
    hideOption(16,40,128);
    arduboy.setCursor(24, 56);
    arduboy.print("PLAY  OPTIONS");

    if (menuIndex == 0) {

      hideOption(56,56, 6);

    }else{

      hideOption(24,56, 3);


    }

  }

  if (screen == 2){

    arduboy.setCursor(16, 8);
    arduboy.print("LIVES:");
    arduboy.setCursor(64, 8);
    arduboy.print(EEPROM.read(1));
    arduboy.setCursor(16, 24);
    arduboy.print("SOUND:");
    arduboy.setCursor(64, 24);
    if (EEPROM.read(2) == 1){
        arduboy.print("ON");
    }
    if (EEPROM.read(2) == 2){
        arduboy.print("OFF");
    }
    arduboy.setCursor(16, 40);
    arduboy.print("MUSIC:");

    arduboy.setCursor(64, 40);
    if (EEPROM.read(3) == 1){
        arduboy.print("ON");
    }
    if (EEPROM.read(3) == 2){
        arduboy.print("OFF");
    }

    arduboy.setCursor(64, 56);
    arduboy.print("EXIT");

    if (menuIndex == 0) {

      hideOption(16,24, 10); //sound
      hideOption(16,40, 10); //music
      hideOption(64,56, 10); //exit

    }else if (menuIndex == 1){

      hideOption(16,8, 10); //lives
      hideOption(16,40, 10); //music
      hideOption(64,56, 10); //exit


    }else if (menuIndex == 2){

      hideOption(16,8, 10); //lives
      hideOption(16,24, 10); //sound
      hideOption(64,56, 10);//exit


    }else{

      hideOption(16,8, 10); //lives
      hideOption(16,24, 10); //sound
      hideOption(16,40, 10); //music

    }

  }

  if (screen == 3){

    //draw game hud
    if (gameState != 2 && gameState != 4 && gameState != 5){


      arduboy.drawBitmap(0, 0, &ighud[0], 8, 8, WHITE);//flag icon
      arduboy.drawBitmap(96, 0, &ighud[8], 8, 8, WHITE);//room icon

      arduboy.setCursor(8, 0); //flag goal.
      arduboy.print(flagCount);
      arduboy.setCursor(16, 0);
      arduboy.print("/");
      arduboy.setCursor(24, 0);
      arduboy.print(flagGoal);

       //countdown
      arduboy.setCursor(40, 0);
      if (minutes < 10) sprintf(buffer, "0%d", minutes);
      if (minutes >= 10) sprintf(buffer, "%d", minutes);
      arduboy.print(buffer);
      arduboy.setCursor(56, 0);
      arduboy.print(":");
      arduboy.setCursor(64, 0);
      if (seconds >=10){
        arduboy.print(seconds);
      }else{
        sprintf(buffer, "0%d", seconds);
        arduboy.print(buffer);

      }
      //arduboy.setCursor(64, 0); //memory free
      //arduboy.print(freeMemory());

      arduboy.setCursor(104, 0); //room area.
      arduboy.print(player->plPosOnMapH);
      arduboy.setCursor(112, 0);
      arduboy.print("-");
      arduboy.setCursor(120, 0);
      arduboy.print(player->plPosOnMapV);

      hideOption(0,0,128);
    }

    if (gameState == 2  && screen == 3 && gameState != 4 && gameState != 5){

      sprintf(buffer, "0%d", stage);
      arduboy.setCursor(16, 32); //room area.
      arduboy.print("STAGE ");
      arduboy.print(buffer);
      arduboy.print(" CLEARED");

    }





    if ((gameState == 4 || gameState == 5) && screen == 3){

      if (plScore >= 1000000) sprintf(buffer, "%d", plScore);
      if (plScore < 1000000 && plScore >= 100000) sprintf(buffer, "0%d", plScore);
      if (plScore < 100000 && plScore >= 10000) sprintf(buffer, "00%d", plScore);
      if (plScore < 10000 && plScore >= 1000) sprintf(buffer, "000%d", plScore);
      if (plScore < 1000 && plScore >= 100) sprintf(buffer, "0000%d", plScore);
      if (plScore < 100 && plScore >= 10) sprintf(buffer, "00000%d", plScore);
      if (plScore < 10) sprintf(buffer, "000000%d", plScore);

      if (gameState == 4) {
        arduboy.setCursor(36, 16);
        arduboy.print("GAME OVER");
        arduboy.setCursor(12, 56);
        arduboy.print("PRESS A+B TO EXIT");
        arduboy.setCursor(40, 35);
        arduboy.print(buffer);
      }else if (gameState == 5) {
        arduboy.drawBitmap(72, 0, &victorlogo[0], 48, 48, WHITE);
        arduboy.setCursor(8, 8);
        arduboy.print("VICTORY!");
        arduboy.setCursor(12, 56);
        arduboy.print("PRESS A+B TO EXIT");
        arduboy.setCursor(8, 32);
        arduboy.print(buffer);
      }


    }


  }

}

void drawPlayer(void){
  if (player->plAction == 0 || player->plAction == 1 || player->plAction == 2){

        if (player->plTimer < player->plframeExpo){
              player->plTimer++;
         }else{
             if (player->plAction !=2) player->plFrameInd++;
             if (player->plAction == 2 && player->plFrameInd < 6) player->plFrameInd++;
             player->plTimer=0;
         }
}


 if (player->plAction == 0){ //Idling

     if (player->plDir == 4){//left.

         if (player->plFrameInd == 2) player->plFrameInd = 0;

         arduboy.drawBitmap(player->plX,player->plY,&splayer[192+(8*player->plFrameInd)],8,8,WHITE);
     }else if (player->plDir == 2){ //down.

         if (player->plFrameInd == 2) player->plFrameInd = 0;

         arduboy.drawBitmap(player->plX,player->plY,&splayer[0+(8*player->plFrameInd)],8,8,WHITE);
     }else if (player->plDir == 6){//up

         if (player->plFrameInd == 2) player->plFrameInd = 0;

         arduboy.drawBitmap(player->plX,player->plY,&splayer[64+(8*+player->plFrameInd)],8,8,WHITE);
     }else if (player->plDir == 0){//right

         if (player->plFrameInd == 2) player->plFrameInd = 0;

         arduboy.drawBitmap(player->plX,player->plY,&splayer[128+(8*player->plFrameInd)],8,8,WHITE);
     }


 }


 if (player->plAction == 1){ //walking


     if (player->plDir == 2){ //walk down.

        if (player->plFrameInd == 6) player->plFrameInd = 0;
         arduboy.drawBitmap(player->plX,player->plY,&splayer[16+(8*player->plFrameInd)],8,8,WHITE);

     }else if (player->plDir == 6){ //walk up.

         if (player->plFrameInd == 6) player->plFrameInd = 0;
         arduboy.drawBitmap(player->plX,player->plY,&splayer[80+(8*player->plFrameInd)],8,8,WHITE);

      }else if (player->plDir == 0){ //walk right.

          if (player->plFrameInd == 6) player->plFrameInd = 0;
          arduboy.drawBitmap(player->plX,player->plY,&splayer[136+(8*player->plFrameInd)],8,8,WHITE);

     }else if (player->plDir == 4){ //walk left.

          if (player->plFrameInd == 6) player->plFrameInd = 0;
         arduboy.drawBitmap(player->plX,player->plY,&splayer[208+(8*player->plFrameInd)],8,8,WHITE);

     }


 }

 if (player->isHit){ //explode

          if (player->plFrameInd < 6){
              arduboy.drawBitmap(player->plX, player->plY, &sexplotion[0+(8*player->plFrameInd)], 8, 8, WHITE);
          }

 }

}

void drawObjects(void){

  //draw flag

  if (flag != 0) {

    if (arduboy.everyXFrames(flag->timer)){
      flag->frameIndex++;
    }

      if (flag->frameIndex == 5) flag->frameIndex = 0;
      arduboy.drawBitmap(flag->iX, flag->iY, &sflag[0+(8*flag->frameIndex)], 8, 8, WHITE);

  }

  //draw hazards
  for (unsigned char i = 0; i < 6; i++){

    if (gameState != 2) {

      if (hazardsList[i] != 0 && hazardsList[i]->active == true){

        if(hazardsList[i]->hType == 1){//Mine

          if (arduboy.everyXFrames(hazardsList[i]->frameTimer)) hazardsList[i]->frameIndex++;

          if (hazardsList[i]->frameIndex == 2) hazardsList[i]->frameIndex = 0;
          arduboy.drawBitmap(hazardsList[i]->hX, hazardsList[i]->hY, &shazards[0+(8*hazardsList[i]->frameIndex)], 8, 8, WHITE);

        }else if (hazardsList[i]->hType == 2){ //turret

          if (arduboy.everyXFrames(hazardsList[i]->frameTimer)) hazardsList[i]->frameIndex++;

          if (hazardsList[i]->frameIndex >= 2) hazardsList[i]->frameIndex = 0;
          arduboy.drawBitmap(hazardsList[i]->hX, hazardsList[i]->hY, &shazards[16+(8*hazardsList[i]->frameIndex)], 8, 8, WHITE);

        }else if (hazardsList[i]->hType == 3){ //pit

          if (arduboy.everyXFrames(hazardsList[i]->frameTimer)) hazardsList[i]->frameIndex++;

          if (hazardsList[i]->frameIndex >= 2) hazardsList[i]->frameIndex = 0;
          arduboy.drawBitmap(hazardsList[i]->hX, hazardsList[i]->hY, &shazards[32+(8*hazardsList[i]->frameIndex)], 8, 8, WHITE);

        }

      }

    }

  }
  //Draw projectiles.
  for (unsigned char i = 0; i < 24; i++){

    if (projectileList[i] != 0){

      if (projectileList[i]->hType == 4){

        if (projectileList[i]->exposeTimer > 0){
          projectileList[i]->exposeTimer--;
        }else{
          projectileList[i]->frameIndex++;
          projectileList[i]->exposeTimer = 10;
        }
        if (projectileList[i]->hType == 4){//bullet

          if (projectileList[i]->frameIndex >= 2) projectileList[i]->frameIndex = 0;

          arduboy.drawBitmap(projectileList[i]->hX, projectileList[i]->hY, &shazards[48+(8*projectileList[i]->frameIndex)],8,8,WHITE);

        }

      }

    }

  }

}

void drawBG(void){

  if (player->isHit == false && gameState != 4 && gameState != 5 && screen == 3){

    //Create arrows if in starting room or in an unlocked room.
    static bool flash = true;

    if (roomLock == false){

      if (arduboy.everyXFrames(20)){
        flash = !flash;
      }

      if (flash){

        if (player->plPosOnMapH > 0) arduboy.drawBitmap(0, 32, &bgtiles[32], 8, 8, WHITE);
        if (player->plPosOnMapH < numOfScreensH) arduboy.drawBitmap(120, 32, &bgtiles[24], 8, 8, WHITE);
        if (player->plPosOnMapV > 0) arduboy.drawBitmap(64, 8, &bgtiles[48], 8, 8, WHITE);
        if (player->plPosOnMapV < numOfScreensV) arduboy.drawBitmap(64, 56, &bgtiles[40], 8, 8, WHITE);

      }

    }

    for(unsigned char i = 0; i < 16; i++){


         arduboy.drawBitmap(i*8, 8, &bgtiles[screenMap[1][i]], 8, 8, WHITE);
         arduboy.drawBitmap(i*8, 16, &bgtiles[screenMap[2][i]], 8, 8, WHITE);
         arduboy.drawBitmap(i*8, 24, &bgtiles[screenMap[3][i]], 8, 8, WHITE);
         arduboy.drawBitmap(i*8, 32, &bgtiles[screenMap[4][i]], 8, 8, WHITE);
         arduboy.drawBitmap(i*8, 40, &bgtiles[screenMap[5][i]], 8, 8, WHITE);
         arduboy.drawBitmap(i*8, 48, &bgtiles[screenMap[6][i]], 8, 8, WHITE);
         arduboy.drawBitmap(i*8, 56, &bgtiles[screenMap[7][i]], 8, 8, WHITE);
         arduboy.drawBitmap(i*8, 64, &bgtiles[screenMap[8][i]], 8, 8, WHITE);

         if (roomLock == true) if (roomMap[player->plPosOnMapV][player->plPosOnMapH] != 5 && roomMap[player->plPosOnMapV][player->plPosOnMapH] != 4) arduboy.drawBitmap(roomSwitchX*8, roomSwitchY*8, &bgSwitch[0],8,8,WHITE);
         if (roomLock == false) if (roomMap[player->plPosOnMapV][player->plPosOnMapH] != 5 && roomMap[player->plPosOnMapV][player->plPosOnMapH] != 4) arduboy.drawBitmap(roomSwitchX*8, roomSwitchY*8, &bgSwitch[8],8,8,WHITE);

           if (roomMap[player->plPosOnMapV][player->plPosOnMapH] == 1 || roomMap[player->plPosOnMapV][player->plPosOnMapH] == 2) {

              if (hazardsList[0] > 0 && hazardsList[0]->active == true) arduboy.drawBitmap(hazardsList[0]->hX, hazardsList[0]->hY, &bgmask[8],8,8,BLACK); //Hazard mask
              if (hazardsList[1] > 0 && hazardsList[1]->active == true) arduboy.drawBitmap(hazardsList[1]->hX, hazardsList[1]->hY, &bgmask[8],8,8,BLACK); //Hazard mask
              if (hazardsList[2] > 0 && hazardsList[2]->active == true) arduboy.drawBitmap(hazardsList[2]->hX, hazardsList[2]->hY, &bgmask[8],8,8,BLACK); //Hazard mask
              if (hazardsList[3] > 0 && hazardsList[3]->active == true) arduboy.drawBitmap(hazardsList[3]->hX, hazardsList[3]->hY, &bgmask[8],8,8,BLACK); //Hazard mask

           }

         arduboy.drawBitmap(player->plX, player->plY, &bgmask[8], 8, 8, BLACK); //player mask.


    }

  }

}
