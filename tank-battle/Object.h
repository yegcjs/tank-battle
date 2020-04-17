#pragma once

using namespace std;

extern int cur_time;

struct xy {
	int x, y;
	xy(int X, int Y)
		:x(X), y(Y){}
	xy() { x = y = 0; }
};
enum Action{up,down,left,right,shoot,still};

class Object
{
protected:
	xy pos,dir;
	int interval;
	const int time;
public:
	Object(xy position, Action direction, int refresh_interval);
	Object(const Object& obj, const int refresh_interval);
	void move_to(xy new_position);
	void set_direction(Action direction);
	int start_time() const;
	xy position() const;
	Action direction() const;
};

class Tank :public Object {
protected:
	int atk, hp;
	Action actn;
public:
	Tank(int attack,int health_point,int refresh_interval,xy position,Action direction);
	//return remained hp
	int attacked();
	int get_hp()const;
	Action next_action() const;
};

class Enemy : public Tank {
public:
	Enemy(int attack, int health_point, int refresh_interval, xy position, Action direction);
	//probablity: 10% not change, 20% each direction, 10% shoot
	Action act();
};

class Player : public Tank {
public:
	Player(int attack, int health_point, int refresh_interval, xy position, Action direction);
	Action act(Action action);
};

class Bullet:public Object{
private:
	int atk;
public:
	Bullet(const Tank &tank,const int refresh_interval);
	int get_attack() const;
};
