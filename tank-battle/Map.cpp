#define _CRT_SECURE_NO_WARNINGS
#include "Map.h"
#include<cmath>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<conio.h>
#include<Windows.h>
#include<conio.h>
#include"Painter.h"
Map::Map(int map_num)
	:player(NULL)
{
	painter = new Painter;
	string MapID = to_string(map_num);
	//map configure
	string file_name = "./maps/map" + MapID + "_config.txt";
	FILE* config = fopen(file_name.c_str(), "r");
	if (!config) {
		printf("Error:Can not open %s\n",file_name.c_str());
		system("pause");
		exit(1);
	}
	//const char available_options[10][50] = { "goal","life" };
	while (!feof(config)) {
		char option[50];
		int value;
		if (fscanf(config, "%s:%d", option, &value) <= 0) {
			if (!info.goal) info.goal = 10;
			if (!info.life_remain) info.life_remain = 4;
			break;
		}
		if (!strcmp(option, "goal"))
			info.goal = value;
		else if (!strcmp(option, "life"))
			info.life_remain = value;
		else if (!strcmp(option, "HP"))
			info.hp = value;
	}

	//map blocks
	file_name = "./maps/map" + MapID + ".csv";
	FILE* mapfile = fopen(file_name.c_str(), "r");
	if (!mapfile) {
		printf("Error:Can not open %s\n",file_name.c_str());
		system("pause");
		exit(-1);
	}
	bool flag_enemy = 0, flag_player = 0;
	for(int i=0;i<HEIGHT;i++)
		for (int j = 0; j < WIDTH; j++) {
			int tmp;
			//printf("%d,%d", i ,j);
			if (fscanf(mapfile, "%d,", &tmp) <=0) {
				printf("Error: This map is not complete!\nPlease consider change a map.\n");
				system("pause");
				exit(-1);
			}
			if (tmp == -9) {
				flag_enemy = true;
				enemy_spown_point.push_back(xy(i, j));
				blocks[i][j] = empty;
			}
			else if (tmp == -8) {
				flag_player = true;
				player_revive_point = xy(i, j);
				blocks[i][j] = empty;
			}
			else blocks[i][j] = (Block)tmp;
		}
	if (!(flag_enemy && flag_player)) {
		printf("Error:map1.txt is not complete\n");
		system("pause");
		exit(1);
	}
	reset_player();
	//
	memset(cur_tanks, 0, sizeof(cur_tanks));
	memset(cur_bullets, 0, sizeof(cur_bullets));
	memset(cur_buffs, 0, sizeof(cur_buffs));
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			cur_tanks[player->position().x + i][player->position().y + j] = player;
}

void Map::add_enemy() {
	static int time_stamp=0;
	time_stamp = cur_time < time_stamp ? cur_time : time_stamp;
	if (cur_time - time_stamp < 50)//50 round a new enemy
		return;
	
	xy PSpown;
	int code = rand() % enemy_spown_point.size();
	for(int i=0;i<enemy_spown_point.size();i++){
		PSpown = enemy_spown_point[(code+i)%enemy_spown_point.size()];
		if (tank_OK(PSpown)) {
			time_stamp = cur_time;
			break;
		}
	}

	if (time_stamp == cur_time) {
		int q=rand() % 10;
		Enemy* tmp = new Enemy(q<8?1:2, rand() % 3 + 1, 2*(rand() % 2 + 1), PSpown, down);
		enemies.insert(tmp);
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				cur_tanks[PSpown.x + i][PSpown.y + j] = tmp;
	}
}

void Map::add_buff() {
	static int buff_cnt = 0;
	buff_cnt = cur_time < buff_cnt ? cur_time: buff_cnt;
	
	//5 killed enemy for one buff
	if (info.killed_enemies / 5 > buff_cnt)
		buff_cnt++;
	else return;

	xy pos = xy(rand() % HEIGHT, rand() % WIDTH);
	while(blocks[pos.x][pos.y]!=empty || cur_buffs[pos.x][pos.y]!=Buff::none|| cur_tanks[pos.x][pos.y])
		pos = xy(rand() % HEIGHT, rand() % WIDTH);
	
	cur_buffs[pos.x][pos.y] = (Buff::buff_type)(rand() % 4 + 1);
	painter->paint_buff(pos, cur_buffs[pos.x][pos.y]);
}

