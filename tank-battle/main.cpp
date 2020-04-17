#include<iostream>
#include"Function.h"
using namespace std;

int main() {
	
	int result_code;
	do {
		start();
		result_code = gaming();
	} while (end(result_code));

	return 0;
}