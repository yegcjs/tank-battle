#include "Map.h"
#include<cstdio>
#include<cstdlib>
Map::Map()
	:player(1,1,1,xy(98,2),right)
{
	//map
	FILE* mapfile = fopen("./maps/map1.txt", "r");
	if (!mapfile) {
		printf("Error:Can not open map1.txt\n");
		system("pause");
		exit(1);
	}
	int cnt_spown_point=0;
	for(int i=0;i<100;i++)
		for (int j = 0; j < 100; j++) {
			int tmp;
			if (fscanf(mapfile, "%d", &tmp) <= 0) {
				printf("Error: This map is not complete!\nPlease consider change a map.\n");
				system("pause");
				exit(-1);
			}
			if (tmp == -9) {
				enemy_spown_point[++cnt_spown_point] = xy(i, j);
				blocks[i][j] = empty;
			}
			else blocks[i][j] = (Block)tmp;
		}

	//
}

void Map::add_enemy() {
	xy PSpown;
	int code = rand() % 3;
	while (1) {
		PSpown = enemy_spown_point[code];
		if (tank_OK(PSpown))
			break;
		else code = (++code) % 3;
	}

	enemies.insert(new Enemy(1, rand() % 3 + 1, rand() % 2 + 2, 
					PSpown, down));
}

bool Map::tank_OK(xy pos) {
	int x = pos.x, y = pos.y;
	if (x <= 0 || x >= HEIGHT || y <= 0 || y >= WIDTH)
		return false;
	for(int i=-1;i<=1;i++)
		for(int j=-1;j<=1;j++)
			if (blocks[x + i][y + j] > 0 || isTank[x + i][y + j])
				return false;
	
}