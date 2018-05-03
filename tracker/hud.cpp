#include "ioMod.h"
#include "hud.h"

Hud& Hud::getInstance() {
  static Hud instance;
  return instance;

}

Hud::~Hud() {
  std::cout << "Deleting Hud" << std::endl;
}

void Hud::draw(SDL_Renderer * const renderer, int gameState) const {
  // First set the blend mode so that alpha blending will work;
    // the default blend mode is SDL_BLENDMODE_NONE!
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    // Now set the color for the hud:
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255/2 );

    // Render rect
    SDL_RenderFillRect( renderer, &r );

    // Now set the color for the outline of the hud:
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255/2 );
    SDL_RenderDrawRect( renderer, &r );

  if(gameState==0){
    IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/title/text"),
                                   gdata.getXmlInt("display/hud/title/locX"),
                                   gdata.getXmlInt("display/hud/title/locY"),
                                   hudColor, "Marker_Felt_Thin");
    IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/SPACE_key/text"),
                                   gdata.getXmlInt("display/hud/SPACE_key/locX"),
                                   gdata.getXmlInt("display/hud/SPACE_key/locY"),
                                   hudColor, "Marker_Felt_Thin");
  }else if(gameState==1){
    IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/lose/text"),
                                   gdata.getXmlInt("display/hud/lose/locX"),
                                   gdata.getXmlInt("display/hud/lose/locY"),
                                   hudColor, "Marker_Felt_Thin");
    IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/RESTART_key/text"),
                                   gdata.getXmlInt("display/hud/RESTART_key/locX"),
                                   gdata.getXmlInt("display/hud/RESTART_key/locY"),
                                   hudColor, "Marker_Felt_Thin");
  }else if(gameState==2){
    IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/win/text"),
                                   gdata.getXmlInt("display/hud/win/locX"),
                                   gdata.getXmlInt("display/hud/win/locY"),
                                   hudColor, "Marker_Felt_Thin");
    IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/RESTART_key/text"),
                                   gdata.getXmlInt("display/hud/RESTART_key/locX"),
                                   gdata.getXmlInt("display/hud/RESTART_key/locY"),
                                   hudColor, "Marker_Felt_Thin");
  }
  IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/R_key/text"),
                                 gdata.getXmlInt("display/hud/R_key/locX"),
                                 gdata.getXmlInt("display/hud/R_key/locY"),
                                 hudColor, "Marker_Felt_Thin");
  IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/A_key/text"),
                                 gdata.getXmlInt("display/hud/A_key/locX"),
                                 gdata.getXmlInt("display/hud/A_key/locY"),
                                 hudColor, "Marker_Felt_Thin");
  IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/D_key/text"),
                                 gdata.getXmlInt("display/hud/D_key/locX"),
                                 gdata.getXmlInt("display/hud/D_key/locY"),
                                 hudColor, "Marker_Felt_Thin");
  IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/W_key/text"),
                                 gdata.getXmlInt("display/hud/W_key/locX"),
                                 gdata.getXmlInt("display/hud/W_key/locY"),
                                 hudColor, "Marker_Felt_Thin");
  IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/S_key/text"),
                                 gdata.getXmlInt("display/hud/S_key/locX"),
                                 gdata.getXmlInt("display/hud/S_key/locY"),
                                 hudColor, "Marker_Felt_Thin");
  IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/M_key/text"),
                                 gdata.getXmlInt("display/hud/M_key/locX"),
                                 gdata.getXmlInt("display/hud/M_key/locY"),
                                 hudColor, "Marker_Felt_Thin");
  IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/F1_key/text"),
                                 gdata.getXmlInt("display/hud/F1_key/locX"),
                                 gdata.getXmlInt("display/hud/F1_key/locY"),
                                 hudColor, "Marker_Felt_Thin");
  IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/C_key/text"),
                                 gdata.getXmlInt("display/hud/C_key/locX"),
                                 gdata.getXmlInt("display/hud/C_key/locY"),
                                 hudColor, "Marker_Felt_Thin");
  IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/G_key/text"),
                                 gdata.getXmlInt("display/hud/G_key/locX"),
                                 gdata.getXmlInt("display/hud/G_key/locY"),
                                 hudColor, "Marker_Felt_Thin");
  IoMod::getInstance().writeText(gdata.getXmlStr("display/hud/E_key/text"),
                                 gdata.getXmlInt("display/hud/E_key/locX"),
                                 gdata.getXmlInt("display/hud/E_key/locY"),
                                 hudColor, "Marker_Felt_Thin");

  // Render the rect to the screen
  SDL_RenderPresent(renderer);

}
