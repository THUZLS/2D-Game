#ifndef FALLINGSPRITE__H
#define FALLINGSPRITE__H
#include <string>
#include "multisprite.h"

class FallingSprite : public MultiSprite {
public:
  FallingSprite(const std::string& name) : MultiSprite(name) { }
  FallingSprite(const FallingSprite& s) : MultiSprite(s) { }
  virtual ~FallingSprite() { }
  virtual void update(Uint32 ticks);
  virtual void collided();
  void randomizeVelocity();
  void randomizePosition();
};
#endif

