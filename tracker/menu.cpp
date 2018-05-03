#include<sstream>
#include "menu.h"
#include "ioMod.h"

Menu::Menu(SDL_Renderer* rend) :
  renderer(rend),
  gdata(Gamedata::getInstance()),
  hudFrame( {gdata.getXmlInt("menu/loc/x"),
             gdata.getXmlInt("menu/loc/y"),
             gdata.getXmlInt("menu/width"),
             gdata.getXmlInt("menu/height")}
  ),
  menuColor({static_cast<Uint8>(gdata.getXmlInt("menu/color/r")),
             static_cast<Uint8>(gdata.getXmlInt("menu/color/g")),
             static_cast<Uint8>(gdata.getXmlInt("menu/color/b")),
             static_cast<Uint8>(gdata.getXmlInt("menu/color/a"))}
  ),
  textColor({static_cast<Uint8>(gdata.getXmlInt("menu/textcolor/r")),
             static_cast<Uint8>(gdata.getXmlInt("menu/textcolor/g")),
             static_cast<Uint8>(gdata.getXmlInt("menu/textcolor/b")),
             static_cast<Uint8>(gdata.getXmlInt("menu/textcolor/a"))}
  ),
  options(),
  optionLoc( { gdata.getXmlInt("menu/optionLoc/x"),
               gdata.getXmlInt("menu/optionLoc/y")}
           ),
  clicks( {Sprite("clickOff"), Sprite("clickOn")} ),
  currentClick(0),
  currentOption(0),
  spaces(gdata.getXmlInt("menu/spaces")),
  startClickX(optionLoc[0]-spaces),
  startClickY(optionLoc[1]+spaces),
  clickX(startClickX),
  clickY(startClickY)
{
  int noOfOptions = gdata.getXmlInt("menu/noOfOptions");
  std::stringstream strm;
  for (int i = 0; i < noOfOptions; ++i) {
    strm.clear();
    strm.str("");
    strm << i;
    std::string option("menu/option"+strm.str());
    options.push_back(gdata.getXmlStr(option));
  }
}

void Menu::incrIcon() {
  clickY += spaces;
  if ( clickY > static_cast<int>(options.size())*spaces+optionLoc[1]) {
    clickY = startClickY;
    currentOption = 0;
  }
  else ++currentOption;
}

void Menu::decrIcon() {
  clickY -= spaces;
  if ( clickY < spaces+optionLoc[1]) {
    clickY = startClickY+2*spaces;
    currentOption = options.size()-1;
  }
  else --currentOption;
}

void Menu::draw() const {
  // First set the blend mode so that alpha blending will work;
  // the default blend mode is SDL_BLENDMODE_NONE!
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  // Set the hud background color:
  SDL_SetRenderDrawColor( renderer, 255, 255, 255, .9*255 );

  // Draw the filled rectangle:
  SDL_RenderFillRect( renderer, &hudFrame );

  // Set the  color for the Menu outline:
  SDL_Rect menuFrame = {hudFrame.x+50, hudFrame.y+40,
                        hudFrame.w-100, hudFrame.h-100};
  SDL_SetRenderDrawColor( renderer, menuColor.r,
                          menuColor.g, menuColor.b, menuColor.a );
  SDL_RenderFillRect( renderer, &menuFrame );

  IoMod::getInstance().writeText("Start Menu", hudFrame.x + 830, hudFrame.y + 80, textColor, "Marker_Felt_Thin");
  std::stringstream strm;
  for (int i = 1; i <= 8; ++i)
  {
    strm.clear();
    strm.str("");
    strm << i;
    IoMod::getInstance().writeText(gdata.getXmlStr("menu/text" + strm.str()),
                                   hudFrame.x + 380,
                                   hudFrame.y + 100 + 60 * i + ((i>=7)?60:0),
                                   textColor, "Marker_Felt_Thin");
  }
  int space = spaces;
  for ( const std::string& option : options ) {
    IoMod::getInstance().writeText(option, optionLoc[0], optionLoc[1] + space, textColor, "Marker_Felt_Thin");
    space += spaces;
  }
  // We have to draw the clickOn & clickOff relative to the screen,
  // and we don't want to offset by the location of the viewprot:
  clicks[currentClick].getImage()->draw(0, 0, clickX, clickY);
}

