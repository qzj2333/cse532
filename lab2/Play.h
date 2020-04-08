// header file of play class

#if ! defined (PLAY_H)
#define PLAY_H
#include "common.h"

// contains info for a line in any character file
struct container
{
	unsigned int order;
	string characterName;
	string text;

	bool operator<(const container& c);
};

class Play
{
private:
	mutex m;
	condition_variable cv;
	unsigned int line_counter;	// line counter of current fragment
	unsigned int scene_fragment_counter;	// fragment counter
	unsigned int on_stage;	// # of characters currently on stage
	vector<string> names;	// all scene names of current script
	vector<string>::iterator it;	// iterator for the scene names vector

public:
	string currCharacter;	// current active character of the play, "" if none
	Play(vector<string>& n);
	void recite(vector<container>::iterator& iter, unsigned int& scene_fragment_number);
	int enter(shared_ptr<Fragment> f);
	int exit(shared_ptr<Fragment> f);
};

#endif /* defined PLAY_H */