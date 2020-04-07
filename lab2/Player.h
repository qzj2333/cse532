#pragma once
#include "Play.h"

class Player {
private:
	Play &currPlay;
	thread t;
	vector<container> content;
	//future<int> f;
	queue<shared_ptr<Fragment>> fragment_queue;
	mutex m;
	condition_variable cv;
public:
	static bool end;
	Player(Play& p);
	~Player();
	void prepare();	// !!!!
	void read(shared_ptr<Fragment>& f);
	void act(unsigned int frag_num);
	void enter(shared_ptr<Fragment>& fragment);
	void exit();
};