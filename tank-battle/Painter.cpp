#include "Painter.h"
#include<fstream>
#include<iostream>
#include<windows.h>

using namespace std;

Painter::Painter() {
	//set console
	//read_icon
	auto read_icon = [](const char *dir) {
		ifstream in(dir);
		//string icon;
		if (in) {
			char icon[10];
			in.getline(icon,10);
			return string(icon);
		}
		return string("??");
	};

	//Block
	block_icon.insert(pair<Map::Block, string>(Map::empty, read_icon((char*)"icons/empty.icon")));
	block_icon.insert(pair<Map::Block, string>(Map::brick, read_icon((char*)"icons/brick.icon")));
	block_icon.insert(pair<Map::Block, string>(Map::iron, read_icon((char*)"icons/iron.icon")));
	block_icon.insert(pair<Map::Block, string>(Map::grass, read_icon((char*)"icons/grass.icon")));
	block_icon.insert(pair<Map::Block, string>(Map::base, read_icon((char*)"icons/base.icon")));

	//tank_icon
	for (int i = 1; i < 5; i++) {
		char id[2] = "0"; id[0] += i;
		string dir = "icons/tank" + string(id) + ".icon";
		tank_icons[i] = read_icon(dir.c_str());
	}

	//bullet_icon
	bullet_icon = read_icon((char*)"icons/bullet.icon");

	//buff_icon
	buff_icon.insert(pair<Buff::buff_type, string>(Buff::invincible, read_icon((char*)"icons/invincible.icon")));
	buff_icon.insert(pair<Buff::buff_type, string>(Buff::addHP, read_icon((char*)"icons/addHP.icon")));
	buff_icon.insert(pair<Buff::buff_type, string>(Buff::power, read_icon((char*)"icons/power.icon")));
	buff_icon.insert(pair<Buff::buff_type, string>(Buff::acceleration, read_icon((char*)"icons/acceleration.icon")));

}



void Painter::paint_tank(const Tank* tank,const Map::Block blocks[HEIGHT][WIDTH]){
	
	
	//set color according to hp of the tank
	switch (tank->get_hp())
	{
	case 1:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)green);
		break;
	case 2:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)azure);
		break;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)purple);
		break;
	}

	auto paint_at = [this,blocks](xy position, const string& icon) {
		if (blocks[position.x][position.y] == Map::grass) return;
		locate(xy(position.x,position.y));
		printf("%s",icon.c_str());
	};

	//paint 6 blocks around
	xy direction = tank->direction();
	xy center = tank->position();
	int wheel = min(tank->get_interval() / 2, 4);
	if(blocks[center.x][center.y]!=Map::grass)
		paint_at(center,tank_icons[wheel]);
	
	xy front = center + direction;
	paint_at(front, tank_icons[wheel]);
	
	xy side1 = center + xy(direction.x ? 0 : 1, direction.y ? 0 : 1);
	paint_at(side1, tank_icons[wheel]);
	
	xy side2 = center + xy(direction.x ? 0 : -1, direction.y ? 0 : -1);
	paint_at(side2, tank_icons[wheel]);
	
	xy backside1 = center - direction + xy(direction.x ? 0 : 1, direction.y ? 0 : 1);
	paint_at(backside1, tank_icons[wheel]);
	
	xy backside2 = center - direction + xy(direction.x ? 0 : -1, direction.y ? 0 : -1);
	paint_at(backside2, tank_icons[wheel]);

}

void Painter::depaint_tank(const Tank* tank, const Map::Block blocks[HEIGHT][WIDTH]) {
	xy pos = tank->position();
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			paint_block(blocks, xy(pos.x + i, pos.y + j));
}

void Painter::paint_map(const Map* map) {
	for(int i=0;i<HEIGHT;i++)
		for (int j = 0; j < WIDTH; j++) {
			paint_block(map->blocks, xy(i, j));
/*			locate(xy(i, j));
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)white);
			printf("%s", block_icon.find(map->blocks[i][j])->second.c_str());*/
			//cout << block_icon.find(map->blocks[i][j])->second;
		}
}

void Painter::paint_block(const Map::Block blocks[HEIGHT][WIDTH], const xy position) {
	locate(position);
	Map::Block block = blocks[position.x][position.y];
	//enum color {black,blue,green,azure,red,purple,orange,white,grey};
	switch (block)
	{
	case Map::empty:
		break;
	case Map::brick:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), orange);
		break;
	case Map::iron:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), red);
		break;
	case Map::grass:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), green);
		break;
	case Map::base:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), purple);
		break;
	default:
		break;
	}
	//printf("  ");
	printf("%s", block_icon.find(block)->second.c_str());
}


void Painter::paint_bullet(const Bullet* bullet, const Map::Block blocks[HEIGHT][WIDTH]) {
	xy pos = bullet->position();
	if (blocks[pos.x][pos.y] == Map::grass) return;
	locate(pos);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), grey);
	printf("%s", bullet_icon.c_str());
	//cout << bullet_icon << endl;
}

void Painter::paint_info(const Map::CONFIG& config, const Tank* player){
	//config: goal, killed enemies, remain life
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), white);
	
	locate(xy(HEIGHT / 5, WIDTH + 5));
	printf("Goal:\n");
	locate(xy(HEIGHT / 5+1, WIDTH + 5));
	printf("%d", config.goal);
	
	locate(xy(2*HEIGHT / 5, WIDTH + 5));
	printf("Killed Enemies:\n");
	locate(xy(2 * HEIGHT / 5+1, WIDTH + 5));
	printf("%d", config.killed_enemies);

	locate(xy(3*HEIGHT / 5, WIDTH + 5));
	printf("Remained Life:\n");
	locate(xy(3 * HEIGHT / 5+1, WIDTH + 5));
	printf("%d", config.life_remain);

	locate(xy(4 * HEIGHT / 5, WIDTH + 5));
	printf("HP:\n");
	locate(xy(4 * HEIGHT / 5 + 1, WIDTH + 5));
	printf("%d", player->get_hp());

}

void Painter::locate(xy coordinate) {
	COORD coord;
	coord.X = coordinate.y*2+2;
	coord.Y = coordinate.x+1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Painter::paint_buff(xy position, Buff::buff_type type) {
	locate(position);
	switch (type)
	{
	case Buff::none:
		break;
	case Buff::invincible:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), blue);
		break;
	case Buff::addHP:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), red);
		break;
	case Buff::power:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), purple);
		break;
	case Buff::acceleration:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), azure);
		break;
	default:
		break;
	}
	printf("%s", buff_icon.find(type)->second.c_str());
}

void Painter::paint_frame() {

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), white);

	locate(xy(-1, -1));
	//printf("DEBUG");
	printf("/-");
	for (int i = 1; i < WIDTH; i++)
		printf("--");
	printf("-\\");
	for (int i = 1; i <= HEIGHT; i++) {
		locate(xy(i-1, -1));
		printf("|");
		locate(xy(i-1, WIDTH));
		printf("|");
	}
	locate(xy(HEIGHT, -1));
	printf("\\-");
	for (int i = 1; i < WIDTH; i++)
		printf("--");
	printf("-/");
}