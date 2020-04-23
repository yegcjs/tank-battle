#pragma once
#include<map>
#include<string>
#include"Object.h"
#include"Map.h"

//extern struct xy;

//using namespace std;

class Painter
{
private:
	map<Map::Block, string> block_icon;
	map<Buff::buff_type, string> buff_icon;
	//[speed][x][y] in a upwards tank
	string tank_icons[5];
	string bullet_icon;
	enum color {black,blue,green,azure,red,purple,orange,white,grey};
public:
	//read from files 
	Painter();
	void paint_frame();
	void paint_tank(const Tank *tank, const Map::Block blocks[HEIGHT][WIDTH]);
	void depaint_tank(const Tank* tank, const Map::Block blocks[HEIGHT][WIDTH]);
	void paint_map(const Map *map);
	void paint_block(const Map::Block blocks[HEIGHT][WIDTH], const xy position);
	void paint_bullet(const Bullet *bullet, const Map::Block blocks[HEIGHT][WIDTH]);
	void paint_info(const Map::CONFIG &config,const Tank* player);
	void paint_buff(xy position, Buff::buff_type type);
	void locate(xy coordinate);
};

