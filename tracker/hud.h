#include <SDL2/SDL.h>
#include "gamedata.h"

class Hud {
public:

  static Hud& getInstance();
  ~Hud();
  void draw(SDL_Renderer * const renderer, int gameState) const;

private:
  const Gamedata& gdata;
  SDL_Color hudColor;
  SDL_Rect r;
  // int gameState;
  Hud():gdata(Gamedata::getInstance()),
        hudColor({static_cast<Uint8>(gdata.getXmlInt("display/hud/fontcolor/red")),
			                                           static_cast<Uint8>(gdata.getXmlInt("display/hud/fontcolor/green")),
                                                 static_cast<Uint8>(gdata.getXmlInt("display/hud/fontcolor/blue")),
			                                           static_cast<Uint8>(gdata.getXmlInt("display/hud/fontcolor/alpha"))}),
        r({gdata.getXmlInt("display/hud/locX"),
            gdata.getXmlInt("display/hud/locY"),
            gdata.getXmlInt("display/hud/width"),
            gdata.getXmlInt("display/hud/height")}){}
  Hud(const Hud&) = delete;
  Hud& operator=(const Hud&) = delete;
};
