#include <cmath>
#include <random>
#include <functional>
#include "smartTwowaymultisprite.h"
#include "gamedata.h"
#include "renderContext.h"

float dragonsdistance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

SmartTwoWayMultiSprite::SmartTwoWayMultiSprite(const std::string &name, const Vector2f &pos,
                                               int w, int h) : TwoWayMultiSprite(name,0),
                                                               playerPos(pos),
                                                               playerWidth(w),
                                                               playerHeight(h),
                                                               currentMode(NORMAL),
                                                               safeDistance(Gamedata::getInstance().getXmlFloat(name + "/safeDistance"))
{}

SmartTwoWayMultiSprite::SmartTwoWayMultiSprite(const SmartTwoWayMultiSprite &s) : TwoWayMultiSprite(s),
                                                                                  playerPos(s.playerPos),
                                                                                  playerWidth(s.playerWidth),
                                                                                  playerHeight(s.playerHeight),
                                                                                  currentMode(s.currentMode),
                                                                                  safeDistance(s.safeDistance)
{}

void SmartTwoWayMultiSprite::update(Uint32 ticks) { 
  TwoWayMultiSprite::update(ticks);
  float x= getX()+getImage()->getWidth()/2;
  float y= getY()+getImage()->getHeight()/2;
  float ex= playerPos[0]+playerWidth/2;
  float ey= playerPos[1]+playerHeight/2;
  float distanceToEnemy = ::dragonsdistance( x, y, ex, ey );

  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < safeDistance) currentMode = EVADE;
  }
  else if  ( currentMode == EVADE ) {
    if(distanceToEnemy > safeDistance) currentMode=NORMAL;
    else {
      TwoWayMultiSprite::explode();
    }
  }
}
