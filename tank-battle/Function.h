#pragma once

//return map selected
int start();

//return a code for game result
//if win: 0, lose: -1
int gaming(int map_num);

//show ending according to the game result
//return true if restart, return 0 if not
bool end(int result);
