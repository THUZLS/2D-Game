#ifndef SMARTTWOWAYMULTISPRITE__H
#define SMARTTWOWAYMULTISPRITE__H
#include <string>
#include "twowaymultisprite.h"

class SmartTwoWayMultiSprite : public TwoWayMultiSprite
{
public:
  SmartTwoWayMultiSprite(const std::string&, const Vector2f& pos, int w, int h);
  SmartTwoWayMultiSprite(const SmartTwoWayMultiSprite&);
  virtual ~SmartTwoWayMultiSprite() { } 

  virtual void update(Uint32 ticks);
  void setPlayerPos(const Vector2f& p) { playerPos = p; }

private:
  enum MODE {NORMAL, EVADE};
  Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  float safeDistance;
};
#endif
