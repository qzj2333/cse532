#pragma once
#include "Play.h"

class Player {
private:
	Play &currPlay;
	thread t;
	vector<container> content;
public:
	Player(Play& p);
	void read();
	void act();
	void enter();
	void exit();
};