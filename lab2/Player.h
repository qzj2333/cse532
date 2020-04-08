#if ! defined (PLAYER_H)
#define PLAYER_H

#include "Play.h"

class Player {
private:
	Play& currPlay;
	thread t;
	vector<container> content;
	queue<shared_ptr<Fragment>> fragment_queue;
	mutex m;
	condition_variable cv;
	
public:
	bool end;
	Player(Play& p);
	~Player();
	void prepare();	// !!!!
	void read(shared_ptr<Fragment>& f);
	void act(shared_ptr<Fragment>& f);
	void enter(shared_ptr<Fragment>& fragment);
};

#endif