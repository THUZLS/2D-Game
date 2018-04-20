#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "smartSprite.h"
#include "twowaymultisprite.h"
#include "player.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"
#include "smartTwowaymultisprite.h"

Engine::~Engine() {
  delete player;
  for(auto sprite : sprites) {
    delete sprite;
  }
  for(auto dragon : dragons) {
    delete dragon;
  }
  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  sky("sky", Gamedata::getInstance().getXmlInt("sky/factor") ),
  hills("hills", Gamedata::getInstance().getXmlInt("hills/factor") ),
  mountain("mountain", Gamedata::getInstance().getXmlInt("mountain/factor") ),
  rock("rock", Gamedata::getInstance().getXmlInt("rock/factor") ),
  desert("desert", Gamedata::getInstance().getXmlInt("desert/factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  dragons(),
  strategies(),
  player(new Player("LanceDragon")),
  currentSprite(0),
  currentStrategy(1),
  collision(false),
  makeVideo( false ),
  hud(Hud::getInstance()),
  poolhud(*player)
{
  for (int i = 0; i < Gamedata::getInstance().getXmlInt("numDragon"); i++) {
    Vector2f pos = player->getPosition();
    int w = player->getScaledWidth();
    int h = player->getScaledHeight();
    dragons.push_back(new SmartTwoWayMultiSprite("dragon", pos, w, h));
    player->attach(dragons[i]);
  }
  for (int i = 0; i < Gamedata::getInstance().getXmlInt("numFlame"); i++) {
    Vector2f pos = player->getPosition();
    int w = player->getScaledWidth();
    int h = player->getScaledHeight();
    sprites.push_back(new SmartSprite("flame", pos, w, h));
    player->attach( sprites[i] );
  }

  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  sky.draw();
  hills.draw();
  mountain.draw();
  rock.draw();
  desert.draw();
  player->draw();
  poolhud.draw();

  for (int i = 0; i < (int)sprites.size(); i++) {
    sprites[i]->draw();
  }

  for (int i = 0; i < (int)dragons.size(); i++) {
    dragons[i]->draw();
  }

//  IoMod::getInstance().writeText("Press m to change strategy", 20, 80);
  strategies[currentStrategy]->draw();

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  auto it = sprites.begin();
  while ( it != sprites.end() ) {
    if ( strategies[currentStrategy]->execute(*player, **it) ) {
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = sprites.erase(it);
    }
    else ++it;
  }
}

void Engine::update(Uint32 ticks) {
  checkForCollisions();
  poolhud.update();
  player->update(ticks);
  sky.update();
  hills.update();
  mountain.update();
  rock.update();
  desert.update();

  for (int i = 0; i < (int)sprites.size(); i++) {
    sprites[i]->update(ticks);
  }
  for (int i = 0; i < (int)dragons.size(); i++) {
    dragons[i]->update(ticks);
  }

  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_F1] ) {
          if ( clock.isPaused() ) clock.unpause();
          else{
            hud.draw(renderer);
            clock.pause();
          }
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if ( keystate[SDL_SCANCODE_M] ) {
                currentStrategy = (1 + currentStrategy) % strategies.size();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
        if (keystate[SDL_SCANCODE_E])
        {
          player->explode();
        }
        if ( keystate[SDL_SCANCODE_SPACE] ) {
          player->shoot();
        }

      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(player)->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->down();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
