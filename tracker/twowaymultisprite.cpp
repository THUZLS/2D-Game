#include "twowaymultisprite.h"
#include "gamedata.h"
#include "renderContext.h"
#include "explodingSprite.h"

Vector2f TwoWayMultiSprite::dragonsmakeVelocity(int vx, int vy) const
{
  float newvx = Gamedata::getInstance().getRandFloat(vx - 50, vx + 50);
  float newvy = Gamedata::getInstance().getRandFloat(vy - 50, vy + 50);
  newvx *= []() { if(rand()%2) return -1; else return 1; }();
  newvy *= []() { if(rand()%2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}

void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

TwoWayMultiSprite::~TwoWayMultiSprite()
{
  if (explosion)
    delete explosion;
}

TwoWayMultiSprite::TwoWayMultiSprite(const std::string &name, unsigned currentframe) : Drawable(name,
                                                                                                Vector2f(Gamedata::getInstance().getXmlInt(name + "/startLoc/x"),
                                                                                                         Gamedata::getInstance().getXmlInt(name + "/startLoc/y")),
                                                                                                dragonsmakeVelocity(
                                                                                                    Gamedata::getInstance().getXmlInt(name + "/speedX"),
                                                                                                    Gamedata::getInstance().getXmlInt(name + "/speedY"))),
                                                                                       imagesRight(RenderContext::getInstance()->getImages(name)),
                                                                                       imagesLeft(RenderContext::getInstance()->getImages(name + "Left")),
                                                                                       images(imagesRight),
                                                                                       explosion(nullptr),
                                                                                       currentFrame(currentframe),
                                                                                       numberOfFrames(Gamedata::getInstance().getXmlInt(name + "/frames")),
                                                                                       frameInterval(Gamedata::getInstance().getXmlInt(name + "/frameInterval")),
                                                                                       timeSinceLastFrame(0),
                                                                                       worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
                                                                                       worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

TwoWayMultiSprite::TwoWayMultiSprite(const std::string &name) : Drawable(name,
                                                                         Vector2f(Gamedata::getInstance().getXmlInt(name + "/startLoc/x"),
                                                                                  Gamedata::getInstance().getXmlInt(name + "/startLoc/y")),
                                                                         Vector2f(Gamedata::getInstance().getXmlInt(name + "/speedX"),
                                                                                  Gamedata::getInstance().getXmlInt(name + "/speedY"))),
                                                                imagesRight(RenderContext::getInstance()->getImages(name)),
                                                                imagesLeft(RenderContext::getInstance()->getImages(name + "Left")),
                                                                images(imagesRight),
                                                                explosion(nullptr),
                                                                currentFrame(0),
                                                                numberOfFrames(Gamedata::getInstance().getXmlInt(name + "/frames")),
                                                                frameInterval(Gamedata::getInstance().getXmlInt(name + "/frameInterval")),
                                                                timeSinceLastFrame(0),
                                                                worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
                                                                worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{
}

TwoWayMultiSprite::TwoWayMultiSprite(const TwoWayMultiSprite &s) : Drawable(s),
                                                                   imagesRight(s.imagesRight),
                                                                   imagesLeft(s.imagesLeft),
                                                                   images(s.images),
                                                                   explosion(s.explosion),
                                                                   currentFrame(s.currentFrame),
                                                                   numberOfFrames(s.numberOfFrames),
                                                                   frameInterval(s.frameInterval),
                                                                   timeSinceLastFrame(s.timeSinceLastFrame),
                                                                   worldWidth(s.worldWidth),
                                                                   worldHeight(s.worldHeight)
{ }

TwoWayMultiSprite& TwoWayMultiSprite::operator=(const TwoWayMultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  explosion = s.explosion;
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}

void TwoWayMultiSprite::explode()
{
  if (!explosion)
  {
    Sprite
        sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
  }
}

void TwoWayMultiSprite::draw() const {
  if (explosion)
    explosion->draw();
  else
    images[currentFrame]->draw(getX(), getY(), getScale());
}

void TwoWayMultiSprite::update(Uint32 ticks) {
  if (explosion)
  {
    explosion->update(ticks);
    if (explosion->chunkCount() == 0)
    {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
//    images = imagesRight;
  }
  if (getVelocityX()>0){
  images = imagesRight;
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
//    images = imagesLeft;
  }
  if (getVelocityX()<0){
  images = imagesLeft;
  }

}
