#pragma once
#include<map>
#include<string>
#include"Object.h"
#include"Map.h"
//extern struct xy;

class Painter
{
private:
	map<Map::Block, string> block_icon;
	//[speed][x][y] in a upwards tank
	string tank_icons[5][3][3];
	string bullet_icon;
	int colors[20];
public:
	//read from files 
	Painter();
	void reset();
	void paint_tank(const Tank &tank);
	void paint_map(const Map& map);
	void paint_bullet(const Bullet& bullet);
};

