#pragma once
#include "Play.h"


class Player {
private:
	//ifstream &file;
	shared_ptr<ifstream> file;
	const string name; // need reference! But reference makes name becomes empty
	Play &currPlay;
	thread t;
	vector<container> content;
public:
	Player(Play& p,const string& charName, shared_ptr<ifstream> inputFile); // !!!!!!!!!!!!!! should be ifstream&
	void read();
	void act();
	void enter();
	void exit();

};