#ifndef PLAYER__H
#define PLAYER__H

#include "twowaymultisprite.h"
#include <list>
#include <cmath>

class SmartSprite;

// In this example the player is derived from TwoWayMultiSprite.
// However, there are many options.
class Player : public TwoWayMultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);
  virtual ~Player() { }
  virtual void update(Uint32 ticks);

  void collided() { collision = true; }
  void missed() { collision = false; }
  Player& operator=(const Player&);

  void right();
  void left();
  void up();
  void down();
  void stop();

  void attach(SmartSprite* o);
  void detach(SmartSprite* o);
private:
  bool collision;
  Vector2f initialVelocity;
  float slowDownFactor;
  std::list<SmartSprite*> observers;
};
#endif
