#include "twowaymultisprite.h"
#include "gamedata.h"
#include "renderContext.h"

void Twowaymultisprite::switchFrame() {
	if (getVelocityX() < 0) {
    currentFrame = 0;
	}
  else {
    currentFrame = 1;
  }
}

Vector2f Twowaymultisprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);;
  float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);;
  newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  newvy *= [](){ if(rand()%2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}

Twowaymultisprite::Twowaymultisprite( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance()->getImages(name) ),

  currentFrame(0),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Twowaymultisprite::Twowaymultisprite(const Twowaymultisprite& s) :
  Drawable(s),
  images(s.images),
  currentFrame(s.currentFrame),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
  { }

Twowaymultisprite& Twowaymultisprite::operator=(const Twowaymultisprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}

void Twowaymultisprite::draw() const {
  images[currentFrame]->draw(getX(), getY(), getScale());
}

void Twowaymultisprite::update(Uint32 ticks) {
  switchFrame();

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
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  }

}
