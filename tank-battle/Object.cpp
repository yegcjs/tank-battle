#include "Object.h"
#include<cstdlib>
Object::Object(xy position, Action direction, int refresh_interval)
	:pos(position), interval(refresh_interval),time(cur_time)
{
    set_direction(direction);
}

Object::Object(const Object& obj, const int refresh_interval)
    : pos(obj.position()), dir(obj.dir),interval(refresh_interval),time(cur_time){}

bool Object::timeup() {
    return !((cur_time - time) % interval);
}

void Object::move() {
    pos.x += dir.x;
    pos.y += dir.y;
}


void Object::set_direction(Action direction) {
    switch (direction)
    {
    case up:
        dir = xy(-1, 0);
        break;
    case down:
        dir = xy(1, 0);
        break;
    case left:
        dir = xy(0, -1);
        break;
    case right:
        dir = xy(0, 1);
        break;
    default:
        break;
    }
}

xy Object::position() const{
    return pos;
}

xy Object::direction() const {
    return dir;
}

int Object::start_time() const {
    return time;
}

int Object::get_interval() const {
    return interval;
}

int Object::set_interval(int new_interval) {
    int tmp = interval;
    interval = new_interval > 0 ? new_interval : 1;
    return tmp;
}

Tank::Tank(int attack, int health_point, int refresh_interval, xy position, Action direction)
    :atk(attack), hp(health_point), Object(position, direction, refresh_interval),shooting(false){
    actn = still;
}

int Tank::get_hp() const {
    return hp;
}

int Tank::set_hp(int new_hp) {
    int tmp = hp;
    hp = new_hp;
    return tmp;
}

int Tank::set_attack(int new_attack) {
    int tmp = atk;
    atk = new_attack;
    return tmp;
}

int Tank::attacked(int bullet_attack) {
    return (hp=hp-bullet_attack);
}

int Tank::get_attack() const {
    return atk;
}

Action Tank::next_action() const{
    return actn;
}

bool Tank::isShooting() const {
    return shooting;
}

void Tank::stopShooting() {
    shooting = false;
}

Enemy::Enemy(int attack, int health_point, int refresh_interval, xy position, Action direction)
    :Tank(attack, health_point, refresh_interval, position, direction){}


Action Enemy::act() {
    int code = rand() % 100;
    
    if (code < 20)
        actn = up;
    else if (code < 40)
        actn = down;
    else if (code < 60)
        actn = left;
    else if (code < 80)
        actn = right;
    else if(code <85)
        shooting = true;
    set_direction(actn);
    return actn;
}

Player::Player(int attack, int health_point, int refresh_interval, xy position, Action direction)
    :Tank(attack, health_point, refresh_interval, position,direction){}

Action Player::act(Action action) {
    if (action == shoot) {
        shooting = true;
        return action;
    }
    actn = action;
    set_direction(action);
    return actn;
}

Bullet::Bullet(const Tank& tank,const int refresh_interval) 
    :Object(tank,refresh_interval){
    pos = pos + dir;
    atk = tank.get_attack();
}

int Bullet::get_attack() const {
    return atk;
}