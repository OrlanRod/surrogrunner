
#ifndef PLAYER_H
#define PLAYER_H

class Player{

    public:
      unsigned char plFrameInd = 0; //For looping through animation frames.
      unsigned char plTimer = 0; //Used for incrementing to frameExpose time.
      unsigned char plDir = 0; //Player's facing direction.
      //unsigned char resetRunTimer= 10;
      //unsigned char runTimer= 10;
      unsigned char resetRunTimer= 4;
      unsigned char runTimer= 4;
      unsigned char plX = 64; //Player's Position.
      unsigned char plY = 16;
      unsigned char plSP = 0; //movement speed.
      unsigned char plPosOnMapH = 0;
      unsigned char plPosOnMapV = 0;
      unsigned char plframeExpo = 10;
      unsigned char plAction = 0; //Check/set what action the player is doing. ie idling, walking, etc.
      bool delay = false;
      bool isHit = false;


        Player();

        ~Player();

        bool collision(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

        void plControls(void);


};

#endif
