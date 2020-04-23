#include<iostream>
#include"Function.h"
#include<Windows.h>

using namespace std;

int main() {
	//system("CHCP 65001");
	int result_code;
	do {
		int map = start();
		result_code = gaming(map);
	} while (end(result_code));

	return 0;
}