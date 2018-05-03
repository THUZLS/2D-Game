#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include <unistd.h>
#include "smartSprite.h"
#include "twowaymultisprite.h"
#include "player.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"
#include "smartTwowaymultisprite.h"
#include "fallingSprite.h"
#include "sound.h"
#include "ioModShort.h"

Engine::~Engine() {
  delete player;
  delete healthBar;
  for(auto sprite : sprites) {
    delete sprite;
  }
  for(auto dragon : dragons) {
    delete dragon;
  }
  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
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
                   healthBar(new HealthBar("HealthBar")),
                   sky("sky", Gamedata::getInstance().getXmlInt("sky/factor")),
                   hills("hills", Gamedata::getInstance().getXmlInt("hills/factor")),
                   mountain("mountain", Gamedata::getInstance().getXmlInt("mountain/factor")),
                   rock("rock", Gamedata::getInstance().getXmlInt("rock/factor")),
                   desert("desert", Gamedata::getInstance().getXmlInt("desert/factor")),
                   viewport(Viewport::getInstance()),
                   sprites(),
                   dragons(),
                   strategies(),
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
                   lifeNumber(Gamedata::getInstance().getXmlInt("numberOfLife")),
                   makeVideo(false),
                   hud(Hud::getInstance()),
                   poolhud(*player),
                   godmode(false),
                   countdown(2),
                   menuEngine()
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

  healthBar->draw();

  //  IoMod::getInstance().writeText("Press m to change strategy", 20, 80);
  viewport.draw(renderer, strategies[currentStrategy]);
  // strategies[currentStrategy]->draw();
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions(SDLSound &sound)
{
  auto it = sprites.begin();
  auto dit = dragons.begin();
  while ( it != sprites.end() ) {
    if ( strategies[currentStrategy]->execute(*player, **it) ) {
      SmartSprite* doa = *it;
      player->detach(doa);
      delete doa;
      it = sprites.erase(it);
    }else if(player->bulletCollided(*it)) {
        SmartSprite* doa = *it;
        doa->explode();
        sound[0];
        ++extinguishedFlames;
        if (winCondition <= extinguishedFlames)
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
      if(!godmode){
        lifeNumber -= 2;
      }
      sound[5];
      if (lifeNumber <= 0)
      {
        std::cout << "Game over!" << std::endl;
        gameState = 1;
      }
    }
    else if (player->bulletCollided(*dit))
    {
      SmartTwoWayMultiSprite *doc = *dit;
      doc->explode();
      if(!godmode){
        --lifeNumber;
      }
      sound[0];
      if (lifeNumber <= 0)
      {
        std::cout << "Game over!" << std::endl;
        gameState = 1;
      }
    }else
      ++dit;
  }
}

void Engine::update(Uint32 ticks, SDLSound& sound)
{
  if (gameState==0){
    checkForCollisions(sound);
    healthBar->update(ticks, lifeNumber);
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

    viewport.update(); // always update viewport last
  }else if(gameState==1){
    healthBar->update(ticks, lifeNumber);
    if ( --countdown <= 0)
    {
      hud.draw(renderer, gameState);
      sound.toggleMusic();
      sound[3];
      //sleep(10);
      clock.pause();
      countdown=2;
    }
  }else if (gameState == 2){
    hud.draw(renderer, gameState);
    clock.pause();
    sound.toggleMusic();
    sound[1];
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
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  SDLSound sound;
  IOmod &io = IOmod::getInstance();
  io.setRenderer(renderer);
  clock.pause();
  int choose = menuEngine.play();
  bool done = (choose > 0) ? false : true;
  if(choose==2)
    godmode = 1;
  clock.unpause();

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
          if ( clock.isPaused() ){
            clock.unpause();
            sound.toggleMusic();
          } else{
            clock.pause();
            sound.toggleMusic();
          }
        }
        if ( keystate[SDL_SCANCODE_F1] ) {
          if (clock.isPaused())
          {
            clock.unpause();
            sound.toggleMusic();
          }
          else{
            hud.draw(renderer, gameState);
            clock.pause();
            sound.toggleMusic();
          }
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if ( keystate[SDL_SCANCODE_C] ) {
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
          sound[5];
          gameState = 1;
          lifeNumber = 0;
        }
        if ( keystate[SDL_SCANCODE_SPACE] ) {
          player->shoot();
          sound[2];
        }
        if (keystate[SDL_SCANCODE_R]){
          clock.unpause();
          static_cast<HealthBar *>(healthBar)->reset();
          return true;
        }
        if (keystate[SDL_SCANCODE_G])
        {
          godmode = 1;
        }
        if (keystate[SDL_SCANCODE_M] || keystate[SDL_SCANCODE_O])
        {
          clock.pause();
          choose = menuEngine.play();
          done = (choose > 0) ? false : true;
          if (choose == 2)
            godmode = 1;
          clock.unpause();
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
      update(ticks, sound);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}
