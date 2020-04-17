#include "Object.h"
#include<cstdlib>
Object::Object(xy position, Action direction, int refresh_interval)
	:pos(position), interval(refresh_interval),time(cur_time)
{
    set_direction(direction);
}

Object::Object(const Object& obj, const int refresh_interval)
    : pos(obj.position), dir(obj.dir),interval(refresh_interval){}

void Object::move_to(xy new_position) {
    pos = new_position;
}


void Object::set_direction(Action direction) {
    switch (direction)
    {
    case up:
        dir.x = -1;
        break;
    case down:
        dir.x = 1;
        break;
    case left:
        dir.y = -1;
        break;
    case right:
        dir.y = 1;
        break;
    default:
        break;
    }
}

xy Object::position() const{
    return pos;
}

Action Object::direction() const {
    if (dir.x == -1)
        return up;
    else if (dir.x == 1)
        return down;
    else if (dir.y == -1)
        return left;
    else return right;
}

int Object::start_time() const {
    return time;
}

Tank::Tank(int attack, int health_point, int refresh_interval, xy position, Action direction)
    :atk(attack), hp(health_point), Object(position, direction, refresh_interval){
    actn = still;
}

int Tank::get_hp() const {
    return hp;
}

int Tank::attacked() {
    return --hp;
}

Action Tank::next_action() const{
    return actn;
}

Enemy::Enemy(int attack, int health_point, int refresh_interval, xy position, Action direction)
    :Tank(attack, health_point, refresh_interval, position, direction){}


Action Enemy::act() {
    int code = rand() % 10;
    
    if (!code)
        return actn;
    else if (code <= 2)
        actn = up;
    else if (code <= 4)
        actn = down;
    else if (code <= 6)
        actn = left;
    else if (code <= 8)
        actn = right;
    else actn = shoot;
    
    return actn;
}

Player::Player(int attack, int health_point, int refresh_interval, xy position, Action direction)
    :Tank(attack, health_point, refresh_interval, position,direction){}

Action Player::act(Action action) {
    return actn = action;
}

Bullet::Bullet(const Tank& tank,const int refresh_interval) 
    :Object(tank,refresh_interval){}

int Bullet::get_attack() const {
    return atk;
}