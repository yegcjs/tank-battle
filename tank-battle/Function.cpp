#include"Function.h"
#include"Map.h"
#include<iostream>
using namespace std;

void start() {
	cout << "Press Enter to continue..." << endl;
	while (getchar() != '\n');
	return;
}

int gaming() {
	/*cout << "this is gaming" << endl;
	if (getchar() == '\n')
		return 0;
	*/
	while (1) {
		Map game;
		game.paint();
		game.set_enemy_act();
		game.set_player_act();
		game.update();
		return 1;//lose
	}
	
	return 0;//win
}

bool end(int result) {
	if (!result) {
		//win the game
		cout << "you win, press enter to try again" << endl;
		if (getchar() == '\n')
			return true;
		return false;
	}
	return true;
}