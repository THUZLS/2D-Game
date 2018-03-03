#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class Twowaymultisprite : public Drawable {
public:
  Twowaymultisprite(const std::string&);
  Twowaymultisprite(const Twowaymultisprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const {
    return images[currentFrame];
  }
  int getScaledWidth()  const {
    return getScale()*images[currentFrame]->getWidth();
  }
  int getScaledHeight()  const {
    return getScale()*images[currentFrame]->getHeight();
  }
  virtual const SDL_Surface* getSurface() const {
    return images[currentFrame]->getSurface();
  }

protected:
  std::vector<Image *> images;

  unsigned currentFrame;
  int worldWidth;
  int worldHeight;

  void switchFrame();
  Twowaymultisprite& operator=(const Twowaymultisprite&);

  Vector2f makeVelocity(int, int) const;
};
#endif
