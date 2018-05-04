#ifndef FLAG_H
#define FLAG_H

class Flag{
private:


public:
  unsigned char iX = 0;
  unsigned char iY = 0;
  unsigned char iId = 0;
  bool active = true;
  unsigned char frameIndex = 0;
  unsigned char timer = 10;

  Flag(); //Constructor
  ~Flag();

};


#endif
