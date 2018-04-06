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

class Player;
class CollisionStrategy;
class SmartSprite;
//class SubjectSprite;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World sky;
  World hills;
  World mountain;
  World rock;
  World desert;
  Viewport& viewport;

  std::vector<SmartSprite*> sprites;
  std::vector<TwoWayMultiSprite*> dragons;
  std::vector<CollisionStrategy*> strategies;
  Player* player;
  int currentSprite;
  int currentStrategy;
  bool collision;

  bool makeVideo;
  Hud& hud;

  void draw() const;
  void update(Uint32);
  

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions();
};
