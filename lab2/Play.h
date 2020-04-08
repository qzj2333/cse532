#if ! defined (PLAY_H)
#define PLAY_H
#include "common.h"

// should move to Player class later
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
	mutex mprint;
	condition_variable cv;
	unsigned int line_counter = 1;
	unsigned int scene_fragment_counter = 0;
	unsigned int on_stage = 0;	// # of characters currently on stage
	vector<string> names;
	vector<string>::iterator it;

public:
	static bool end;
	string currCharacter;
	Play(vector<string>& n);
	void recite(vector<container>::iterator& iter, unsigned int& scene_fragment_number);
	int enter(shared_ptr<Fragment> f);
	int exit(shared_ptr<Fragment> f);
};

#endif /* defined PLAY_H */