#include"Function.h"
#include"Map.h"
#include<conio.h>
#include<windows.h>
#include<iostream>
using namespace std;

int cur_time;

int start() {
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	system("chcp 936");
	system("cls");

	//paint welcome
	COORD coord;
	coord.X = 15;	coord.Y = 10;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);//green
	for (int i = 0; i <= 4; i++, coord.Y++) {
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		if (i == 0)
			cout << "~¡ñ¨€¡þ¨€¨€¨{¨{¨{¨{¨{¨{ ¡ñ¡¡¡ñ¡¡¡ñ";
		else if (i == 1)
			cout << "¨{¨|¨€¨€¨€¨€¨€¨€¨|¨{¨z¨y";
		else if (i == 2)
			cout << "¨{¨|¨€¨€¨€¨€¨€¨€¨|¨{¨z¨y";
		else if (i == 3)
			cout << "¨€¨€¨€¨€¨€¨€¨€¨€¨€¨€¨€¨€¨€¨€";
		else cout << "¨¡Ñ¡ø¡Ñ¡ø¡Ñ¡ø¡Ñ¡ø¡Ñ¡ø¡Ñ¡ø¨";
	}
	coord.X = 20; coord.Y = 16;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);//whie
	cout << "Press Enter to continue..." << endl;
	while (_getch() != 13);


	system("cls");

	coord.X = 20; coord.Y = 10;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	cout << "ÇëÑ¡ÔñµØÍ¼£º" << endl;

	int selected = 1;
	char key=0;
	do {
		//cout << key << endl;
		if (key == 72) selected = max(1, selected - 1);
		else if (key == 80) selected = min(4, selected + 1);
		for (int i = 1; i <= 4; i++) {
			coord.X = 18; coord.Y = 12 + i;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			if (i == selected)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);//orange
			cout << ">> "<<"µØÍ¼ " << i;
			if(i==selected)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);//white
		}
	} while ((key = _getch()) != 13);//enter

	system("chcp 65001");
	system("cls");
	return selected;
}

int gaming(int map_num) {
	Map *game=new Map(map_num);
	cur_time = 0;
	game->paint();

	do {
		//game->paint();
		game->set_enemy_act();
		game->set_player_act();
		int status = game->update();
		if (status) { // win:1 lose -1
			delete game;
			return status > 0 ? 0 : -1;
		}
		game->add_enemy();
		game->add_buff();
		Sleep(50);
	} while (++cur_time);
	system("cls");
}

bool end(int result) {

	COORD coord;
	coord.X = 15;	coord.Y = 10;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	
	//print a frame in 80x40 game frame
	//(25,15) -> (55,25)
	coord.X = 25; coord.Y = 15;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	cout << "*";
	for (int i = 26; i < 55; i++)
		cout << "-";
	cout << "*";
	for (int j = 16; j < 25; j++) {
		coord.X = 25; coord.Y = j;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		cout << "|";
		for (int i = 26; i < 55; i++)
			cout << " ";
		cout << "|";
	}
	coord.X = 25; coord.Y = 25;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	cout << "*";
	for (int i = 26; i < 55; i++)
		cout << "-";
	cout << "*";


	//print game info
	coord.X = 30; coord.Y = 17;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	if (!result)  //result == 0 means win
		cout << "You win!";
	else cout << "You lose..";
	
	coord.X = 27; coord.Y = 21;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	cout << "[Enter] to try again";
	coord.X = 27; coord.Y = 22;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	cout<<"    [Q] to quit." << endl;

	char c;
	while ((c=_getch()) != 'Q' && (c!='q'))
		if(c==13) return true;
	return false;
}