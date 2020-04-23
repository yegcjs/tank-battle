#pragma once

using namespace std;

extern int cur_time;

struct xy {
	int x, y;
	xy(int X, int Y)
		:x(X), y(Y){}
	xy() { x = y = 0; }
	xy operator+(const xy& tmp) {
		return xy(x + tmp.x, y + tmp.y);
	}
	xy operator-(const xy& tmp) {
		return xy(x - tmp.x, y - tmp.y);
	}
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
	//if time is up for the object to update
	bool timeup();
	void move();
	void set_direction(Action direction);
	int start_time() const;
	int get_interval() const;
	int set_interval(int new_interval);//return the old one
	xy position() const;
	xy direction() const;
};

class Tank :public Object {
protected:
	int atk, hp;
	bool shooting;
	Action actn;
public:
	Tank(int attack,int health_point,int refresh_interval,xy position,Action direction);
	//return remained hp
	int attacked(int bullet_attack);
	int get_hp()const;
	int get_attack()const;
	Action next_action() const;
	bool isShooting() const;
	void stopShooting();
	int set_hp(int new_hp);//return the previous hp
	int set_attack(int new_attack);//return the previous atk
};

class Enemy : public Tank {
public:
	Enemy(int attack, int health_point, int refresh_interval, xy position, Action direction);
	//probablity: 35% not change, 20% each direction, 10 shooting
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
