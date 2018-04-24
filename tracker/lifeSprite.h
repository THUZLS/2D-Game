#ifndef LIFESPRITE__H
#define LIFESPRITE__H
#include <string>
#include "sprite.h"

class LifeSprite : public Sprite {
public:
  LifeSprite(const std::string&, const Vector2f& pos, const Vector2f& vel, const Vector2f& pPos, int w, int h);
  //virtual ~LifeSprite() { } 

  virtual void update(Uint32 ticks, const Vector2f& pos,int number);
  void setPlayerPos(const Vector2f& p) { playerPos = p; }
private:
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
};
#endif
