#ifndef ITEMS_H
#define ITEMS_H

class Items{
private:
  unsigned char iX = 0;
  unsigned char iY = 0;
  unsigned char iId = 0;

public:
  bool active = true;

  Items(); //Constructor
  ~Items();

  char getItemXPos(void);
  char getItemYPos(void);
  void setItemXPos(unsigned char x);
  void setItemYPos(unsigned char y);
  char getItemID(void);
  void setItemID(unsigned char i);

  void itemDraw(unsigned char x, unsigned char y, unsigned char anim, unsigned char animExpose);

};


#endif
