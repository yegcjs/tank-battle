#pragma once
#define HEIGHT 30
#define WIDTH 30
#include"Object.h"
#include"Buff.h"
#include<set>
#include<vector>

extern int cur_time;


class Map
{
friend class Painter;

private:

	enum Block{empty=0,brick=1,iron=2,grass=-1,base=-2};//positive value = defense
	Block blocks[HEIGHT][WIDTH];
	Tank *cur_tanks[HEIGHT][WIDTH];//blocks[i][j] is ccupied by cur_tanks[i][j]
	Bullet* cur_bullets[HEIGHT][WIDTH];//blocks[i][j] is ccupied by cur_bullets[i][j]
	vector<xy> enemy_spown_point;
	xy player_revive_point;
	Buff::buff_type cur_buffs[HEIGHT][WIDTH];

	set<Enemy*> enemies;
	set<Bullet*> bullets;
	set<Bullet*> mybullets;
	set<Buff*> buffs;
	Player *player;
	//int life_remain;
	
	//return whether a tank can be put at pos
	bool tank_OK(xy pos);

	struct CONFIG {
		int goal, killed_enemies;
		int life_remain;
		int hp;
		CONFIG() { 
			goal = 10;
			killed_enemies = 0;
			life_remain = 3;
			hp = 1;
		}
	}info;

	Painter *painter;
public:
	//init
	Map(int map_num);

	~Map();

	//generate an enemy
	void add_enemy();

	//
	void add_buff();

	//set enemies' next action
	void set_enemy_act();

	//get players next action
	void set_player_act();

	//player revive
	void reset_player();

	//update the tanks and bullet, and repaint them
	//return win:1 or lose:-1 or continue:0
	int update();

	//repaint the screen
	void paint();
};