bool Map::tank_OK(xy pos) {
	int x = pos.x, y = pos.y;
	if (x <= 0 || x+1 >= HEIGHT || y <= 0 || y+1 >= WIDTH)
		return false;
	for(int i=-1;i<=1;i++)
		for(int j=-1;j<=1;j++)
			if (blocks[x + i][y + j] > 0 || cur_tanks[x + i][y + j])
				return false;
	return true;
}

void Map::set_enemy_act() {
	for (set<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); it++) 
		(*it)->act();
}

void Map::set_player_act() {
	if (GetAsyncKeyState('W'))
		player->act(up);
	else if (GetAsyncKeyState('S'))
		player->act(down);
	else if (GetAsyncKeyState('A'))
		player->act(left);
	else if (GetAsyncKeyState('D'))
		player->act(right);
	else if (GetAsyncKeyState('J'))
		player->act(shoot);
	else player->act(still);
}

void Map::reset_player() {
	if (player) {
		for (set<Buff*>::iterator it = buffs.begin(); it != buffs.end(); /**/) {
			Buff* tmp = *it;
			it++;
			if (tmp->get_owner() == player) {
				buffs.erase(tmp);
				delete tmp;
			}
		}
		delete player;
		player = NULL;
	}

	//xy should change to base
	if (!tank_OK(player_revive_point))
		return;

	player = new Player(1, info.hp, 4, player_revive_point, right);
	
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			cur_tanks[player->position().x + i][player->position().y + j] = player;
}

