#include <garbage.h>
#include <flag.h>
#include <hazards.h>
#include <player.h>

extern unsigned char numOfFlagsSpawned;
extern unsigned char numOfHazardsSpawned;
extern bool roomLock;
extern bool spawnedFlag;
extern bool spawnedHazard;

extern Player *player;
extern Flag *flag;
extern Hazards *hazardsList[];
extern Hazards *projectileList[];


void memManageOBJ(String obj, String act){

    if (act == "REMOVEINACTIVE"){

      for (unsigned i = 0; i < 4; i++){

        if (hazardsList[i] != 0){
          if (hazardsList[i]->active == false && hazardsList[i]->frameIndex >= 6) {
            numOfHazardsSpawned--;
            delete hazardsList[i];
            hazardsList[i] = 0;
          }

        }


      }


      for(unsigned char ii = 0; ii < 24; ii++){

        if (projectileList[ii] != 0){ //delete projectile if not active.

            if (projectileList[ii]->active == false){

              for (unsigned char it = 0; it < 6; it++){

                if (hazardsList[it] != 0 && hazardsList[it]->hType == 2){

                  if (projectileList[ii]->hID == hazardsList[it]->hID) {

                    hazardsList[it]->projIndex--;
                    delete projectileList[ii];
                    projectileList[ii] = 0;

                  }

                }

              }


            }

        }

      }

    }else if (act == "REMOVEPLAYER") {

      delete player;

    }else if (act == "REMOVEALL"){

      if (obj == "FLAG"){ //flag

        if (flag != 0){

          numOfFlagsSpawned--;
          delete flag;
          flag = 0;

        }

        if (roomLock == false) spawnedFlag = false;
      }

      if (obj == "HAZARDS"){ //Hazards
        for(unsigned char ii=0; ii < 6; ii++){

          if (hazardsList[ii] != 0) numOfHazardsSpawned--;
          if (hazardsList[ii] != 0) delete hazardsList[ii];
          hazardsList[ii] = 0;

        }
        spawnedHazard = false;
      }

      if (obj == "HAZARDS*"){ //Remove hazards but don't turn off spawnhazard.

        for(unsigned char ii=0; ii < 6; ii++){

          if (hazardsList[ii] != 0) numOfHazardsSpawned--;
          if (hazardsList[ii] != 0) delete hazardsList[ii];
          hazardsList[ii] = 0;

        }

      }

      if (obj == "BULLETS"){ //Hazards

        for(unsigned char ii=0; ii < 24; ii++){

          if (projectileList[ii] != 0) delete projectileList[ii];
          projectileList[ii] = 0;
        }

      }

    }


}
