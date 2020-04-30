// header file for the play class

#if ! defined (PLAYER_H)
#define PLAYER_H
#include "Play.h"

class Player {
private:
	Play& currPlay;	// the play it serves for
	thread t;	// corresponding thread of current player
	vector<container> content;	// contains all contents of a fragment in the working queue 
	queue<shared_ptr<Fragment>> fragment_queue;	// working queue contains all fragment/tasks for current thread
	mutex m;
	condition_variable cv;
	promise<void> exitSignal;
	future<void> futureObj = exitSignal.get_future();
public:
	bool end;	// true if current player/thread takes no fragments
	Player(Play& p);
	void prepare();	
	int read(shared_ptr<Fragment>& f);
	void act(shared_ptr<Fragment>& f);
	void enter(shared_ptr<Fragment>& fragment);
	bool isStop();
	void stop();
};

#endif