int Map::update() {
	//static Painter painter;

	//step1: move all tank or the tank can shoot
	//step2: move bullet and attack
	//step3: check for whether a buff is timeout

	/*Tank's update assisting functions*/
	//update cur_tanks
	auto update_stamp = [this](Tank* tank) {
		xy pos = tank->position();
		Tank* tag = NULL;
		if (!cur_tanks[pos.x][pos.y])
			tag = tank;
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				cur_tanks[pos.x + i][pos.y + j] = tag;
	};

	auto try_get_buff = [this](Tank* tank) {
		xy pos = tank->position();
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++) {
				if (cur_buffs[pos.x+i][pos.y+j] != Buff::none) {
					buffs.insert(new Buff(tank, cur_buffs[pos.x + i][pos.y + j], 100 * log(info.killed_enemies)));
					cur_buffs[pos.x + i][pos.y + j] = Buff::none;
					return;
				}
			}
	};

	//try to move, if ok:move, else stay
	auto try_move = [this, update_stamp,try_get_buff](Tank* tank) {
		xy cur_pos = tank->position();
		xy nxt_pos = cur_pos + tank->direction();
		update_stamp(tank);
		if (tank_OK(nxt_pos)) {
			painter->depaint_tank(tank, blocks);
			tank->move();
			painter->paint_tank(tank, blocks);
			update_stamp(tank);
			try_get_buff(tank);
			return true;
		}

		update_stamp(tank);
		try_get_buff(tank);
		return false;
	};

	auto shoot = [this](Tank* tank) {
		Bullet* tmp = new Bullet(*tank, 1);
		bullets.insert(tmp);
		if(player && tank==player)
			mybullets.insert(tmp);
		cur_bullets[tmp->position().x][tmp->position().y];
		//painter.paint_bullet(tmp, blocks);
		tank->stopShooting();
	};


	//************************//
	//        STEP 1          //
	//************************//
	//memset(cur_tanks, 0, sizeof(cur_tanks));	

	//update tanks
	if (!player)
		reset_player();
	else if (player->timeup()) {
		if (player->next_action() != still)
			try_move(player);
		if (player->isShooting())
			shoot(player);
	}
	for (set<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); it++) {
		if ((*it)->timeup()) {
			if ((*it)->next_action() != still)
				try_move(*it);
			if ((*it)->isShooting())
				shoot(*it);
		}
	}


	//************************//
	//        STEP 2          //
	//************************//
	//move bullets and attack
	for (set<Bullet*>::iterator it = bullets.begin(); it != bullets.end();/*it update in loop*/) {
		Bullet* cur = *it;
		it++;
		xy pos = cur->position();
		if (cur->timeup()) {
			if (cur->start_time() != cur_time)
				painter->paint_block(blocks, pos);
			cur_bullets[pos.x][pos.y] = NULL;
			cur->move();

			pos = cur->position();
			if (pos.x < 0 || pos.x >= HEIGHT || pos.y < 0 || pos.y >= WIDTH) {
				if (mybullets.count(cur)) mybullets.erase(cur);
				bullets.erase(cur); delete cur;
				continue;
			}
			if (cur_bullets[pos.x][pos.y]) { //attack bullet
				Bullet* tmp = cur_bullets[pos.x][pos.y];
				if (it != bullets.end() && tmp == *it) it++;
				cur_bullets[pos.x][pos.y] = NULL;
				painter->paint_block(blocks, pos);

				if (mybullets.count(tmp)) mybullets.erase(tmp);
				bullets.erase(tmp); delete tmp;
				if (mybullets.count(cur)) mybullets.erase(cur);
				bullets.erase(cur); delete cur;
				continue;
			}
			else cur_bullets[pos.x][pos.y] = cur;
		}

		if ((int)blocks[pos.x][pos.y] > 0) { //attack block
			if ((int)blocks[pos.x][pos.y] <= cur->get_attack()) {
				blocks[pos.x][pos.y] = empty;
				painter->paint_block(blocks, pos);
			}
			cur_bullets[pos.x][pos.y] = NULL;
			if (mybullets.count(cur)) mybullets.erase(cur);
			bullets.erase(cur);
			delete cur;
		}
		else if (cur_tanks[pos.x][pos.y]) { //attack tank
			Tank* tank = cur_tanks[pos.x][pos.y];
			if (player && tank == player) {
				if (tank->attacked(cur->get_attack()) <= 0) {
					painter->depaint_tank(tank, blocks);
					update_stamp(tank);
					reset_player();
					if (--info.life_remain <= 0)
						return -1;
					else painter->paint_tank(player, blocks);
				}
			}
			else if (enemies.count((Enemy*)tank)) {
				if (tank->attacked(cur->get_attack()) <= 0) {
					painter->depaint_tank(tank, blocks);
					update_stamp(tank);
					enemies.erase((Enemy*)tank);
					delete (Enemy*)tank;
					if (mybullets.count(cur)) {
						if (++info.killed_enemies >= info.goal)
							return 1;
					}
				}
			}
			else continue;

			if (mybullets.count(cur)) mybullets.erase(cur);
			cur_bullets[pos.x][pos.y] = NULL;
			bullets.erase(cur); delete cur;
		}
		else painter->paint_bullet(cur, blocks);
	}

	for (set<Buff*>::iterator it = buffs.begin(); it != buffs.end();) {
		Buff* tmp = *it;
		it++;
		if (tmp->timeout()) {
			buffs.erase(tmp);
			delete tmp;
		}
		else painter->paint_buff(tmp->owner_position(), tmp->get_type());
	}

	//add_enemy();
	painter->paint_info(info,player);
	return 0;
}

void Map::paint() {
	//static Painter painter;
	system("cls");

	painter->paint_map(this);
	for(set<Bullet*>::iterator it=bullets.begin();it!=bullets.end();it++)
		painter->paint_bullet(*it,blocks);
	if(player)
		painter->paint_tank(player,blocks);
	for (set<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); it++)
		painter->paint_tank(*it, blocks);
	painter->paint_frame();
}

Map::~Map() {
	for (set<Enemy*>::iterator it = enemies.begin(); it != enemies.end(); it++)
		delete *it;
	if(player)
		delete player;
	for (set<Bullet*>::iterator it = bullets.begin(); it != bullets.end(); it++)
		delete* it;
}