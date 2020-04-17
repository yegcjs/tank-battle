#pragma once

int cur_time;

void start();

//return a code for game result
//if win: 0
int gaming();

//show ending according to the game result
//return true if restart, return 0 if not
bool end(int result);