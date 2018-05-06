#include <Arduboy2.h>

#ifndef HAZARDS_H
#define HAZARDS_H

class Hazards{

private:

public:
  unsigned char hID = 0;
  unsigned char hType = 0; //1 = mine , 2 = turret, 3 = pit, 4 = bullet
  bool active = true;
  unsigned char hAI = 1;
  unsigned char hX = 0;
  unsigned char hY = 0;
  unsigned char hDir = 0;
  unsigned char hSPD = 0;
  unsigned char stepTimer;
  unsigned char cellSteps = 8;
  unsigned char frameIndex = 0;
  unsigned char exposeTimer = 10;

  //projectile vars.
  unsigned char projMoveTimerSet;

  //mine vars
  unsigned char frameTimer = 10;
  bool dirCycle = false;

  //turret vars
  bool fire = false;
  unsigned char fireStyle = 1; //1 = diag, 2= strait.
  unsigned char fireIndex = 0;
  unsigned char projIndex = 0;
  unsigned char setSpawnTimer = 150;

  //Pit vars
  bool pitTrigger = false;

  Hazards();
  ~Hazards();

  char getDistance(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2);
  void hazardAI(void);

};

#endif
