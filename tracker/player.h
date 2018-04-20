#ifndef PLAYER__H
#define PLAYER__H

#include "twowaymultisprite.h"
#include "multibullets.h"
#include <list>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>

class SmartSprite;
class SmartTwoWayMultiSprite;

// In this example the player is derived from TwoWayMultiSprite.
// However, there are many options.
class Player : public TwoWayMultiSprite
{
public:
  Player(const std::string&);
  Player(const Player&);
  virtual ~Player() { }
  virtual void update(Uint32 ticks);

  void collided() { collision = true; }
  void missed() { collision = false; }
  Player& operator=(const Player&)=delete;

  void right();
  void left();
  void up();
  void down();
  void stop();

  void attach(SmartSprite* o);
  void detach(SmartSprite* o);
  void attach(SmartTwoWayMultiSprite *o);
  void detach(SmartTwoWayMultiSprite *o);

  void shoot();
  unsigned int bulletCount() const {return bullets.bulletCount();}
  unsigned int freeCount() const {return bullets.freeCount();}
  virtual void draw() const;

private:
  bool collision;
  Vector2f initialVelocity;
  float slowDownFactor;
  std::list<SmartSprite*> observers;
  std::list<SmartTwoWayMultiSprite*> dragonsobservers;

  //Data attributes for bullets
  std::string bulletName;
  float bulletInterval;
  float timeSinceLastBullet;
  float minBulletSpeed;
  MultiBullets bullets;

  enum FACE {LEFT , RIGHT};
  FACE facing;
};
#endif
