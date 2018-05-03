#include "player.h"
#include "gamedata.h"
#include "smartSprite.h"
#include "smartTwowaymultisprite.h"
#include <cmath>
#include "imageFactory.h"

Player::Player(const std::string &name) : TwoWayMultiSprite(name),
                                          collision(false),
                                          initialVelocity(getVelocity()),
                                          slowDownFactor(Gamedata::getInstance().getXmlFloat(name + "/slowDownFactor")),
                                          observers(),
                                          dragonsobservers(),
                                          bulletName(Gamedata::getInstance().getXmlStr(name+"/bulletName")),
                                          bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
                                          timeSinceLastBullet(0),
                                          minBulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/minSpeedX")),
                                          bullets(bulletName),
                                          facing(RIGHT)
{ setVelocityX(0); setVelocityY(0); }

Player::Player(const Player &s) : TwoWayMultiSprite(s),
                                  collision(s.collision),
                                  initialVelocity(s.getVelocity()),
                                  slowDownFactor(s.slowDownFactor),
                                  observers(s.observers),
                                  dragonsobservers(s.dragonsobservers),
                                  bulletName(s.bulletName),
                                  bulletInterval(s.bulletInterval),
                                  timeSinceLastBullet(s.timeSinceLastBullet),
                                  minBulletSpeed(s.minBulletSpeed),
                                  bullets(s.bullets),
                                  facing(RIGHT)
{ }

// Player& Player::operator=(const Player& s) {
//   TwoWayMultiSprite::operator=(s);
//   collision = s.collision;
//   initialVelocity = s.initialVelocity;
//   slowDownFactor = s.slowDownFactor;
//   observers = s.observers;
//   dragonsobservers = s.dragonsobservers;
//   return *this;
// }

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

void Player::draw() const {
  TwoWayMultiSprite::draw();
  bullets.draw();
}

void Player::update(Uint32 ticks) {
  TwoWayMultiSprite::update(ticks);
  timeSinceLastBullet += ticks;
  bullets.update(ticks);
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }
  std::list<SmartTwoWayMultiSprite *>::iterator it = dragonsobservers.begin();
  while (it != dragonsobservers.end())
  {
    (*it)->setPlayerPos(getPosition());
    ++it;
  }
  if(getVelocityX()>0)
    facing=RIGHT;
  else if(getVelocityX()<0)
    facing = LEFT;
  stop();
}

void Player::attach(SmartSprite* o) {
  observers.push_back(o);
}

void Player::attach(SmartTwoWayMultiSprite *o){
  dragonsobservers.push_back(o);
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

void Player::detach(SmartTwoWayMultiSprite *o)
{
  std::list<SmartTwoWayMultiSprite *>::iterator ptr = dragonsobservers.begin();
  while (ptr != dragonsobservers.end())
  {
    if (*ptr == o)
    {
      ptr = dragonsobservers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Player::shoot(){
  if(timeSinceLastBullet>bulletInterval){
    Vector2f vel = getVelocity();
    float x;
    float y= getY()+getScaledHeight()/4;
    if(vel[0]>0){
      x = getX()+getScaledWidth();
      vel[0]+=minBulletSpeed;
    }else if(vel[0]<0){
      x=getX();
      vel[0] -= minBulletSpeed;
    }else if(facing == RIGHT){
      x = getX()+getScaledWidth();
      vel[0] += minBulletSpeed;
    }else if(facing==LEFT){
      x = getX();
      vel[0] -= minBulletSpeed;
    }
    bullets.shoot(Vector2f(x,y),vel);
    timeSinceLastBullet = 0;
  }
}
