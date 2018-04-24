#include <cmath>
#include <random>
#include <functional>
#include "fallingSprite.h"
#include "gamedata.h"
#include "renderContext.h"

void FallingSprite::randomizeVelocity() {
  float vy = getVelocityY();
  float newvy = Gamedata::getInstance().getRandFloat(50, vy+50);
  setVelocity(Vector2f(0, newvy));
}

void FallingSprite::randomizePosition() {
  float newpx = Gamedata::getInstance().getRandFloat(-10, Gamedata::getInstance().getXmlInt("world/width") );
  float newpy = Gamedata::getInstance().getRandFloat(-10, Gamedata::getInstance().getXmlInt("world/height"));
  setPosition(Vector2f(newpx, newpy));
}

void FallingSprite::update(Uint32 ticks) {
  MultiSprite::advanceFrame(ticks);

  setPosition(getPosition() + (getVelocity() * static_cast<float>(ticks) * 0.001));

  if (getY() > worldHeight-getScaledHeight()) {
    setY(0 - getScaledHeight());
  }
}

void FallingSprite::collided() {
  std::cout << "falling sprite " << getName() << "collided" << std::endl;
}

