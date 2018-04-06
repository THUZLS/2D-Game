#include "player.h"
#include "gamedata.h"
#include "smartSprite.h"

Player::Player( const std::string& name) :
  TwoWayMultiSprite(name),
  collision(false),
  initialVelocity(getVelocity()),
  slowDownFactor(Gamedata::getInstance().getXmlFloat(name+"/slowDownFactor")),
  observers()
{ setVelocityX(0); setVelocityY(0); }

Player::Player(const Player& s) :
  TwoWayMultiSprite(s), 
  collision(s.collision),
  initialVelocity(s.getVelocity()),
  slowDownFactor(s.slowDownFactor),
  observers(s.observers)
  { }

Player& Player::operator=(const Player& s) {
  TwoWayMultiSprite::operator=(s);
  collision = s.collision;
  initialVelocity = s.initialVelocity;
  slowDownFactor = s.slowDownFactor;
  observers = s.observers;
  return *this;
}

void Player::stop() { 
  //setVelocity( Vector2f(0, 0) );
  setVelocityX(slowDownFactor * getVelocityX());
  setVelocityY(slowDownFactor * getVelocityY());
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
  }
} 
void Player::left()  { 
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
} 
void Player::up()    { 
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down()  { 
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::update(Uint32 ticks) {
  if ( !collision ) TwoWayMultiSprite::update(ticks);
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }
  stop();
}

void Player::attach(SmartSprite* o) {
  observers.push_back(o);
}

void Player::detach(SmartSprite* o) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while (ptr != observers.end()) {
    if (*ptr == o) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}
