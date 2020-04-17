#pragma once
#define HEIGHT 100
#define WIDTH 100
#include"Object.h"
#include"Painter.h"
#include<set>

extern int cur_time;

class Map
{
friend class Painter;

private:
	enum Block{empty=0,brick=1,iron=2,grass=-1,base=-2};//positive value = defense
	Block blocks[HEIGHT][WIDTH];
	bool isTank[HEIGHT][WIDTH];//whether the block is occupied by a tank
	set<Enemy*> enemies;
	set<Bullet*> bullets;
	Player player;

	xy enemy_spown_point[3];

	//return whether a tank can be put at pos
	bool tank_OK(xy pos);
public:
	//init
	Map();

	//generate an enemy
	void add_enemy();

	//set enemies' next action
	void set_enemy_act();

	//get players next action
	void set_player_act();

	//player revive
	void reset_player();

	//update the action to the tanks
	void update();

	//repaint the screen
	void paint();
};