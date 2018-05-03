#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "twowaymultisprite.h"
#include "smartSprite.h"
#include "hud.h"
#include "poolhud.h"
#include "healthBar.h"
#include "menuEngine.h"

class Player;
class CollisionStrategy;
class SmartSprite;
class SmartTwoWayMultiSprite;
class FallingSprite;
class SDLSound;
//class SubjectSprite;

class Engine
{
public:
  Engine ();
  ~Engine ();
  bool play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  HealthBar *healthBar;
  World sky;
  World hills;
  World mountain;
  World rock;
  World desert;
  Viewport& viewport;

  std::vector<SmartSprite*> sprites;
  std::vector<SmartTwoWayMultiSprite*> dragons;
  std::vector<CollisionStrategy*> strategies;
  std::vector<FallingSprite*> farFallingSprites;
  std::vector<FallingSprite*> middleFallingSprites;
  std::vector<FallingSprite*> closeFallingSprites;
  Player* player;
  int currentSprite;
  int currentStrategy;
  bool collision;
  int gameState;
  int winCondition;
  int extinguishedFlames;
  int lifeNumber;

  bool makeVideo;
  Hud& hud;
  PoolHud poolhud;
  bool godmode;
  int countdown;
  MenuEngine menuEngine;

  void draw() const;
  void update(Uint32, SDLSound &sound);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions(SDLSound &sound);
};
