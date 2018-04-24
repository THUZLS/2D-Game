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
#include "lifeSprite.h"
#include "fallingSprite.h"

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
  for (LifeSprite *life : lifeVector){
    delete life;
  }
  for (FallingSprite *fallingSprite : farFallingSprites){
    delete fallingSprite;
  }
  for (FallingSprite *fallingSprite : middleFallingSprites){
    delete fallingSprite;
  }
  for (FallingSprite *fallingSprite : closeFallingSprites){
    delete fallingSprite;
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() : rc(RenderContext::getInstance()),
                   io(IoMod::getInstance()),
                   clock(Clock::getInstance()),
                   renderer(rc->getRenderer()),
                   sky("sky", Gamedata::getInstance().getXmlInt("sky/factor")),
                   hills("hills", Gamedata::getInstance().getXmlInt("hills/factor")),
                   mountain("mountain", Gamedata::getInstance().getXmlInt("mountain/factor")),
                   rock("rock", Gamedata::getInstance().getXmlInt("rock/factor")),
                   desert("desert", Gamedata::getInstance().getXmlInt("desert/factor")),
                   viewport(Viewport::getInstance()),
                   sprites(),
                   dragons(),
                   strategies(),
                   lifeVector(),
                   farFallingSprites(),
                   middleFallingSprites(),
                   closeFallingSprites(),
                   player(new Player("LanceDragon")),
                   currentSprite(0),
                   currentStrategy(1),
                   collision(false),
                   gameState(0),
                   winCondition(Gamedata::getInstance().getXmlInt("winCondition")),
                   extinguishedFlames(0),
                   makeVideo(false),
                   hud(Hud::getInstance()),
                   poolhud(*player)
{
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  for (int i = 0; i < Gamedata::getInstance().getXmlInt("numDragon"); i++) { 
    dragons.push_back(new SmartTwoWayMultiSprite("dragon", pos, w, h));
    player->attach(dragons[i]);
  }
  for (int i = 0; i < Gamedata::getInstance().getXmlInt("numFlame"); i++) {
    sprites.push_back(new SmartSprite("flame", pos, w, h));
    player->attach( sprites[i] );
  }

  int lifeNumber = Gamedata::getInstance().getXmlInt("numberOfLife");
  lifeVector.reserve(lifeNumber);
  for (int i = 0; i < lifeNumber; ++i)
  {
    lifeVector.push_back(new LifeSprite("Lifebar", pos, Vector2f(0, 0), pos, w, h));
  }
  
  int n = Gamedata::getInstance().getXmlInt("numSnows");
  for (int i=0; i<(int)(n*0.33); i++) {
    farFallingSprites.push_back(new FallingSprite("Snow"));
    farFallingSprites.back()->setScale(farFallingSprites.back()->getScale() * 0.5);
    farFallingSprites.back()->randomizePosition();
    farFallingSprites.back()->randomizeVelocity();
  }
  for (int i=(int)(n*0.33); i<(int)(n*0.66); i++) {
    middleFallingSprites.push_back(new FallingSprite("Snow"));
    middleFallingSprites.back()->setScale(middleFallingSprites.back()->getScale() * 0.8);
    middleFallingSprites.back()->randomizePosition();
    middleFallingSprites.back()->randomizeVelocity();
  }
  for (int i=(int)(n*0.66); i<n; i++) {
    closeFallingSprites.push_back(new FallingSprite("Snow"));
    closeFallingSprites.back()->randomizePosition();
    closeFallingSprites.back()->randomizeVelocity();
  }

  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );

  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  sky.draw();
  for (const FallingSprite* fallingSprite : farFallingSprites) fallingSprite->draw();
  hills.draw();
  for (const FallingSprite* fallingSprite : middleFallingSprites) fallingSprite->draw();
  mountain.draw();
  rock.draw();
  for (const FallingSprite* fallingSprite : closeFallingSprites) fallingSprite->draw();
  desert.draw();
  player->draw();
  poolhud.draw(extinguishedFlames);

  for (int i = 0; i < (int)sprites.size(); i++) {
    sprites[i]->draw();
  }

  for (int i = 0; i < (int)dragons.size(); i++) {
    dragons[i]->draw();
  }

  for (unsigned int i = 0; i < lifeVector.size(); i++){
    lifeVector.at(i)->draw();
  }

  //  IoMod::getInstance().writeText("Press m to change strategy", 20, 80);
  viewport.draw(renderer, strategies[currentStrategy]);
  // strategies[currentStrategy]->draw();
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  auto it = sprites.begin();
  auto dit = dragons.begin();
  auto lit = lifeVector.end() - 1;
  while ( it != sprites.end() ) {
    if ( strategies[currentStrategy]->execute(*player, **it) ) {
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = sprites.erase(it);
    }else if(player->bulletCollided(*it)) {
        SmartSprite* doa = *it;
        doa->explode();
        ++extinguishedFlames;
        if (winCondition == extinguishedFlames)
        {
          std::cout << "You Win!" << std::endl;
          gameState = 2;
        }
    }else
        ++it;
  }

  while (dit != dragons.end())
  {
    if (strategies[currentStrategy]->execute(*player, **dit))
    {
      SmartTwoWayMultiSprite *doc = *dit;
      player->detach(doc);
      delete doc;
      dit = dragons.erase(dit);
      // delete doc;
      if (lifeVector.size() > 0)
      {
        LifeSprite *dob = *lit;
        delete dob;
        lit = lifeVector.erase(lit);
        lit--;
        lit = lifeVector.erase(lit);
        lit--;
        if (lifeVector.size() == 0)
        {
          std::cout << "Game over!" << std::endl;
          gameState = 1;
        }
      }
    }
    else if (player->bulletCollided(*dit))
    {
      SmartTwoWayMultiSprite *doc = *dit;
      doc->explode();
      if (lifeVector.size() > 0)
      {
        LifeSprite *dob = *lit;
        delete dob;
        lit = lifeVector.erase(lit);
        lit--;
        if (lifeVector.size() == 0)
        {
          std::cout << "Game over!" << std::endl;
          gameState = 1;
        }
      }
    }
    else
      ++dit;
  }
}

