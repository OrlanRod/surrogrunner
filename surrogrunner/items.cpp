#include "items.h"
#include "assets.h"

extern Arduboy2 arduboy;

Items::Items(){

}

Items::~Items(){

}

char Items::getItemXPos(){
  return iX;
}

char Items::getItemYPos(){
  return iY;
}

void Items::setItemXPos(unsigned char x){
  iX = x;
}
void Items::setItemYPos(unsigned char y){
  iY = y;
}

char Items::getItemID(){
  return iId;
}

void Items::setItemID(unsigned char id){
  iId = id;
}

void Items::itemDraw(unsigned char x, unsigned char y, unsigned char anim, unsigned char animExpose){
  static unsigned char frameIndex;
  static unsigned char timer = animExpose;

  if (timer > 0){
    timer--;
  } else {
    frameIndex++;
    timer = animExpose;
  }

  if (anim == 1){//flag
    if (frameIndex == 5) frameIndex = 0;
    arduboy.drawBitmap(x,y, &sflag[0+(8*frameIndex)], 8, 8, WHITE);
  }


}
