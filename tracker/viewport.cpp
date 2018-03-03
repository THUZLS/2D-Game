#include <sstream>
#include "viewport.h"
#include "ioMod.h"
#include "clock.h"

Viewport& Viewport::getInstance() {
  static Viewport viewport;
  return viewport;
}

Viewport::Viewport() : 
  gdata(Gamedata::getInstance()),
  position(0, 0),
  worldWidth(gdata.getXmlInt("world/width")),
  worldHeight(gdata.getXmlInt("world/height")),
  viewWidth(gdata.getXmlInt("view/width")), 
  viewHeight(gdata.getXmlInt("view/height")),
  objWidth(0), objHeight(0),
  objectToTrack(NULL) 
{}

void Viewport::setObjectToTrack(const Drawable *obj) { 
  objectToTrack = obj; 
  objWidth = objectToTrack->getScaledWidth();
  objHeight = objectToTrack->getScaledHeight();
}

void Viewport::draw() const {
  SDL_Color trackingColor = {static_cast<Uint8>(gdata.getXmlInt("display/tracking/fontcolor/red")), static_cast<Uint8>(gdata.getXmlInt("display/tracking/fontcolor/green")),
                        static_cast<Uint8>(gdata.getXmlInt("display/tracking/fontcolor/blue")), static_cast<Uint8>(gdata.getXmlInt("display/tracking/fontcolor/alpha"))};

  SDL_Color fpsColor = {static_cast<Uint8>(gdata.getXmlInt("display/fps/fontcolor/red")), static_cast<Uint8>(gdata.getXmlInt("display/fps/fontcolor/green")),
                        static_cast<Uint8>(gdata.getXmlInt("display/fps/fontcolor/blue")), static_cast<Uint8>(gdata.getXmlInt("display/fps/fontcolor/alpha"))};

  SDL_Color nameColor = {static_cast<Uint8>(gdata.getXmlInt("display/name/fontcolor/red")), static_cast<Uint8>(gdata.getXmlInt("display/name/fontcolor/green")),
                        static_cast<Uint8>(gdata.getXmlInt("display/name/fontcolor/blue")), static_cast<Uint8>(gdata.getXmlInt("display/name/fontcolor/alpha"))};

  IoMod::getInstance().
    writeText("Tracking: "+objectToTrack->getName(),
              gdata.getXmlInt("display/tracking/locX"),
              gdata.getXmlInt("display/tracking/locY"),
              trackingColor);

  std::ostringstream fpsStream;
  fpsStream << "FPS " << Clock::getInstance().getFps() << std::endl;
  IoMod::getInstance().
    writeText(fpsStream.str(),
              gdata.getXmlInt("display/fps/locX"),
              gdata.getXmlInt("display/fps/locY"),
              fpsColor);

  IoMod::getInstance().
    writeText("Lance Zhang",
              gdata.getXmlInt("display/name/locX"),
              gdata.getXmlInt("display/name/locY"),
              nameColor);
}

void Viewport::update() {
  const float x = objectToTrack->getX();
  const float y = objectToTrack->getY();

  position[0] = (x + objWidth/2) - viewWidth/2;
  position[1] = (y + objHeight/2) - viewHeight/2;
  if (position[0] < 0) position[0] = 0;
  if (position[1] < 0) position[1] = 0;
  if (position[0] > (worldWidth - viewWidth)) {
    position[0] = worldWidth-viewWidth;
  }
  if (position[1] > (worldHeight - viewHeight)) {
    position[1] = worldHeight-viewHeight;
  }
}