void Engine::update(Uint32 ticks) {
  if (!gameState){
    checkForCollisions();
    poolhud.update();
    player->update(ticks);
    for (FallingSprite* fallingSprite : farFallingSprites) fallingSprite->update(ticks);
    for (FallingSprite* fallingSprite : middleFallingSprites) fallingSprite->update(ticks);
    for (FallingSprite* fallingSprite : closeFallingSprites) fallingSprite->update(ticks);
    sky.update();
    hills.update();
    mountain.update();
    rock.update();
    desert.update();

    for (int i = 0; i < (int)sprites.size(); i++)
    {
      sprites[i]->update(ticks);
    }
    for (int i = 0; i < (int)dragons.size(); i++)
    {
      dragons[i]->update(ticks);
    }

    int number = 0;
    for (LifeSprite *life : lifeVector)
    {
      life->update(ticks, player->getPosition(), number);
      number++;
    }

    viewport.update(); // always update viewport last
  }else if(gameState==1){
    hud.draw(renderer, gameState);
    clock.pause();
  }else if (gameState == 2){
    hud.draw(renderer, gameState);
    clock.pause();
  }
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % sprites.size();
  Viewport::getInstance().setObjectToTrack(sprites[currentSprite]);
}

bool Engine::play() {
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
            hud.draw(renderer, gameState);
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
          // gameState = 1;
        }
        if ( keystate[SDL_SCANCODE_SPACE] ) {
          player->shoot();
        }
        if (keystate[SDL_SCANCODE_R]){
          clock.unpause();
          return true;
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
  return false;
}
